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

#include "include/Engine.h"

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

// void from_json(const nlohmann::json& j,
//                 lux_conf_t & c) {  // NOLINT(runtime/references)
//     j.at("bot_token").get_to(c.bot_token);
// }

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

    // while (!bot.init_complete) {
    //     logger.debug("Waiting for bot init");
    //     usleep(5 * 1000 * 1000);
    // }

    // #ifndef ENGINE_INIT_FROM_DISCORD
    //     // inialize the engine and configure the discord channels
    //     engine.init();
    //     engine.create_channels();
    // #endif /* ENGINE_INIT_FROM_DISCORD */

    Engine engine;
    engine.init();

    while (1) {
        // game_loop();
        // engine.execute_game_loop();
        usleep(2 * 1000 * 1000);
        engine.tik();
    }
}
}  // namespace arta

int main(int argc, char **argv) { return arta::main(argc, argv); }
