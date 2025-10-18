/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * DataTypes.h
 */

#ifndef INCLUDE_DATATYPES_H_
#define INCLUDE_DATATYPES_H_

namespace arta {
enum AreaType {
    AREA_TYPES_PLANET,
    AREA_TYPES_STATION,
    AREA_TYPES_MAX
};

enum SystemModifierType{
    SYSTEM_MODIFIER_SOLAR_ACTIVITY,
    SYSTEM_MODIFER_MAX
};

enum AreaModifierType {
    AREA_MODIFIER_WAR,
    AREA_MODIFER_MAX
};


}  // namespace arta

#endif  // INCLUDE_DATATYPES_H_
