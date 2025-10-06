/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * Utils.h
 *
 * Collection of utilities
 */

#ifndef INCLUDE_UTILS_H_
#define INCLUDE_UTILS_H_

#include <string>
#include <cstdlib>
#include <ctime>

namespace arta {
typedef u_int64_t id_t;

/**
 * @class Utils
 *
 * @brief collection of utilities
 */
class Utils {
   private:
    id_t nextId = 0;

   public:
    Utils();

    /**
     * @brief provide a new unique_id
     */
    id_t idGet(void);

    int roll(int bound);
    int roll(int lower_bound, int upper_bound);
};
}  // namespace arta

#endif  // INCLUDE_UTILS_H_
