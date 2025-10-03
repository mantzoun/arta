/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * Engine.h
 *
 * Class for the game universe.
 */

#ifndef INCLUDE_UNIVERSE_H_
#define INCLUDE_UNIVERSE_H_

#include <string>
#include <list>

#include "include/Entity.h"
#include "include/System.h"

namespace arta {
/**
 * @class Universe
 *
 * @brief Universe class
 */
class Universe : public Entity {
   private:
      std::list<System*> systems;

   public:
      using Entity::Entity;
      void tikAdvance(void) override;
      void init(void);
};
}  // namespace arta

#endif  // INCLUDE_UNIVERSE_H_
