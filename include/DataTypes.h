/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * DataTypes.h
 */

#ifndef INCLUDE_DATATYPES_H_
#define INCLUDE_DATATYPES_H_

#include "ext/json.hpp"

namespace arta {
typedef int64_t pop_t;

enum AreaType {
    AREA_TYPE_PLANET,
    AREA_TYPE_STATION,
    AREA_TYPE_MAX
};

enum ConsumerModifierType{
    CONSUMER_MODIFIER_TYPE_SOLAR_ACTIVITY,
    CONSUMER_MODIFIER_TYPE_WAR,
    CONSUMER_MODIFIER_TYPE_MAX
};

enum ConsumerType {
    CONSUMER_TYPE_SYSTEM,
    CONSUMER_TYPE_PLANET,
    CONSUMER_TYPE_STATION,
    CONSUMER_TYPE_MAX
};

struct ConsumerModifierEffect {
    int turnsActive;
    int turnsMinimum;
    int chanceToEnd;    // chance for effect to finish each turn after the minimum turns have passed (percentage)
    std::string title;
    std::string startingText;
    std::string endingText;
    ConsumerModifierType type;
    // Area Specific
    int populationYearlyIncfluence; // effect on population growth +/- in one-thousandths per year
    // System specific
    ConsumerModifierType areaAdditionalEfect; //for system effects, also apply this effect to areas
};

inline void to_json(nlohmann::json& j, const ConsumerModifierEffect& e) {
    j = nlohmann::json{
    {"turnsActive", e.turnsActive},
    {"turnsMinimum", e.turnsMinimum},
    {"chanceToEnd", e.chanceToEnd},
    {"title", e.title},
    {"startingText", e.startingText},
    {"endingText", e.endingText},
    {"type", e.type},
    {"populationYearlyIncfluence", e.populationYearlyIncfluence},
    {"areaAdditionalEfect", e.areaAdditionalEfect},
  };
}

inline void from_json(const nlohmann::json& j, ConsumerModifierEffect& e) {
    j.at("turnsActive").get_to(e.turnsActive);
    j.at("turnsMinimum").get_to(e.turnsMinimum);
    j.at("chanceToEnd").get_to(e.chanceToEnd);
    j.at("title").get_to(e.title);
    j.at("startingText").get_to(e.startingText);
    j.at("endingText").get_to(e.endingText);
    j.at("type").get_to(e.type);
    j.at("populationYearlyIncfluence").get_to(e.populationYearlyIncfluence);
    j.at("areaAdditionalEfect").get_to(e.areaAdditionalEfect);
}

}  // namespace arta

#endif  // INCLUDE_DATATYPES_H_
