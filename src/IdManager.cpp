/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * IdManager.cpp
 */

#include "include/IdManager.h"

namespace arta {
id_t IdManager::idGet(void) { return nextId++; }
}  // namespace arta
