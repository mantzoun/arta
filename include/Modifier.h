/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * Modifier.h
 *
 * List of all modifiers for systems and areas
 */


/////////////////////////////////////////////////////////////////// Add interface
#ifndef INCLUDE_MODIFIER_H_
#define INCLUDE_MODIFIER_H_

#include <string>

#include "include/Area.h"
#include "include/System.h"
#include "include/Utils.h"

namespace arta {
     enum SystemModifierType{
        SYSTEM_MODIFIER_SOLAR_ACTIVITY,
        SYSTEM_MODIFER_MAX
    };

    enum AreaModifierType {
        AREA_MODIFIER_WAR,
        AREA_MODIFER_MAX
    };

    struct SystemModifierEffect {
        int turnsActive;
        int turnsMinimum;
        int chanceToEnd;    // chance for effect to finish each turn after the minimum turns have passed
        std::string title;
        SystemModifierType type;
        AreaModifierType areaAdditionalEfect; //for system effects, also apply this effec to areas
    };

    struct AreaModifierEffect {
        int populationYearlyIncfluence; // effect on population growth +/- in one-thousandths per year
        int turnsActive;
        int turnsMinimum;
        int chanceToEnd;    // chance for effect to finish each turn after the minimum turns have passed (percentage)
        std::string title;
        AreaModifierType type;
    };

    struct AreaModifierEntry {
        AreaModifierEffect * (*cb)(Area *);
        int chanceToFire;
    };

    struct SystemModifierEntry {
        SystemModifierEffect * (*cb)(System *);
        int chanceToFire;
    };

    bool isApplicable(Area * area, AreaType validType, AreaModifierType type){
        if (area->typeGet() != validType) {
            return NULL;
        }

        if (area->modifierIsActive(type)) {
            return NULL;
        }
        return true;
    }

    AreaModifierEffect * planetaryWar(Area * area) {
        if (! isApplicable(area, AREA_TYPES_PLANET, AREA_MODIFIER_WAR)) {
            return NULL;
        }

        AreaModifierEffect * effect = new AreaModifierEffect();
        effect->populationYearlyIncfluence = -10;
        effect->title = "WAR!";
        effect->chanceToEnd = 50;
        effect->turnsActive = 0;
        effect->turnsMinimum = 5;
        effect->type = AREA_MODIFIER_WAR;

        return effect;
    }

    AreaModifierEntry AreaModifiers[] = {
        { planetaryWar, 1000 },
    };

    SystemModifierEntry SystemModifiers[] = {
    };

class Modifiers {
    private:

    public:
        static AreaModifierEffect * rollNewModifier(Area * area, Utils * utils) {
            for (auto& modifier : AreaModifiers) {
                    if (utils->roll(2000) < modifier.chanceToFire) {
                        AreaModifierEffect * effect = modifier.cb(area) {
                            if (effect != NULL) {
                                return effect;
                            }
                        }
                    }
            }

            return NULL;
        }

        static SystemModifierEffect * rollNewModifier(System * system);

};
}  // namespace arta

#endif  // INCLUDE_MODIFIER_H_