/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * DataTypes.h
 */

#ifndef INCLUDE_DATATYPES_H_
#define INCLUDE_DATATYPES_H_

namespace arta {
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
    ConsumerModifierType type;
    // Area Specific
    int populationYearlyIncfluence; // effect on population growth +/- in one-thousandths per year
    // System specific
    ConsumerModifierType areaAdditionalEfect; //for system effects, also apply this effect to areas
};

}  // namespace arta

#endif  // INCLUDE_DATATYPES_H_
