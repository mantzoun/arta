/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * Area.cpp
 */

#include "include/Area.h"

namespace arta {
void Area::tikAdvance(void) {
    logger->debug("Area " + name + " type " + std::to_string(type) + " population " + std::to_string(population));

    rollForNewEffect();

    processActiveEffects();

    populationUpdate();
}

void Area::init(void) {}

void Area::populationUpdate(void) {
    pop_t popMod =  (populationBaseGrowth * population) / (365 * 1000);
    logger->info(name + " - population organic growth " + std::to_string(popMod));
    populationMod(popMod);
}

void Area::rollForNewEffect(void) {
    ConsumerModifierEffect * effect = (ConsumerModifierEffect *) Modifiers::rollNewModifier(*this, utils);

    if (effect != NULL) {
        logger->info(name + " - " + effect->startingText);

        activeModifiers.insert(effect->type);
        ConsumerModifierEffect effectLocalCopy = *effect;
        activeModifierEffects.push_back(effectLocalCopy);
        delete effect;
    }
}

void Area::processActiveEffects(void) {
    for (auto it = activeModifierEffects.begin(); it != activeModifierEffects.end(); ) {
    auto& effect = *it;

    if (effect.turnsActive >= effect.turnsMinimum) {
        if (utils->roll(1, 100) < effect.chanceToEnd) {
            activeModifiers.erase(effect.type);
            logger->info(name + " - " + effect.endingText);
            it = activeModifierEffects.erase(it);
            continue;
        }
    }

    // Still Active
    effect.turnsActive++;

    pop_t popEffect = (effect.populationYearlyIncfluence * population) / (365 * 1000);
    logger->info(name + " - population affected by " + std::to_string(popEffect) + " by " + effect.title);
    populationMod(popEffect);

    ++it;  // only increment if not erased
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

void Area::populationBaseGrowthSet(int b) {
    populationBaseGrowth = b;
}

int Area::populationBaseGrowthGet(void) {
    return populationBaseGrowth;
}
}
