/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * Modifier.h
 *
 * List of all modifiers for systems and areas
 */

#ifndef INCLUDE_MODIFIER_H_
#define INCLUDE_MODIFIER_H_

#include <string>

#include "include/DataTypes.h"
#include "include/EffectConsumer.h"
#include "include/Utils.h"

namespace arta {
    struct ModifierEntry {
        ConsumerModifierEffect * (*cb)(EffectConsumer *);
        int chanceToFire;
    };

class Modifiers {
    private:

    public:
        static bool isApplicable(EffectConsumer * consumer, ConsumerType validType, ConsumerModifierType type);
        static void * rollNewModifier(EffectConsumer * consumer, Utils * utils);
};
}  // namespace arta

#endif  // INCLUDE_MODIFIER_H_
