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

    std::string Entity::nameGet(void) {
        return name;
    }

    void Entity::nameSet(std::string name) {
        this->name = name;
    }

    id_t Entity::idGet(void) {
        return id;
    }

    void Entity::idSet(id_t id) {
        this->id = id;
    }

}  // namespace arta
