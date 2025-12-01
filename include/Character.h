/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * Player.h
 *
 * Class for player character data
 */

#ifndef INCLUDE_CHARACTER_H_
#define INCLUDE_CHARACTER_H_

#include <string>
#include <list>

#include "ext/json.hpp"
#include "include/Entity.h"
#include "include/EffectConsumer.h"
#include "include/Area.h"

namespace arta {
/**
 * @class Character
 *
 * @brief Player character class
 */
class Character {
    private:
        std::string id;  // 64bit hex string for now
    public:
};

}

#endif  /* INCLUDE_CHARACTER_H_ */