/*
 * Copyright (c) 2024 The cutils authors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "unity.h"
#include "log.h"
#include "mock_log_port.h"

#include <string.h>


/**
 * Log an informational message
 * 
 * Description:
 * - This test ensures that the logg function correctly formats and 
 *   sends an informational message.
 * 
 * Steps:
 * - Call log with info log_level and a message.
 *  
 * Expected result:
 * - Verify the message content.
 * - Verify the message length.
 */
void test_log_info_message(void) 
{
    char *expected_message = "[INFO]: This is an info message\n";
    
    logg(info, "This is an info message");

    TEST_ASSERT_EQUAL(1, log_port_stream_fake.call_count);
    TEST_ASSERT_EQUAL_STRING(expected_message, log_port_stream_fake.arg0_val);
    TEST_ASSERT_EQUAL(strlen(expected_message), log_port_stream_fake.arg1_val);
}

/**
 * Log a debug message
 * 
 * Description:
 * - This test ensures that the logg function correctly formats and 
 *   sends an debug message.
 * 
 * Steps:
 * - Call log with debug log_level and a message.
 *  
 * Expected result:
 * - Verify the message content.
 * - Verify the message length.
 */
void test_log_debug_message(void) 
{
    char *expected_message = "[DEBUG]: This is a debug message\n";
    
    logg(debug, "This is a debug message");

    TEST_ASSERT_EQUAL(1, log_port_stream_fake.call_count);
    TEST_ASSERT_EQUAL_STRING(expected_message, log_port_stream_fake.arg0_val);
    TEST_ASSERT_EQUAL(strlen(expected_message), log_port_stream_fake.arg1_val);
}

/**
 * Log an alert message
 * 
 * Description:
 * - This test ensures that the logg function correctly formats and 
 *   sends an alert message.
 * 
 * Steps:
 * - Call log with alert log_level and a message.
 *  
 * Expected result:
 * - Verify the message content.
 * - Verify the message length.
 */
void test_log_alert_message(void) 
{
    char *expected_message = "[ALERT]: This is an alert message\n";
    
    logg(alert, "This is an alert message");

    TEST_ASSERT_EQUAL(1, log_port_stream_fake.call_count);
    TEST_ASSERT_EQUAL_STRING(expected_message, log_port_stream_fake.arg0_val);
    TEST_ASSERT_EQUAL(strlen(expected_message), log_port_stream_fake.arg1_val);
}

/**
 * Log an error message
 * 
 * Description:
 * - This test ensures that the logg function correctly formats and 
 *   sends an error message.
 * 
 * Steps:
 * - Call log with error log_level and a message.
 *  
 * Expected result:
 * - Verify the message content.
 * - Verify the message length.
 */
void test_log_error_message(void) 
{
    char *expected_message = "[ERROR]: This is an error message\n";
    
    logg(error, "This is an error message");

    TEST_ASSERT_EQUAL(1, log_port_stream_fake.call_count);
    TEST_ASSERT_EQUAL_STRING(expected_message, log_port_stream_fake.arg0_val);
    TEST_ASSERT_EQUAL(strlen(expected_message), log_port_stream_fake.arg1_val);
}

/**
 * Empty format string
 * 
 * Description:
 * - This test ensures that the logg function correctly formats and 
 *   sends an empty message.
 * 
 * Steps:
 * - Call log with empty format string.
 *  
 * Expected result:
 * - Verify the message content.
 * - Verify the message length.
 */
void test_log_empty_format_string(void) {
    char *expected_message = "[INFO]: \n";

    logg(info, "");

    TEST_ASSERT_EQUAL(1, log_port_stream_fake.call_count);
    TEST_ASSERT_EQUAL_STRING(expected_message, log_port_stream_fake.arg0_val);
    TEST_ASSERT_EQUAL(strlen(expected_message), log_port_stream_fake.arg1_val);
}

/**
 * Message with argument
 * 
 * Description:
 * - This test ensures that the logg function correctly formats and 
 *   sends an message with arguments.
 * 
 * Steps:
 * - Call log with empty format string.
 *  
 * Expected result:
 * - Verify the message content.
 * - Verify the message length.
 */
void test_log_message_with_argument(void) 
{
    const char *expected_message = "[DEBUG]: Debug value: 42, -1, 3.1415\n";

    logg(debug, "Debug value: %d, %d, %.4f", 42, -1, 3.1415f);

    TEST_ASSERT_EQUAL(1, log_port_stream_fake.call_count);
    TEST_ASSERT_EQUAL_STRING(expected_message, log_port_stream_fake.arg0_val);
    TEST_ASSERT_EQUAL(strlen(expected_message), log_port_stream_fake.arg1_val);
}

/**
 * Handle buffer overflow
 * 
 * Description:
 * - This test checks that a message exceeding MAX_BUFFER_SIZE 
 *   is handled correctly.
 * 
 * Steps:
 * - Call log with a message length bigger than MAX_BUFFER_SIZE.
 *  
 * Expected result:
 * - Verify the buffer overflow message content.
 * - Verify the buffer overflow message length.
 */
void test_log_buffer_overflow(void)
{
    char *expected_message = "[LOG]: Message exceeds buffer size\n";
    char long_message[92];
    memset(long_message, 'A', sizeof(long_message) - 1);
    long_message[sizeof(long_message) - 1] = '\0';

    logg(info, "%s", long_message);
    
    TEST_ASSERT_EQUAL(1, log_port_stream_fake.call_count);
    TEST_ASSERT_EQUAL_STRING(expected_message, log_port_stream_fake.arg0_val);
    TEST_ASSERT_EQUAL(strlen(expected_message), log_port_stream_fake.arg1_val);
}

/**
 * Handle unexpected formatting error
 * 
 * Description:
 * - This test simulates a formatting error (e.g., vsnprintf returning negative) 
 *   and ensures that the proper error message is logged.
 * 
 * Steps:
 * - Call log with a formatting error.
 *  
 * Expected result:
 * - Verify the unexpected error message content.
 * - Verify the unexpected error message length.
 */
void test_log_formatting_error(void)
{
    char *expected_message = "[LOG]: Unexpected error\n";

    logg(alert, NULL);

    TEST_ASSERT_EQUAL(1, log_port_stream_fake.call_count);
    TEST_ASSERT_EQUAL_STRING(expected_message, log_port_stream_fake.arg0_val);
    TEST_ASSERT_EQUAL(strlen(expected_message), log_port_stream_fake.arg1_val);
}
