/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * Entity.h
 *
 * Class for star systems
 */

#ifndef INCLUDE_SYSTEM_H_
#define INCLUDE_SYSTEM_H_

#include <string>

#include "include/Entity.h"

namespace arta {
/**
 * @class System
 *
 * @brief System class
 */
class System : public Entity {
   private:


   public:
    using Entity::Entity;

};
}  // namespace arta

#endif  // INCLUDE_SYSTEM_H_
