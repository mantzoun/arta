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

#include "include/Logger.h"
#include "include/MessageConsumer.h"
#include "include/MySocket.h"

std::unordered_map<std::string, mg_connection*> wsClientMap;
std::mutex wsMapMutex;
arta::MySocket mySocket;

std::string getTokenFromConn(mg_connection* conn) {
    const char* query = mg_get_request_info(conn)->query_string;
    if (!query) return "";
    std::string qs(query);
    auto pos = qs.find("token=");
    if (pos == std::string::npos) return "";
    return qs.substr(pos + 6, 16); // take 16 hex chars
}

void writeToWebSocket(mg_connection * conn, const std::string& message, const std::string& panel) {
    Json::Value msgJson;
    msgJson["panel"] = panel;
    msgJson["text"] = message;
    Json::StreamWriterBuilder writer;
    std::string msgStr = Json::writeString(writer, msgJson);

    mg_websocket_write(conn, MG_WEBSOCKET_OPCODE_TEXT, msgStr.c_str(), msgStr.size());
}

void broadcast(const std::string& msg, const std::string& panel) {
    std::lock_guard<std::mutex> lock(wsMapMutex);
    for (const auto & [ key, value ] : wsClientMap) {
        writeToWebSocket(value, msg, panel);
    }
}

void sendToClient(const std::string& token, const std::string& msg, const std::string& panel) {
    std::lock_guard<std::mutex> lock(wsMapMutex);
    auto it = wsClientMap.find(token);
    if (it != wsClientMap.end()) {
        writeToWebSocket(it->second, msg, panel);
    }
}

//TODO Sanitize, incorrect IDs, empty messages etc.
std::string sendToEngine(const std::string& cmd) {
    if (! mySocket.sendMessage(cmd)) {
        return "Error forwarding to engine: " + cmd;
    }
    return "";
}

int commandHandler(struct mg_connection* conn, void*) {
    char buf[1024];
    int len = mg_read(conn, buf, sizeof(buf) - 1);
    if (len < 0) len = 0;
    buf[len] = 0;
    std::string cmd(buf);

    // Sanitize input (ASCII printable only)
    cmd.erase(std::remove_if(cmd.begin(), cmd.end(), [](char c){ return c < 0x20 || c > 0x7E; }), cmd.end());

    std::string response = sendToEngine(cmd);

    // Return direct response to the HTTP client
    mg_printf(conn,
        "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n"
        "Access-Control-Allow-Origin: *\r\n\r\n%s", response.c_str());
    return 200;
}

// --- WebSocket handlers ---
int wsConnect(const struct mg_connection*, void*) { return 0; }

void wsRready(struct mg_connection* conn, void* userData) {
    std::string token = getTokenFromConn(conn);
    if (token.empty()) {
        mg_websocket_write(conn, MG_WEBSOCKET_OPCODE_TEXT,
                           "Missing token", strlen("Missing token"));
        return;
    }

    std::cout << token + "\n";

    std::lock_guard<std::mutex> lock(wsMapMutex);
    wsClientMap[token] = conn;
}

void wsClosed(const mg_connection* conn, void* userData) {
    std::lock_guard<std::mutex> lock(wsMapMutex);
    for (auto it = wsClientMap.begin(); it != wsClientMap.end(); ++it) {
        if (it->second == conn) {
            wsClientMap.erase(it);
            break;
        }
    }
}

int wsReceive(struct mg_connection* conn, int, char* msg, size_t len, void*) {
    std::string incoming(msg, len);
    std::cout << "[WS] Received from client: " << incoming << std::endl;

    std::string response = sendToEngine(incoming);

    // Broadcast result back to all WS clients
    std::lock_guard<std::mutex> lock(wsMapMutex);
    std::string token = getTokenFromConn(conn);
    if (!token.empty()) {
        mg_websocket_write(conn, MG_WEBSOCKET_OPCODE_TEXT, incoming.c_str(), incoming.size());
    }

    return 1; // must return non-zero to indicate success
}

void callback(std::string message) {
    auto pos = message.find(' ');
    std::string playerId = message.substr(0, pos);
    std::string response  = message.substr(pos + 1);

    if (playerId == "broadcast") {
        broadcast(response, "updates");
    } else {
        sendToClient(playerId, response, "response");
    }
}

int main() {
    // CivetWeb options: serve ./www as document root
    const char* options[] = {
        "document_root", "./www",
        "listening_ports", "8080",
        nullptr
    };

    mySocket.init(1);

    mg_callbacks callbacks{};
    mg_context* ctx = mg_start(&callbacks, nullptr, options);

    // HTTP POST endpoint
    mg_set_request_handler(ctx, "/command", commandHandler, nullptr);

    // WebSocket endpoint
    mg_set_websocket_handler(ctx, "/ws", wsConnect, wsRready, wsReceive, wsClosed, nullptr);

    std::cout << "Server running at http://localhost:8080\n";

    std::string message= "";
    while (true) {
        message = mySocket.popFromQueue();
        while (message != "") {
            callback(message);
            message = mySocket.popFromQueue();
        }
        usleep(50);
    }
}
