/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * Main.cpp
 */

#include <stdio.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <map>

#include <chrono>

#include "include/Engine.h"
#include "include/Utils.h"
#include "include/MySocket.h"

#include "ext/json.hpp"

namespace arta {
typedef struct {
    std::string socketPath;
} arta_conf_t;

static arta_conf_t config;

void from_json(const nlohmann::json& j, arta_conf_t & c) {  // NOLINT(runtime/references)
    j.at("unixSocketPath").get_to(c.socketPath);
}

MySocket                mySocket;
Engine                  engine;

void messageSend(std::string message) {
    mySocket.sendMessage(message);
}

int main(int argc, char **argv) {
    engine.init();
    engine.messageCbSet(messageSend);

    from_json(loadFile("config.json"), config);

    mySocket.init(0, config.socketPath);

    auto now = std::chrono::system_clock::now();
    auto nextTrigger = time_point_cast<std::chrono::minutes>(now) + std::chrono::minutes(1);

    std::string message= "";

    while (true) {
        message = mySocket.popFromQueue();
        while (message != "") {
            engine.messageHandle(message);
            message = mySocket.popFromQueue();
        }

        auto now = std::chrono::system_clock::now();
        if (now >= nextTrigger) {
            engine.tik();
            nextTrigger += std::chrono::minutes(1);
        }

        usleep(50);
    }
}
}  // namespace arta

int main(int argc, char **argv) { return arta::main(argc, argv); }
