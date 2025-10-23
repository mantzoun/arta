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

#include "ext/json.hpp"
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
      std::list<System> systems;

   public:
      using Entity::Entity;
      void tikAdvance(void) override;
      void init(void);

   friend void to_json(nlohmann::json& j, \
                     const Universe& u);  // NOLINT(runtime/references)
   friend void from_json(const nlohmann::json& j, \
                     Universe & u);  // NOLINT(runtime/references)
};

inline
void to_json(nlohmann::json& j, \
             const Universe & u) {  // NOLINT(runtime/references)
  j = nlohmann::json{
      {"id", u.id},
      {"name", u.name},
      {"systems", u.systems},
  };
}

inline
void from_json(const nlohmann::json& j,
                Universe & u) {  // NOLINT(runtime/references)
   j.at("id").get_to(u.id);
   j.at("name").get_to(u.name);
   j.at("systems").get_to(u.systems);
}
}  // namespace arta

#endif  // INCLUDE_UNIVERSE_H_
