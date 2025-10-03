/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * Entity.cpp
 */

#include <format>

#include "include/Engine.h"

namespace arta {
void Engine::init(void) {
    for (int i = 0; i < 5; i++) {
        System * s = new System(&logger, &timeManager, &idManager, idManager.idGet(), std::format("input : {}", i));

        systems.push_back(s);
    }
}

void Engine::tik(void) {
}

}  // namespace arta
