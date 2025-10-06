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
#include <unordered_set>

#include "include/Entity.h"
#include "include/Modifier.h"

namespace arta {
typedef int64_t pop_t;

enum AreaType {
    AREA_TYPES_PLANET,
    AREA_TYPES_STATION,
    AREA_TYPES_MAX
};

/**
 * @class Area
 *
 * @brief Area class
 */
class Area : public Entity {
   private:
    AreaType type;
    pop_t population;
    std::unordered_set<AreaModifierType> activeModifiers;
    std::unordered_set<AreaModifierEffect*> activeModifierEffects;

   public:
    using Entity::Entity;
    void tikAdvance(void) override;

    void populationSet(pop_t pop);
    void populationMod(pop_t diff);
    pop_t populationGet(void);

    void typeSet(AreaType t);
    AreaType typeGet(void);

    bool modifierIsActive(AreaModifierType type);

};
}  // namespace arta

#endif  // INCLUDE_AREA_H_