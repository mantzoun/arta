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
#include <fstream>

#include "ext/json.hpp"

namespace arta {
typedef u_int64_t id_t;

inline
void saveFile(const std::string & filename, const nlohmann::json & j) {
std::ofstream file(filename);
if (!file) {
    throw std::runtime_error("Failed to open file for writing: " + filename);
}
file << j.dump(4);
}

inline
nlohmann::json loadFile(const std::string & filename) {
std::ifstream file(filename);
if (!file) {
    throw std::runtime_error("Failed to open file for reading: " + filename);
}
nlohmann::json j;
file >> j;
return j;
}

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
    std::vector<std::string> tokenize(const std::string &string_with_spaces);

    int sendMessage(std::string message);
};
}  // namespace arta

#endif  // INCLUDE_UTILS_H_
