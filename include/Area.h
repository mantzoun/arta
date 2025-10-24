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

#include "ext/json.hpp"
#include "include/DataTypes.h"
#include "include/Entity.h"
#include "include/Modifier.h"

namespace arta {
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

    void init();

    void populationSet(pop_t pop);
    void populationMod(pop_t diff);
    pop_t populationGet(void);

    void typeSet(AreaType t);
    AreaType typeGet(void);

    void populationBaseGrowthSet(int b);
    int populationBaseGrowthGet(void);

    friend void to_json(nlohmann::json& j, \
                     const Area& a);  // NOLINT(runtime/references)
    friend void from_json(const nlohmann::json& j, \
                     Area & a);  // NOLINT(runtime/references)
};

inline
void to_json(nlohmann::json& j, \
             const Area & a) {  // NOLINT(runtime/references)
  j = nlohmann::json{
        {"id", a.id},
        {"name", a.name},
        {"cons_type", a.cons_type},
        {"activeModifiers", a.activeModifiers},
        {"activeModifierEffects", a.activeModifierEffects},
        {"type", a.type},
        {"population", a.population},
        {"populationMax", a.populationMax},
        {"populationBaseGrowth", a.populationBaseGrowth},
  };
}

inline
void from_json(const nlohmann::json& j,
                Area & a) {  // NOLINT(runtime/references)
    j.at("id").get_to(a.id);
    j.at("name").get_to(a.name);
    j.at("cons_type").get_to(a.cons_type);
    j.at("activeModifiers").get_to(a.activeModifiers);
    j.at("activeModifierEffects").get_to(a.activeModifierEffects);
    j.at("type").get_to(a.type);
    j.at("population").get_to(a.population);
    j.at("populationMax").get_to(a.populationMax);
    j.at("populationBaseGrowth").get_to(a.populationBaseGrowth);
}
}  // namespace arta

#endif  // INCLUDE_AREA_H_
