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

#include "include/DataTypes.h"
#include "include/Entity.h"
#include "include/Modifier.h"

namespace arta {
typedef int64_t pop_t;
/**
 * @class Area
 *
 * @brief Area class
 */
class Area : public Entity, public EffectConsumer {
   private:
    AreaType type;
    pop_t population;
    pop_t populationMax;
    int   populationBaseGrowth; // points per thousand

    void rollForNewEffect(void);
    void processActiveEffects(void);
    void populationUpdate(void);

   public:
    using Entity::Entity;
    void tikAdvance(void) override;

    void populationSet(pop_t pop);
    void populationMod(pop_t diff);
    pop_t populationGet(void);

    void typeSet(AreaType t);
    AreaType typeGet(void);

    void populationBaseGrowthSet(int b);
    int populationBaseGrowthGet(void);
};
}  // namespace arta

#endif  // INCLUDE_AREA_H_
