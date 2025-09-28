/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * Logger.h
 *
 * Implement all logger functionality in one object, shared
 * between threads. need to check thread safety
 */

#ifndef INCLUDE_LOGGER_H_
#define INCLUDE_LOGGER_H_

#include <string>

namespace arta {
/**
 * @brief Supported logger log severity levels
 */
enum log_lvl { ARTA_LOG_DEBUG, ARTA_LOG_INFO, ARTA_LOG_WARN, ARTA_LOG_ERROR };

/**
 * @class Logger
 *
 * @brief The logger object will handle logging for all
 *        application modules.
 */
class Logger {
   private:
    log_lvl level;

   public:
    /**
     * @brief Custom logger constructor
     *
     * @param lvl The log severity level
     */
    explicit Logger(log_lvl lvl);

    /**
     * @brief Set the log level
     *
     * @param lvl The log level
     */
    void set_level(log_lvl lvl);

    /**
     * @brief get the log level in string format
     *
     * @param lvl The log level
     */
    std::string log_lvl_to_str(log_lvl lvl);

    /**
     * @brief Print a message to stdout, if it exceeds the configured severity
     *
     * @param lvl The message severity
     * @param msg The log message
     */
    void log(log_lvl lvl, const std::string &msg);

    /**
     * @brief wrapper that calls the log() function with DEBUG severity
     *
     * @param msg The log message
     */
    void debug(const std::string &msg);

    /**
     * @brief wrapper that calls the log() function with INFO severity
     *
     * @param msg The log message
     */
    void info(const std::string &msg);

    /**
     * @brief wrapper that calls the log() function with WARN severity
     *
     * @param msg The log message
     */
    void warn(const std::string &msg);

    /**
     * @brief wrapper that calls the log() function with ERROR severity
     *
     * @param msg The log message
     */
    void error(const std::string &msg);
};
}  // namespace arta

#endif  // INCLUDE_LOGGER_H_
