/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * Entity.cpp
 */

#include "include/Entity.h"

namespace arta {
Entity::Entity(Logger* logger, TimeManager* timeMan, IdManager* idMan, id_t id, std::string name) {
    this->logger = logger;
    this->timeManager = timeMan;
    this->idManager = idMan;

    this->id = id;
    this->name = name;
}

void Entity::tikAdvance(void) {
    // Do nothing in Base class
}

}  // namespace arta
