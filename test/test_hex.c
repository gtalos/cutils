/*
 * Copyright (c) 2024
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "unity.h"
#include "hex.h"

#include <string.h>


/**
 * Convert valid hexadecimal string to binary.
 * 
 * Description:
 * - This test ensures that the hex_to_bytes function correctly 
 *   converts a valid hexadecimal string into its binary representation.
 * 
 * Test Steps:
 * - Provide a valid hexadecimal string.
 * - Call the hex_to_bytes function.
 * 
 * Expected result:
 * - The returned value is 0 (indicating success).
 * - The binary output matches the expected values.
 */
void test_hex_to_bytes_valid_8_bytes(void)
{
    char *hex = "0123456789abcdef";
    uint8_t bin[8] = {0};
    uint8_t expected[] = {
        0x01, 0x23, 0x45, 0x67, 
        0x89, 0xab, 0xcd, 0xef
    };

    TEST_ASSERT_EQUAL_INT32(0, hex_to_bytes(hex, strlen(hex), bin));
    TEST_ASSERT_EQUAL_MEMORY(expected, bin, strlen(hex) / 2);
}

/**
 * Convert shorter valid hexadecimal string to binary.
 * 
 * Description:
 * - This test verifies that the hex_to_bytes function works with 
 *   shorter hexadecimal strings.
 * 
 * Test Steps:
 * - Provide a valid hexadecimal string of smaller length.
 * - Call the hex_to_bytes function.
 * 
 * Expected result:
 * - The returned value is 0.
 * - The binary output matches the expected values.
 */
void test_hex_to_bytes_valid_5_bytes(void)
{
    char *hex = "a7b8c17025";
    uint8_t bin[8] = {0};
    uint8_t expected[] = {
        0xa7, 0xb8, 0xc1, 0x70, 0x25
    };

    TEST_ASSERT_EQUAL_INT32(0, hex_to_bytes(hex, strlen(hex), bin));
    TEST_ASSERT_EQUAL_MEMORY(expected, bin, strlen(hex) / 2);
}

/**
 * Convert minimal valid hexadecimal string to binary.
 * 
 * Description:
 * - This test verifies conversion of a single-byte hexadecimal string.
 * 
 * Test Steps:
 * - Provide a 2-character hexadecimal string.
 * - Call the hex_to_bytes function.
 * 
 * Expected result:
 * - The returned value is 0.
 * - The binary output matches the expected value.
 */
void test_hex_to_bytes_valid_1_byte(void)
{
    char *hex = "9f";
    uint8_t bin[8] = {0};
    uint8_t expected[] = {0x9f};

    TEST_ASSERT_EQUAL_INT32(0, hex_to_bytes(hex, strlen(hex), bin));
    TEST_ASSERT_EQUAL_MEMORY(expected, bin, strlen(hex) / 2);
}

/**
 * Handle invalid hexadecimal string length.
 * 
 * Description:
 * - This test ensures that the function correctly handles an invalid 
 *   hexadecimal string with an odd number of characters.
 * 
 * Test Steps:
 * - Provide an odd-length hexadecimal string.
 * - Call the hex_to_bytes function.
 * 
 * Expected result:
 * - The function returns -1 (indicating an error).
 * - The binary output remains unchanged.
 */
void test_hex_to_bytes_invalid_odd_length(void)
{
    char *hex = "123";
    uint8_t bin[8] = {0};
    uint8_t expected[8] = {0};

    TEST_ASSERT_EQUAL_INT32(-1, hex_to_bytes(hex, strlen(hex), bin));
    TEST_ASSERT_EQUAL_MEMORY(expected, bin, sizeof(bin));
}

/**
 * Handle zero-length input string.
 * 
 * Description:
 * - This test ensures that the function correctly handles a zero-length 
 *   hexadecimal string.
 * 
 * Test Steps:
 * - Provide an empty hexadecimal string.
 * - Call the hex_to_bytes function.
 * 
 * Expected result:
 * - The function returns 0.
 * - The binary output remains unchanged.
 */
void test_hex_to_bytes_empty_string(void)
{
    char *hex = "1234";
    uint8_t bin[] = {0xff, 0xff, 0xff, 0xff};
    uint8_t expected[] = {0xff, 0xff, 0xff, 0xff};

    TEST_ASSERT_EQUAL_INT32(0, hex_to_bytes(hex, 0, bin));
    TEST_ASSERT_EQUAL_MEMORY(expected, bin, sizeof(bin));
}

/**
 * Convert binary to valid hexadecimal string.
 * 
 * Description:
 * - This test ensures that the hex_from_bytes function correctly 
 *   converts binary data into a hexadecimal string.
 * 
 * Test Steps:
 * - Provide a binary array.
 * - Call the hex_from_bytes function.
 * 
 * Expected result:
 * - The output hexadecimal string matches the expected value.
 */
void test_bytes_to_hex_valid_8_bytes(void)
{
    uint8_t bin[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
    
    char hex[32];
    memset(hex, 0xff, sizeof(hex));
    char *expected = "0123456789abcdef";

    hex_from_bytes(bin, sizeof(bin), false, hex);
   
    TEST_ASSERT_EQUAL_MEMORY(expected, hex, strlen(expected) + 1);
}

/**
 * Handle zero-length binary input for hex conversion.
 * 
 * Description:
 * - This test ensures that the hex_from_bytes function correctly 
 *   handles a zero-length binary input.
 * 
 * Test Steps:
 * - Provide an empty binary array.
 * - Call the hex_from_bytes function.
 * 
 * Expected result:
 * - The output remains unchanged.
 */
void test_bytes_to_hex_empty_binary(void)
{
    uint8_t bin[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
    
    char hex[32] = {'a'};
    char expected[32] = {'a'};

    hex_from_bytes(bin, 0, false, hex);
   
    TEST_ASSERT_EQUAL_MEMORY(expected, hex, sizeof(hex));
}
