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
#include "include/MySocket.h"

// #include "ext/json.hpp"
// #include "include/lux_utils.h"
// #include "include/discord_postman.h"
// #include "include/discord_bot.h"
// #include "include/engine.h"

namespace arta {
// typedef struct {
//     std::string bot_token;
// } lux_conf_t;

// static lux_conf_t config;
// static DiscordBot bot;
// Engine engine;

// Logger logger = Logger(ARTA_LOG_DEBUG);
// TimeManager timeManager;

// void game_loop() {
//     logger.info("here");
//     bot.post_message("syslog", "here");
// }

// void from_json(const nlohmann::json& j, lux_conf_t & c) {  // NOLINT(runtime/references)
//     j.at("bot_token").get_to(c.bot_token);
// }

MySocket                mySocket;
Engine                  engine;

void messageSend(std::string message) {
    mySocket.sendMessage(message);
}

int main(int argc, char **argv) {
    // logger.info("Starting Discord Bot\n");
    // from_json(load_file("config.json"), config);

    //         engine.setLogger(&logger);
    //         engine.set_postman(&bot);
    // #ifdef ENGINE_INIT_FROM_DISCORD
    //         // prepare the engine to receive data from the bot
    //         engine.init();
    // #endif /* ENGINE_INIT_FROM_DISCORD */

    // bot.set_logger(&logger);
    // bot.set_engine(&engine);
    // std::string id = "myid";
    // bot.init(config.bot_token, id);

    // while (!bot.initComplete) {
    //     logger.debug("Waiting for bot init");
    //     usleep(5 * 1000 * 1000);
    // }

    // #ifndef ENGINE_INIT_FROM_DISCORD
    //     // inialize the engine and configure the discord channels
    //     engine.init();
    //     engine.create_channels();
    // #endif /* ENGINE_INIT_FROM_DISCORD */

    engine.init();
    engine.messageCbSet(messageSend);
    mySocket.init(0);

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
