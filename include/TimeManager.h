/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * TimeManager.h
 *
 * Implement in-universe time keeping
 * and relative functions
 */

#ifndef INCLUDE_TIMEMANAGER_H_
#define INCLUDE_TIMEMANAGER_H_

#include <string>

namespace arta {

typedef u_int64_t tik_t;

/**
 * @class TimeManager
 *
 * @brief The time manager object will keep in game time
 *        and provide time-related functions
 */
class TimeManager {
   private:
    tik_t gameTime = 0;

   public:
    /**
     * @brief Advance the game time by a number of tiks
     *
     * @param tiks    How much to advance the game time
     */
    void timeAdvance(tik_t tiks);

    /**
     * @brief Set the game time to this tik
     *
     * @param time    Set the game time to this value
     */
    void timeSet(tik_t time);

    /**
     * @brief Set the game time to this tik
     *
     * @param time    Set the game time to this value
     */
    tik_t timeGet(void);

    /**
     * @brief Convert the current game time to a string
     */
    std::string nowToString(void);

    /**
     * @brief Convert an arbitrary game time to a string
     *
     * @param time    The game time to be converted to a string
     */
    std::string tiksToString(tik_t);
};
}  // namespace arta

#endif  // INCLUDE_TIMEMANAGER_H_
