/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * Entity.cpp
 */

#include <format>

#include "include/Engine.h"

#define SAVEFILE "engine.json"

namespace arta {
void Engine::tik(void) {
    timeManager.timeAdvance(1);
    universe.tikAdvance();

    nlohmann::json json;
    to_json(json, *this);
    saveFile(SAVEFILE, json);
}

void Engine::init(void) {
    nlohmann::json json = loadFile(SAVEFILE);
    from_json(json, *this);

    universe.loggerSet(&logger);
    universe.utilsSet(&utils);
    universe.timeManagerSet(&timeManager);

    universe.init();
}

void Engine::messageCbSet(void (*cb)(std::string)) {
    messageCb = cb;
}

void Engine::messageUI(std::string message) {
    this->messageCb(message);
}

void Engine::messageHandle(std::string message) {
    auto tokens = utils.tokenize(message);

    std::string playerId = tokens[0];
    tokens.erase(tokens.begin());

    if (!characterMap.contains(playerId)) {
        messageUI(playerId + " " + "player character does not exist. Type \"start\" to create new Character");
        messageUI("broadcast broadcast test");
    }
}

}  // namespace arta
