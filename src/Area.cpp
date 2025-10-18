/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * Area.cpp
 */

#include "include/Area.h"

namespace arta {
void Area::tikAdvance(void) {
    logger->debug("Area " + name + " type " + std::to_string(type) + " population " + std::to_string(population));

    ConsumerModifierEffect * effect = (ConsumerModifierEffect *) Modifiers::rollNewModifier(this, utils);

    if (effect != NULL) {
        logger->info(name + " New modifier " + effect->title);

        activeModifiers.insert(effect->type);
        activeModifierEffects.insert(effect);
    }
}

void Area::populationSet(pop_t pop) {
    population = pop;
}

void Area::populationMod(pop_t diff) {
    population += diff;
}

pop_t Area::populationGet(void) {
    return population;
}

void Area::typeSet(AreaType t) {
    type = t;
}

AreaType Area::typeGet(void) {
    return type;
}
}