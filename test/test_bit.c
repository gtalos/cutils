#include "unity.h"
#include "bit.h"


/**
 * Validate simple bit copy
 * 
 * Description:
 * - This test verifies that the bit_copy function correctly copies a specified 
 *   number of bits from the source buffer to the destination buffer, starting 
 *   at given bit offsets.
 * 
 * Test Steps:
 * - Set up a source buffer with known values.
 * - Set up a destination buffer initialized to zero.
 * - Copy a specific number of bits starting from specific offsets in both 
 *   buffers.
 * 
 * Expected result:
 * - Assert that the destination buffer matches the expected result.
 */
void test_bit_copy(void) {
    uint8_t src[] = {0xaa, 0xcc};
    uint8_t dst[] = {0, 0};
    
    bit_copy(dst, 4, src, 2, 6);

    TEST_ASSERT_EQUAL_HEX8(0x0a, dst[0]);
    TEST_ASSERT_EQUAL_HEX8(0x80, dst[1]);
}

/**
 * Validate copy across byte boundaries
 * 
 * Description:
 * - This test verifies that the bit_copy function correctly copies a specified 
 *   number of bits from the source buffer to the destination buffer, starting 
 *   at given bit offsets.
 * 
 * Test Steps:
 * - Set up a source buffer with known values.
 * - Set up a destination buffer initialized to zero.
 * - Copy a specific number of bits starting from specific offsets in both 
 *   buffers.
 * 
 * Expected result:
 * - Assert that the destination buffer matches the expected result.
 */
void test_bit_copy_across_byte_boundaries(void) {
    uint8_t src[] = {0xf0, 0xcc};
    uint8_t dst[] = {0x00, 0x00};

    bit_copy(dst, 2, src, 4, 10);

    TEST_ASSERT_EQUAL_HEX8(0x03, dst[0]);
    TEST_ASSERT_EQUAL_HEX8(0x30, dst[1]);
}

/**
 * Toggle unsinged 16-bit integer.
 * 
 * This test ensures that the bit_toggle_endian_u16 function correctly 
 * toggles the endianess of a 16-bit unsigned integer.
 * 
 * Test Steps:
 * - Provide a known 16-bit value.
 * - Call the function to toggle the endianess.
 * 
 * Expected result:
 * - The returned value matches the expected toggled value.
 */
void test_bit_toggle_endian_u16(void)
{
    uint16_t value = 0xabcd;
    uint16_t toggled = bit_toggle_endian_u16(value);

    TEST_ASSERT_EQUAL_HEX16(0xcdab, toggled);
}

/**
 * Toggle unsinged 32-bit integer.
 * 
 * This test ensures that the bit_toggle_endian_u32 function correctly 
 * toggles the endianess of a 32-bit unsigned integer.
 * 
 * Test Steps:
 * - Provide a known 32-bit value.
 * - Call the function to toggle the endianess.
 * 
 * Expected result:
 * - The returned value matches the expected toggled value.
 */
void test_bit_toggle_endian_u32(void)
{
    uint32_t value = 0x12345678;
    uint32_t toggled = bit_toggle_endian_u32(value);

    TEST_ASSERT_EQUAL_HEX32(0x78563412, toggled);
}

/**
 * Toggle unsinged 64-bit integer.
 * 
 * Description:
 * - This test ensures that the bit_toggle_endian_u64 function correctly 
 *   toggles the endianess of a 64-bit unsigned integer.
 * 
 * Test Steps:
 * - Provide a known 64-bit value.
 * - Call the function to toggle the endianess.
 * 
 * Expected result:
 * - The returned value matches the expected reversed value.
 */
void test_bit_toggle_endian_u64(void)
{
    uint64_t value = 0x123456789abcdef0;
    uint64_t toggled = bit_toggle_endian_u64(value);

    TEST_ASSERT_EQUAL_HEX64(0xf0debc9a78563412, toggled);
}

/**
 * Reverse unsinged 16-bit integer.
 * 
 * Description:
 * - This test ensures that the test_bit_reverse_u16 function correctly 
 *   reverse a 16-bit unsigned integer.
 * 
 * Test Steps:
 * - Provide a known 16-bit value.
 * - Call the function to reverse.
 * 
 * Expected result:
 * - The returned value matches the expected reversed value.
 */
void test_bit_reverse_u16(void)
{
    uint16_t value = 0xaaaa;
    uint16_t reversed = bit_reverse_u16(value);

    TEST_ASSERT_EQUAL_HEX16(0x5555, reversed);
}

/**
 * Reverse unsinged 32-bit integer.
 * 
 * Description:
 * - This test ensures that the test_bit_reverse_u32 function correctly 
 *   reverse a 32-bit unsigned integer.
 * 
 * Test Steps:
 * - Provide a known 32-bit value.
 * - Call the function to reverse.
 * 
 * Expected result:
 * - The returned value matches the expected reversed value.
 */
void test_bit_reverse_u32(void)
{
    uint32_t value = 0xaaaaaaaa;
    uint32_t reversed = bit_reverse_u32(value);

    TEST_ASSERT_EQUAL_HEX32(0x55555555, reversed);
}

/**
 * Reverse unsinged 64-bit integer.
 * 
 * Description:
 * - This test ensures that the test_bit_reverse_u64 function correctly 
 *   reverse a 64-bit unsigned integer.
 * 
 * Test Steps:
 * - Provide a known 64-bit value.
 * - Call the function to reverse.
 * - Assert that the returned value matches the expected reversed value.
 */
void test_bit_reverse_u64(void)
{
    uint64_t value = 0x0123456789abcdef;
    uint64_t reversed = bit_reverse_u64(value);

    TEST_ASSERT_EQUAL_HEX64(0xf7b3d591e6a2c480, reversed);
}
