/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * Entity.h
 *
 * Base class for most in-universe objects.
 */

#ifndef INCLUDE_ENTITY_H_
#define INCLUDE_ENTITY_H_

#include <string>

#include "include/Utils.h"
#include "include/Logger.h"
#include "include/TimeManager.h"

namespace arta {
/**
 * @class Entity
 *
 * @brief Base class
 */
class Entity {
   protected:
    id_t id;
    std::string name = "uninit";
    Logger* logger;
    TimeManager* timeManager;
    Utils* utils;

   public:
    /**
     * @brief Base entity constructor
     *
     * @param logger    Pointer to the logger object
     * @param timeMan   Pointer to the time manager object
     * @param utils     Pointer to the Id manager object
     */
    Entity(Logger* logger, TimeManager* timeMan, Utils* utils, id_t id, std::string name);

    virtual void tikAdvance(void) = 0;

    std::string nameGet(void);
    void nameSet(std::string name);

    id_t idGet(void);
    void idSet(id_t id);
};
}  // namespace arta

#endif  // INCLUDE_ENTITY_H_
