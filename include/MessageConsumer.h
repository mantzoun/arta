/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * MessageConsumer.h
 *
 * Interface Class
 */

#ifndef INCLUDE_MESSAGECONSUMER_H_
#define INCLUDE_MESSAGECONSUMER_H_

#include <unordered_set>
#include <list>

#include "include/DataTypes.h"

namespace arta {
/**
 * @class MessageConsumer
 *
 * @brief MessageConsumer class
 */
class MessageConsumer {
   protected:

   public:
      /**
       * @brief Interface to accept messages from other modules
       *
       * @param message the message information
       */
      virtual void messageCb(std::string message) = 0;
    };
}  // namespace arta

#endif  // INCLUDE_MESSAGECONSUMER_H_
