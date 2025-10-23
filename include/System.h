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

#include "ext/json.hpp"
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
    std::list<Area> areas;

   public:
    using Entity::Entity;
    void init(void);
    void tikAdvance(void) override;

    friend void to_json(nlohmann::json& j, \
                     const System& s);  // NOLINT(runtime/references)
    friend void from_json(const nlohmann::json& j, \
                     System & s);  // NOLINT(runtime/references)
};

inline
void to_json(nlohmann::json& j, \
             const System & s) {  // NOLINT(runtime/references)
  j = nlohmann::json{
      {"id", s.id},
      {"name", s.name},
      {"areas", s.areas},
      {"cons_type", s.cons_type},
      {"activeModifiers", s.activeModifiers},
      {"activeModifierEffects", s.activeModifierEffects},
  };
}

inline
void from_json(const nlohmann::json& j,
                System & s) {  // NOLINT(runtime/references)
    j.at("id").get_to(s.id);
    j.at("name").get_to(s.name);
    j.at("areas").get_to(s.areas);
    j.at("cons_type").get_to(s.cons_type);
    j.at("activeModifiers").get_to(s.activeModifiers);
    j.at("activeModifierEffects").get_to(s.activeModifierEffects);
}
}  // namespace arta

#endif  // INCLUDE_SYSTEM_H_
