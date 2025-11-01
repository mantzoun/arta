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

#include "ext/json.hpp"
#include "include/Utils.h"
#include "include/Logger.h"
#include "include/TimeManager.h"
#include "include/Universe.h"
#include "include/IO.h"
#include "include/MessageConsumer.h"

namespace arta {
/**
 * @class Engine
 *
 * @brief Engine class
 */
class Engine : public MessageConsumer  {
   private:
    TimeManager timeManager;
    Logger logger = Logger(ARTA_LOG_DEBUG);
    Utils utils;
    IO    io;

    Universe universe = Universe(&logger, &timeManager, &utils, utils.idGet(), "Euclid");

   public:
    void tik(void);
    void init(void);

    void messageCb(std::string message) override;

    friend void to_json(nlohmann::json& j, \
                     Engine& e);  // NOLINT(runtime/references)
    friend void from_json(const nlohmann::json& j, \
                     Engine & e);  // NOLINT(runtime/references)
};

inline
void to_json(nlohmann::json& j, \
             Engine & e) {  // NOLINT(runtime/references)
    j = nlohmann::json{
      {"time", e.timeManager.timeGet()},
      {"universe", e.universe},
    };
}

inline
void from_json(const nlohmann::json& j,
                Engine & e) {  // NOLINT(runtime/references)
    tik_t time;
    j.at("time").get_to(time);
    j.at("universe").get_to(e.universe);

    e.timeManager.timeSet(time);
}
}  // namespace arta

#endif  // INCLUDE_ENGINE_H_
