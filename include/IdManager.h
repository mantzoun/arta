/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * IdManager.h
 *
 * Implement in-universe time keeping
 * and relative functions
 */

#ifndef INCLUDE_IDMANAGER_H_
#define INCLUDE_IDMANAGER_H_

#include <string>

namespace arta {
typedef u_int64_t id_t;

/**
 * @class IdManager
 *
 * @brief handle Id provisioning in the game
 */
class IdManager {
   private:
    id_t nextId = 0;

   public:
    /**
     * @brief provide a new unique_id
     */
    id_t idGet(void);
};
}  // namespace arta

#endif  // INCLUDE_IDMANAGER_H_
