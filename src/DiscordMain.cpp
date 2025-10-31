/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * DiscordMain.cpp
 */

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <map>
#include <cstring>

#include "ext/json.hpp"
#include "include/Logger.h"
#include "include/Utils.h"
#include "include/IO.h"

// #include "include/discord_postman.h"
#include "include/DiscordBot.h"

namespace arta {
    typedef struct {
        std::string bot_token;
    } arta_discord_conf_t;

    static arta_discord_conf_t config;
    static DiscordBot bot;

    Logger logger = Logger(ARTA_LOG_DEBUG);

    void from_json(const nlohmann::json& j,
                    arta_discord_conf_t & c) {  // NOLINT(runtime/references)
        j.at("bot_token").get_to(c.bot_token);
    }

    int DiscordMain(int argc, char** argv) {
        logger.info("Starting Discord Bot\n");
        from_json(loadFile("config.json"), config);

        bot.loggerSet(&logger);
        std::string id = "myid";
        bot.init(config.bot_token, id);

        while (!bot.initComplete) {
            logger.debug("Waiting for bot init");
            usleep(5 * 1000 * 1000);
        }

        IO io;
        io.setLogger(&logger);
        io.fifoInit("/tmp/discord_client.fifo");
        io.setConsumer(&bot);

        while (1) {
            usleep(60 * 1000 * 1000);
        }
    }
}  // namespace arta

int main(int argc, char** argv) {
    return arta::DiscordMain(argc, argv);
}
