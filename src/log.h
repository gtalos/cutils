/*
 * Copyright (c) 2024 The cutils authors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef LOG_H
#define LOG_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @enum log_level
 * @brief Defines log severity levels.
 *
 * Each log level corresponds to a severity or purpose, ranging from informational
 * messages to error alerts.
 */
enum log_level {
    info,  /**< Informational messages */
    debug, /**< Debugging messages */
    alert, /**< Alert-level messages requiring attention */
    error  /**< Error messages */
};

/**
 * @brief Logs a formatted message at a specific log level.
 *
 * This function sends a log message with a given severity level and formatted string.
 *
 * @param level The severity level of the log message (see @ref log_level).
 * @param fmt A format string, similar to printf, specifying the log message content.
 * @param ... Arguments matching the format specifiers in fmt.
 */
void logg(enum log_level level, const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* LOG_H */
