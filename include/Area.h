/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * Area.h
 *
 * Class for areas
 */

#ifndef INCLUDE_AREA_H_
#define INCLUDE_AREA_H_

#include <string>

#include "include/Entity.h"

namespace arta {
/**
 * @class Area
 *
 * @brief Area class
 */
class Area : public Entity {
   private:


   public:
    using Entity::Entity;
    void tikAdvance(void) override;

};
}  // namespace arta

#endif  // INCLUDE_AREA_H_