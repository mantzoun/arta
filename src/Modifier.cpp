/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * Modifiers.cpp
 */

#include "include/Modifier.h"

namespace arta {
ConsumerModifierEffect * planetaryWar(EffectConsumer& consumer);

ModifierEntry AreaModifiers[] = {
    { planetaryWar, 100 },
};

ModifierEntry SystemModifiers[] = {
};

bool Modifiers::isApplicable(EffectConsumer& consumer, ConsumerType validType, ConsumerModifierType type){
    if (consumer.consumerTypeGet() != validType) {
        return NULL;
    }

    if (consumer.modifierIsActive(type)) {
        return NULL;
    }
    return true;
}

void * Modifiers::rollNewModifier(EffectConsumer& consumer, Utils * utils) {
    auto iterateEffects = [&](auto& modifiers) -> arta::ConsumerModifierEffect* {
        for (auto& modifier : modifiers) {
            if (utils->roll(2000) < modifier.chanceToFire) {
                ConsumerModifierEffect * effect = modifier.cb(consumer);
                if (effect != NULL) {
                    return effect;
                }
            }
        }
        return nullptr;
    };

    if (consumer.consumerTypeGet() == CONSUMER_TYPE_SYSTEM) {
        return iterateEffects(SystemModifiers);
    }
    return iterateEffects(AreaModifiers);
}

ConsumerModifierEffect * planetaryWar(EffectConsumer& consumer) {
    if (! Modifiers::isApplicable(consumer, CONSUMER_TYPE_PLANET, CONSUMER_MODIFIER_TYPE_WAR)) {
        return NULL;
    }

    ConsumerModifierEffect * effect = new ConsumerModifierEffect();
    effect->populationYearlyIncfluence = -4;
    effect->title = "WAR!";
    effect->startingText = "Global conflict has begun";
    effect->endingText = "The war is over";
    effect->chanceToEnd = 50;
    effect->turnsActive = 0;
    effect->turnsMinimum = 5;
    effect->type = CONSUMER_MODIFIER_TYPE_WAR;

    return effect;
}
}
