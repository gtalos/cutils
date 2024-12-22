/*
 * Copyright (c) 2024 The cutils authors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef LOG_PORT_H
#define LOG_PORT_H

#include <stddef.h>

/**
 * @brief Streams a log message to the platform-specific output.
 *
 * This function is used internally by the logging library to send log messages
 * to their destination.
 *
 * @param data Pointer to the data buffer containing the message.
 * @param len Length of the data buffer in bytes.
 */
void log_port_stream(const void *data, size_t len);

#endif /* LOG_PORT_H */
