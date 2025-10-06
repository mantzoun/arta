/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * Modifiers.cpp
 */

#include "include/Modifier.h"

namespace arta {
AreaModifierEffect * Modifiers::rollNewModifier(Area * area, Utils * utils) {
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
}