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

#include "include/IdManager.h"
#include "include/Logger.h"
#include "include/TimeManager.h"

namespace arta {
/**
 * @class Entity
 *
 * @brief Base class
 */
class Entity {
   private:
    id_t id;
    std::string name = "uninit";
    Logger* logger;
    TimeManager* timeManager;
    IdManager* idManager;

   public:
    /**
     * @brief Base entity constructor
     *
     * @param logger    Pointer to the logger object
     * @param timeMan   Pointer to the time manager object
     * @param idMan     Pointer to the Id manager object
     */
    Entity(Logger* logger, TimeManager* timeMan, IdManager* idMan);
};
}  // namespace arta

#endif  // INCLUDE_ENTITY_H_
