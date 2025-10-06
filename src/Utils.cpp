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

int Utils::roll(int bound) {
    return rand() % bound;
}

int Utils::roll(int lower_bound, int upper_bound) {
    return lower_bound + (rand() %(upper_bound - lower_bound));
}
}  // namespace arta
