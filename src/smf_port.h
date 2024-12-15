/*
 * Copyright 2021 The Chromium OS Authors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef SMF_PORT_H
#define SMF_PORT_H

/**
 * @brief Logs a formatted message for the state machine framework (SMF).
 *
 * This function allows the state machine framework to log messages using
 * a platform-specific output, such as a console or a file.
 *
 * @param fmt A format string, similar to printf, specifying the log message content.
 * @param ... Arguments matching the format specifiers in `fmt`.
 */
void smf_port_log(const char *fmt, ...);

#endif /* SMF_PORT_H */
