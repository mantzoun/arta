/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * Engine.h
 *
 * Class for the game engine.
 */

#ifndef INCLUDE_ENGINE_H_
#define INCLUDE_ENGINE_H_

#include <string>
#include <list>

#include "include/IdManager.h"
#include "include/Logger.h"
#include "include/TimeManager.h"
#include "include/System.h"

namespace arta {
/**
 * @class Engine
 *
 * @brief Engine class
 */
class Engine  {
   private:
    TimeManager timeManager;
    Logger logger = Logger(ARTA_LOG_DEBUG);
    IdManager idManager;

    std::list<System*> systems;

   public:
    void init(void);
    void tik(void);
};
}  // namespace arta

#endif  // INCLUDE_ENGINE_H_
