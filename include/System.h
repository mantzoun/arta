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
#include <list>

#include "include/Entity.h"
#include "include/EffectConsumer.h"
#include "include/Area.h"

namespace arta {
/**
 * @class System
 *
 * @brief System class
 */
class System : public Entity, public EffectConsumer {
   private:
    std::list<Area*> areas;

   public:
    using Entity::Entity;
    void init(void);
    void tikAdvance(void) override;

};
}  // namespace arta

#endif  // INCLUDE_SYSTEM_H_
