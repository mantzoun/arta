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

#include "include/Utils.h"
#include "include/Logger.h"
#include "include/TimeManager.h"
#include "include/Universe.h"

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
    Utils utils;

    Universe universe = Universe(&logger, &timeManager, &utils, utils.idGet(), "Euclid");

   public:
    void tik(void);
    void init(void);
};
}  // namespace arta

#endif  // INCLUDE_ENGINE_H_
