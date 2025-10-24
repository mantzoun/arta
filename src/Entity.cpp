/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * Entity.cpp
 */

#include "include/Entity.h"

namespace arta {
Entity::Entity(Logger* logger, TimeManager* timeMan, Utils* utils, id_t id, std::string name) {
    this->logger = logger;
    this->timeManager = timeMan;
    this->utils = utils;

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

    void Entity::loggerSet(Logger* l) {
        logger = l;
    }

    void Entity::utilsSet(Utils* u) {
        utils = u;
    }

    void Entity::timeManagerSet(TimeManager* t) {
        timeManager = t;
    }
}  // namespace arta
