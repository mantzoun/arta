/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * TimeManager.cpp
 */

#include "include/TimeManager.h"

namespace arta {
void TimeManager::timeAdvance(tik_t tiks) { gameTime += tiks; }

void TimeManager::timeSet(tik_t time) { gameTime = time; }

tik_t TimeManager::timeGet(void) { return gameTime; }

std::string TimeManager::nowToString(void) {
    // TODO(me)
    return "";
}

std::string TimeManager::tiksToString(tik_t) {
    // TODO(me)
    return "";
}
}  // namespace arta
