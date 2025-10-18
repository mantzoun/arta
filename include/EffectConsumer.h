/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * EffectConsumer.h
 *
 * Interface Class
 */

#ifndef INCLUDE_EFFECTCONSUMER_H_
#define INCLUDE_EFFECTCONSUMER_H_

#include <unordered_set>

#include "include/DataTypes.h"

namespace arta {
/**
 * @class EffectConsumer
 *
 * @brief EffectConsumer class
 */
class EffectConsumer {
   protected:
      ConsumerType cons_type;
      std::unordered_set<ConsumerModifierType> activeModifiers;
      std::unordered_set<ConsumerModifierEffect*> activeModifierEffects;

   public:
      inline bool modifierIsActive(ConsumerModifierType type) {
         if (activeModifiers.contains(type)) {
            return true;
         }
         return false;
      }

      inline ConsumerType consumerTypeGet(void) {
         return cons_type;
      }

      inline void consumerTypeSet(ConsumerType type) {
         cons_type = type;
      }

};
}  // namespace arta

#endif  // INCLUDE_EFFECTCONSUMER_H_
