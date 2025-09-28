/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * Entity.cpp
 */

#include "include/Entity.h"

namespace arta {
Entity::Entity(Logger* logger, TimeManager* timeMan, IdManager* idMan) {
    this->logger = logger;
    this->timeManager = timeMan;
    this->idManager = idMan;

    this->id = idManager->idGet();
}

}  // namespace arta
