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

    io.setLogger(&logger);
    io.fifoInit("/tmp/gameServer.fifo");
    io.setConsumer(this);

    universe.init();
}

void Engine::messageCb(std::string message) {
    logger.debug(message);
}

}  // namespace arta
