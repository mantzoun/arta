/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * Universe.cpp
 */

#include "include/Universe.h"

namespace arta {
void Universe::init(void) {
    for (std::list<System>::iterator it = systems.begin(); it != systems.end(); ++it){
        (*it).loggerSet(logger);
        (*it).utilsSet(utils);
        (*it).timeManagerSet(timeManager);

        (*it).init();
    }
}

void Universe::tikAdvance(void) {
    logger->debug("Universe " + name + " advancing");
    utils->sendMessage("syslog;;Universe " + name + " advancing, turn " + std::to_string(timeManager->timeGet()));

    for (std::list<System>::iterator it = systems.begin(); it != systems.end(); ++it){
        (*it).tikAdvance();
    }

    // nlohmann::json json;
    // to_json(json, *this);
    // saveFile("uni.json", json);
}
}
