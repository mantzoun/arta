/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * Entity.cpp
 */

#include <format>

#include "include/Engine.h"

namespace arta {
void Engine::tik(void) {
    universe.tikAdvance();
}

void Engine::init(void) {
    universe.init();
}

}  // namespace arta
