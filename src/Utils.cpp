/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * Utils.cpp
 */

#include "include/Utils.h"

namespace arta {
Utils::Utils(void) {
    srand(time(0));
}

id_t Utils::idGet(void) {
    return nextId++;
}

int Utils::roll(int max) {
    return rand() % (1 + max);
}

int Utils::roll(int min, int max) {
    return min + (rand() %(1 + max - min));
}
}  // namespace arta
