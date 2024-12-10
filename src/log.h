/**
 * @file log.h
 * @author gtalocchino
 * @brief 
 * @version 0.1
 * @date 2024-07-03
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef LOG_H
#define LOG_H


enum log_level {
    info,
    debug,
    alert,
    error
};


void logg(enum log_level level, const char *fmt, ...);

#endif
