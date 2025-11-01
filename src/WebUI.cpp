#include <string>
#include <cstring>
#include <vector>
#include <mutex>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <thread>
#include <chrono>
#include <unordered_map>

#include "civetweb.h"
#include <json/json.h>

#include "include/IO.h"
#include "include/Logger.h"
#include "include/MessageConsumer.h"

const char* PIPE_PATH = "/tmp/gameServer.fifo"; // Named pipe path

// --- WebSocket clients ---
std::vector<mg_connection*> ws_clients;
std::unordered_map<std::string, mg_connection*> token_to_client;
std::mutex ws_mutex;

// --- Extract query param token ---
std::string get_token_from_conn(mg_connection* conn) {
    const char* query = mg_get_request_info(conn)->query_string;
    if (!query) return "";
    std::string qs(query);
    auto pos = qs.find("token=");
    if (pos == std::string::npos) return "";
    return qs.substr(pos + 6, 16); // take 16 hex chars
}

// --- Send message to a single client ---
void send_to_client(const std::string& token, const std::string& msg, const std::string& panel) {
    std::lock_guard<std::mutex> lock(ws_mutex);
    auto it = token_to_client.find(token);
    if (it != token_to_client.end()) {
        //mg_websocket_write(it->second, MG_WEBSOCKET_OPCODE_TEXT, msg.c_str(), msg.size());

        Json::Value msgJson;
        msgJson["panel"] = panel;
        msgJson["text"] = msg;
        Json::StreamWriterBuilder writer;
        std::string msgStr = Json::writeString(writer, msgJson);

        mg_websocket_write(it->second, MG_WEBSOCKET_OPCODE_TEXT, msgStr.c_str(), msgStr.size());
    }
}

// --- Send command to the engine via named pipe ---
std::string send_to_engine(const std::string& cmd) {
    std::ofstream pipe(PIPE_PATH);
    if (!pipe.is_open()) return "Error: cannot open pipe";
    pipe << cmd << std::endl;
    pipe.close();

    // For demo purposes, we just echo back
    return "Engine received: " + cmd;
}

// --- HTTP POST /command handler ---
int command_handler(struct mg_connection* conn, void*) {
    char buf[1024];
    int len = mg_read(conn, buf, sizeof(buf) - 1);
    if (len < 0) len = 0;
    buf[len] = 0;
    std::string cmd(buf);

    // Sanitize input (ASCII printable only)
    cmd.erase(std::remove_if(cmd.begin(), cmd.end(), [](char c){ return c < 0x20 || c > 0x7E; }), cmd.end());

    std::string response = send_to_engine(cmd);

    // Broadcast to WebSocket clients
    {
        std::lock_guard<std::mutex> lock(ws_mutex);
        for (auto* ws : ws_clients)
            mg_websocket_write(ws, MG_WEBSOCKET_OPCODE_TEXT, response.c_str(), response.size());
    }

    // Return direct response to the HTTP client
    mg_printf(conn,
        "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n"
        "Access-Control-Allow-Origin: *\r\n\r\n%s", response.c_str());
    return 200;
}

// --- WebSocket handlers ---
int ws_connect(const struct mg_connection*, void*) { return 0; }

void ws_ready(struct mg_connection* conn, void* user_data) {
    std::string token = get_token_from_conn(conn);
    if (token.empty()) {
        mg_websocket_write(conn, MG_WEBSOCKET_OPCODE_TEXT,
                           "Missing token", strlen("Missing token"));
        return;
    }

    std::cout << token + "\n";

    std::lock_guard<std::mutex> lock(ws_mutex);
    token_to_client[token] = conn;
}

void ws_closed(const mg_connection* conn, void* user_data) {
    std::lock_guard<std::mutex> lock(ws_mutex);
    for (auto it = token_to_client.begin(); it != token_to_client.end(); ++it) {
        if (it->second == conn) {
            token_to_client.erase(it);
            break;
        }
    }
}

// void ws_closed(struct mg_connection* conn, void*) {
//     std::lock_guard<std::mutex> lock(ws_mutex);
//     ws_clients.erase(std::remove(ws_clients.begin(), ws_clients.end(), conn), ws_clients.end());
// }

int ws_receive(struct mg_connection* conn, int, char* msg, size_t len, void*) {
    std::string incoming(msg, len);
    std::cout << "[WS] Received from client: " << incoming << std::endl;
    // Optionally forward to engine
    std::string response = send_to_engine(incoming);

    // Broadcast result back to all WS clients
    std::lock_guard<std::mutex> lock(ws_mutex);
    std::string token = get_token_from_conn(conn);
    if (!token.empty()) {
        mg_websocket_write(conn, MG_WEBSOCKET_OPCODE_TEXT, incoming.c_str(), incoming.size());
    }

    return 1; // must return non-zero to indicate success
}

namespace arta {
    class temp : public MessageConsumer {
        private:
        public:
            IO io;
            Logger logger = Logger(ARTA_LOG_DEBUG);
            inline void messageCb(std::string message) {
                logger.debug(message);
                send_to_client("6737637657676357", "test message", "response");
            }
    };
}

int main() {
    // CivetWeb options: serve ./www as document root
    const char* options[] = {
        "document_root", "./www",
        "listening_ports", "8080",
        nullptr
    };

    arta::temp temp;
    temp.io.setLogger(&temp.logger);
    temp.io.fifoInit("/tmp/webuiServer.fifo");
    temp.io.setConsumer(&temp);

    mg_callbacks callbacks{};
    mg_context* ctx = mg_start(&callbacks, nullptr, options);

    // HTTP POST endpoint
    mg_set_request_handler(ctx, "/command", command_handler, nullptr);

    // WebSocket endpoint
    mg_set_websocket_handler(ctx, "/ws", ws_connect, ws_ready, ws_receive, ws_closed, nullptr);

    std::cout << "Server running at http://localhost:8080\n";

    // Keep server alive
    for (;;) std::this_thread::sleep_for(std::chrono::seconds(10));
}
