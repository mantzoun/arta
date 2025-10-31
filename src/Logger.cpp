/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * Logger.cpp
 *
 * implementation of Logger class methods
 */

#include <cstdio>
#include <ctime>
#include <iostream>
#include <mutex>

#include "include/Logger.h"

static std::mutex mutex;

namespace arta {
Logger::Logger(log_lvl lvl) { this->set_level(lvl); }

void Logger::set_level(log_lvl lvl) { this->level = lvl; }

std::string Logger::log_lvl_to_str(log_lvl lvl) {
    std::string log_lvl_str[arta::ARTA_LOG_ERROR + 1] = {"DEBUG  ", "INFO   ", "WARNING", "ERROR  "};

    return log_lvl_str[lvl];
}

void Logger::log(log_lvl lvl, const std::string& msg) {
    if (lvl >= this->level) {
        std::scoped_lock lock(mutex);

        time_t now = time(0);
        tm ltm;
        localtime_r(&now, &ltm);
        char res[40];
        snprintf(res, sizeof(res), "%04d-%02d-%02d %02d:%02d:%02d %s", 1900 + ltm.tm_year, 1 + ltm.tm_mon, ltm.tm_mday,
                 ltm.tm_hour, ltm.tm_min, ltm.tm_sec, log_lvl_to_str(lvl).c_str());
        std::string tstamp = res;
        std::cout << tstamp + " " + msg + "\n";
        std::cout.flush();
    }
}

void Logger::debug(const std::string& msg) { this->log(ARTA_LOG_DEBUG, msg); }

void Logger::info(const std::string& msg) { this->log(ARTA_LOG_INFO, msg); }

void Logger::warn(const std::string& msg) { this->log(ARTA_LOG_WARN, msg); }

void Logger::error(const std::string& msg) { this->log(ARTA_LOG_ERROR, msg); }
}  // namespace arta
