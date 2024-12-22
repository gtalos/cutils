/*
 * Copyright (c) 2024 The cutils authors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "log.h"
#include "log_port.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define MAX_BUFFER_SIZE      (100)
#define LINE_BREAK           "\n"
#define MSG_BUFF_OVERFLOW    "[LOG]: Message exceeds buffer size" LINE_BREAK
#define MSG_UNEXPECTED_ERROR "[LOG]: Unexpected error" LINE_BREAK


static void prepend_and_print(const char *prepend, const char *fmt, va_list args);


void logg(enum log_level level, const char *fmt, ...)
{
    char *tag = NULL;

    /* Determine the tag based on the log level */
    switch (level) {
    case info:
        tag = "[INFO]: ";

        break;
    case debug:
        tag = "[DEBUG]: ";

        break;
    case alert:
        tag = "[ALERT]: ";

        break;
    case error:
    default:
        tag = "[ERROR]: ";

        break;
    }
        
    va_list args;
    va_start(args, fmt);
    prepend_and_print(tag, fmt, args);
    va_end(args);
}

static void prepend_and_print(const char *prepend, const char *fmt, va_list args)
{
    /* Calculate the lengths needed. */
    size_t prepend_len = strlen(prepend);
    
    va_list args_copy;
    va_copy(args_copy, args);

    int format_len = vsnprintf(NULL, 0, fmt, args_copy);

    va_end(args_copy);

    if (format_len < 0) {
        /* Handle error: Message unexpected error. */
        log_port_stream(MSG_UNEXPECTED_ERROR, strlen(MSG_UNEXPECTED_ERROR));
        return;
    }

    /* Allocate memory for the combined string. */
    size_t combined_len = prepend_len + format_len + strlen(LINE_BREAK) + 1;
    if (combined_len > MAX_BUFFER_SIZE) {
        /* Handle error: Message exceeds buffer size. */
        log_port_stream(MSG_BUFF_OVERFLOW, strlen(MSG_BUFF_OVERFLOW));
        return;
    }

    char combined[MAX_BUFFER_SIZE];

    /* Copy prepend string. */
    strcpy(combined, prepend);

    /* Append format string. */
    vsnprintf(combined + prepend_len, format_len + 1, fmt, args);

    /* Add line break sequence. */
    strcat(combined, LINE_BREAK);

    /* Stream the combined string. */
    log_port_stream(combined, strlen(combined));
}
