/*
 * Copyright (c) 2024 The cutils authors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "unity.h"
#include "ring_buffer.h"

#include <string.h>
#include <stdlib.h>
#include <time.h>


/**
 * Test cases for byte mode functions.
 */

/**
 * Validate ring buffer capacity
 * 
 * Description:
 * - This test verifies that the ring_buf_capacity_get function correctly 
 *   returns the total capacity of the ring buffer after initialization. It 
 *   ensures that the buffer's capacity matches the size of the underlying 
 *   buffer provided during setup.
 * 
 * Steps:
 * - Initialize a ring buffer of 5 bytes.
 * - Read the capacity of the ring buffer.
 * 
 * Expected result:
 * - The ring_buf_capacity_get function correctly returns a capacity of 5 
 *   bytes, matching the size of the initialized storage buffer.
 */
void test_validate_ring_buffer_capacity(void)
{
    struct ring_buf rb;
    uint8_t buff[5];

    ring_buf_init(&rb, sizeof(buff), buff);

    TEST_ASSERT_EQUAL_UINT32(5, ring_buf_capacity_get(&rb));
}

/**
 * Validate empty buffer
 * 
 * Description:
 * - Ensures that a newly initialized buffer is empty.
 * 
 * Steps:
 * - Initialize a ring buffer of 5 bytes.
 * - Check ring_buf_is_empty.
 * 
 * Expected result:
 * - ring_buf_is_empty returns true.
 */
void test_validate_empty_buffer(void)
{
    struct ring_buf rb;
    uint8_t buff[5];

    ring_buf_init(&rb, sizeof(buff), buff);

    TEST_ASSERT_TRUE(ring_buf_is_empty(&rb));
}

/**
 * Validate buffer is not empty after write
 * 
 * Description:
 * - Verifies that writing data changes the buffer's state from empty to 
 *   not empty.
 * 
 * Steps:
 * - Initialize a ring buffer.
 * - Write 2 bytes to the buffer.
 * - Check ring_buf_is_empty.
 * 
 * Expected result:
 * - ring_buf_is_empty returns false.
 */
void test_validate_not_empty_after_write(void)
{
    struct ring_buf rb;
    uint8_t buff[5];
    uint8_t data[2] = {0x01, 0x02};

    ring_buf_init(&rb, sizeof(buff), buff);
    ring_buf_put(&rb, data, 2);

    TEST_ASSERT_FALSE(ring_buf_is_empty(&rb));
}

/**
 * Validate buffer space after partial write
 * 
 * Description:
 * - Confirms that available space decreases correctly after a partial write.
 * 
 * Steps:
 * - Initialize a ring buffer.
 * - Write 3 bytes to the buffer.
 * - Check available space with ring_buf_space_get.
 * 
 * Expected result:
 * - ring_buf_space_get returns 2.
 * - ring_buf_size_get returns 3.
 */
void test_validate_space_after_partial_write(void)
{
    struct ring_buf rb;
    uint8_t buff[5];
    uint8_t data[3] = {0x01, 0x02, 0x03};

    ring_buf_init(&rb, sizeof(buff), buff);
    ring_buf_put(&rb, data, 3);

    TEST_ASSERT_EQUAL_UINT32(2, ring_buf_space_get(&rb));
}

/**
 * Validate full buffer space
 * 
 * Description:
 * - Confirms that ring_buf_space_get returns 0 when the buffer is full.
 * 
 * Steps:
 * - Initialize a buffer of 5 bytes.
 * - Write 5 bytes to fill the buffer.
 * - Check ring_buf_space_get.
 * 
 * Expected result:
 * - ring_buf_space_get returns 0.
 */
void test_validate_full_buffer_space(void)
{
    struct ring_buf rb;
    uint8_t buff[5];
    uint8_t data[5] = {0x01, 0x02, 0x03, 0x04, 0x05};

    ring_buf_init(&rb, sizeof(buff), buff);
    ring_buf_put(&rb, data, 5);

    TEST_ASSERT_EQUAL_UINT32(0, ring_buf_space_get(&rb));
}

/**
 * Validate size after full write
 * 
 * Description:
 * - Ensures that ring_buf_size_get reports the correct size after writing the full buffer.
 * 
 * Steps:
 * - Initialize a buffer of 5 bytes.
 * - Write 5 bytes to fill the buffer.
 * - Check ring_buf_size_get.
 * 
 * Expected result:
 * - ring_buf_space_get returns 5.
 */
void test_validate_size_after_full_write(void)
{
    struct ring_buf rb;
    uint8_t buff[5];
    uint8_t data[5] = {0x01, 0x02, 0x03, 0x04, 0x05};

    ring_buf_init(&rb, sizeof(buff), buff);
    ring_buf_put(&rb, data, 5);

    TEST_ASSERT_EQUAL_UINT32(5, ring_buf_size_get(&rb));
}

/**
 * Validate space after full read
 * 
 * Description:
 * - Verifies that space is fully restored after reading all data from the buffer.
 * 
 * Steps:
 * - Initialize a buffer of 5 bytes.
 * - Write 5 bytes to fill the buffer.
 * - Read all 5 bytes.
 * - Check ring_buf_space_get.
 * 
 * Expected result:
 * - ring_buf_space_get returns 5.
 */
void test_validate_space_after_full_read(void)
{
    struct ring_buf rb;
    uint8_t buff[5];
    uint8_t data[5] = {0x01, 0x02, 0x03, 0x04, 0x05};
    uint8_t read[5] = {0};

    ring_buf_init(&rb, sizeof(buff), buff);
    ring_buf_put(&rb, data, 5);
    ring_buf_get(&rb, read, 5);

    TEST_ASSERT_EQUAL_UINT32(5, ring_buf_space_get(&rb));
}

/**
 * Write 0 bytes
 * 
 * Description:
 * - Ensures that writing 0 bytes has no effect on the buffer.
 * 
 * Steps:
 * - Initialize a buffer of 5 bytes.
 * - Write 0 bytes.
 * - Check ring_buf_is_empty.
 * 
 * Expected result:
 * - Buffer remains empty.
 */
void test_write_zero_bytes(void)
{
    struct ring_buf rb;
    uint8_t buff[5];
    uint8_t data[5] = {0};

    ring_buf_init(&rb, sizeof(buff), buff);
    ring_buf_put(&rb, data, 0);

    TEST_ASSERT_TRUE(ring_buf_is_empty(&rb));
}

/**
 * Read 0 bytes
 * 
 * Description:
 * - Ensures that attempting to read 0 bytes does not affect the buffer 
 *   or its contents.
 * 
 * Steps:
 * - Initialize a buffer of 5 bytes.
 * - Write 3 bytes to the buffer.
 * - Attempt to read 0 bytes from the buffer.
 * - Verify the buffer size and data remain unchanged.
 * 
 * Expected result:
 * - The buffer size remains 3, and no data is altered.
 */
void test_read_zero_bytes(void)
{
    struct ring_buf rb;
    uint8_t buff[5];
    uint8_t data[3] = {0x01, 0x02, 0x03};
    uint8_t read[5] = {0};

    ring_buf_init(&rb, sizeof(buff), buff);
    ring_buf_put(&rb, data, 3);

    TEST_ASSERT_EQUAL_UINT32(3, ring_buf_size_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(0, ring_buf_get(&rb, read, 0));
    TEST_ASSERT_EQUAL_UINT32(3, ring_buf_size_get(&rb));
}

/**
 * Read more than available
 * 
 * Description:
 * - Verifies that reading more bytes than are available returns only the 
 *   available data without errors.
 * 
 * Steps:
 * - Initialize a buffer of 5 bytes.
 * - Write 3 bytes to the buffer.
 * - Attempt to read 5 bytes.
 * - Verify only 3 bytes are returned, and the buffer becomes empty.
 * 
 * Expected result:
 * - Only 3 bytes are returned, and the buffer is empty.
 */
void test_read_more_than_available(void)
{
    struct ring_buf rb;
    uint8_t buff[5];
    uint8_t data[3] = {0x01, 0x02, 0x03};
    uint8_t read[5] = {0};

    ring_buf_init(&rb, sizeof(buff), buff);
    ring_buf_put(&rb, data, 3);

    TEST_ASSERT_EQUAL_UINT32(3, ring_buf_get(&rb, read, 5));
    TEST_ASSERT_EQUAL_MEMORY(data, read, 3);

    TEST_ASSERT_TRUE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(0, ring_buf_size_get(&rb));
}

/**
 * Write more than capacity
 * 
 * Description:
 * - Ensures that attempting to write more bytes than the buffer's capacity 
 *   results in no overflow and only writes up to capacity.
 * 
 * Steps:
 * - Initialize a buffer of 5 bytes.
 * - Attempt to write 6 bytes to the buffer.
 * - Verify only 5 bytes are written, and the buffer is full.
 * 
 * Expected result:
 * - Only 5 bytes are written, and the buffer is full.
 */
void test_write_more_than_capacity(void)
{
    struct ring_buf rb;
    uint8_t buff[5];
    uint8_t data[6] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};

    ring_buf_init(&rb, sizeof(buff), buff);

    TEST_ASSERT_EQUAL_UINT32(5, ring_buf_put(&rb, data, 6));
    TEST_ASSERT_EQUAL_UINT32(5, ring_buf_size_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(0, ring_buf_space_get(&rb));
}

/**
 * Repeated writes and reads
 * 
 * Description:
 * - Validates the behavior of the buffer under multiple writes and 
 *   reads in sequence.
 * 
 * Steps:
 * - Initialize a buffer of 5 bytes.
 * - Write and read alternately: Write 3 bytes, read 2 bytes, write 2 bytes, 
 *   read 3 bytes.
 * - Verify the buffer size and content at each step.
 * 
 * Expected result:
 * - Writes and reads alternate correctly, maintaining data integrity.
 */
void test_repeated_writes_and_reads(void)
{
    struct ring_buf rb;
    uint8_t buff[5];
    uint8_t data[5] = {0x01, 0x02, 0x03, 0x04, 0x05};
    uint8_t read[5] = {0};

    ring_buf_init(&rb, sizeof(buff), buff);

    TEST_ASSERT_EQUAL_UINT32(3, ring_buf_put(&rb, data, 3));
    TEST_ASSERT_EQUAL_UINT32(3, ring_buf_size_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(2, ring_buf_get(&rb, read, 2));
    TEST_ASSERT_EQUAL_MEMORY(data, read, 2);

    TEST_ASSERT_EQUAL_UINT32(2, ring_buf_put(&rb, &data[3], 2));
    TEST_ASSERT_EQUAL_UINT32(3, ring_buf_size_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(3, ring_buf_get(&rb, read, 3));
    TEST_ASSERT_EQUAL_MEMORY(&data[2], read, 3);
    TEST_ASSERT_TRUE(ring_buf_is_empty(&rb));
}

/**
 * Validate buffer reset
 * 
 * Description:
 * - Ensures the buffer is cleared after a reset operation.
 * 
 * Steps:
 * - Initialize a buffer of 5 bytes.
 * - Write 3 bytes to the buffer.
 * - Reset the buffer.
 * - Verify that the buffer is empty and capacity is restored.
 * 
 * Expected result:
 * - The buffer is cleared, and all capacity is restored.
 */
void test_validate_buffer_reset(void)
{
    struct ring_buf rb;
    uint8_t buff[5];
    uint8_t data[3] = {0x01, 0x02, 0x03};

    ring_buf_init(&rb, sizeof(buff), buff);
    ring_buf_put(&rb, data, 3);

    ring_buf_reset(&rb);

    TEST_ASSERT_TRUE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(5, ring_buf_space_get(&rb));
}

/**
 * Check write after wraparound
 * 
 * Description:
 * - Verifies correct behavior when writing after the buffer has wrapped 
 *   around.
 * 
 * Steps:
 * - Initialize a buffer of 5 bytes.
 * - Write and read alternately until the buffer wraps around.
 * - Write new data and verify content.
 * 
 * Expected result:
 * - Buffer handles wraparound without errors.
 */
void test_write_after_wraparound(void)
{
    struct ring_buf rb;
    uint8_t buff[5];
    uint8_t data[5] = {0x01, 0x02, 0x03, 0x04, 0x05};
    uint8_t read[2] = {0};

    ring_buf_init(&rb, sizeof(buff), buff);

    ring_buf_put(&rb, data, 5);
    ring_buf_get(&rb, read, 2);

    TEST_ASSERT_EQUAL_UINT32(2, ring_buf_put(&rb, data, 2));
    TEST_ASSERT_EQUAL_UINT32(5, ring_buf_size_get(&rb));
}

/**
 * Validate reading after wraparound
 * 
 * Description:
 * - Ensures that data integrity is maintained when reading after the 
 *   buffer wraps around.
 * 
 * Steps:
 * - Initialize a buffer of 5 bytes.
 * - Write 5 bytes to fill the buffer.
 * - Read 2 bytes to advance the read pointer.
 * - Write 2 bytes to cause a wraparound.
 * - Read all remaining bytes and validate content.
 * 
 * Expected result:
 * - Data is read in the correct order, and integrity is preserved.
 */
void test_validate_reading_after_wraparound(void)
{
    struct ring_buf rb;
    uint8_t buff[5];
    uint8_t data[5] = {0x01, 0x02, 0x03, 0x04, 0x05};
    uint8_t wrap_data[2] = {0x06, 0x07};
    uint8_t read[7] = {0};
    uint8_t expected[5] = {0x03, 0x04, 0x05, 0x06, 0x07};

    ring_buf_init(&rb, sizeof(buff), buff);

    ring_buf_put(&rb, data, 5);
    ring_buf_get(&rb, read, 2);

    TEST_ASSERT_EQUAL_UINT32(2, ring_buf_put(&rb, wrap_data, 2));
    TEST_ASSERT_EQUAL_UINT32(5, ring_buf_size_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(5, ring_buf_get(&rb, read, 5));
    TEST_ASSERT_EQUAL_MEMORY(expected, read, 5);
}

/**
 * Interleaved read/write stress test
 * 
 * Description:
 * - Tests multiple interleaved writes and reads to ensure the buffer 
 *   handles complex operations without errors.
 * 
 * Steps:
 * - Initialize a buffer of 5 bytes.
 * - Perform a series of interleaved writes and reads, varying the sizes.
 * - Verify data integrity after each operation.
 * 
 * Expected result:
 * - Buffer handles all operations correctly, and data integrity is 
 *   preserved.
 */
void test_interleaved_read_write_stress(void)
{
    struct ring_buf rb;
    uint8_t buff[5];
    uint8_t data1[3] = {0x01, 0x02, 0x03};
    uint8_t data2[2] = {0x04, 0x05};
    uint8_t data3[3] = {0x06, 0x07, 0x08};
    uint8_t read[5] = {0};

    ring_buf_init(&rb, sizeof(buff), buff);

    ring_buf_put(&rb, data1, 3);
    TEST_ASSERT_EQUAL_UINT32(3, ring_buf_get(&rb, read, 3));
    TEST_ASSERT_EQUAL_MEMORY(data1, read, 3);

    ring_buf_put(&rb, data2, 2);
    ring_buf_put(&rb, data3, 3);

    TEST_ASSERT_EQUAL_UINT32(5, ring_buf_get(&rb, read, 5));
    uint8_t expected[5] = {0x04, 0x05, 0x06, 0x07, 0x08};
    TEST_ASSERT_EQUAL_MEMORY(expected, read, 5);
}

/**
 * Single-byte buffer edge case
 * 
 * Description:
 * - Validates the behavior of a single-byte buffer under write, 
 *   read, and overwrite conditions.
 * 
 * Steps:
 * - Initialize a buffer of 1 byte.
 * - Perform writes, reads, and overwrite attempts.
 * - Verify the buffer behaves as expected.
 * 
 * Expected result:
 * - The buffer correctly handles single-byte operations and rejects 
 *   overwrites.
 */
void test_single_byte_buffer(void)
{
    struct ring_buf rb;
    uint8_t buff[1];
    uint8_t data = 0x01;
    uint8_t read = 0;

    ring_buf_init(&rb, sizeof(buff), buff);

    TEST_ASSERT_EQUAL_UINT32(1, ring_buf_put(&rb, &data, 1));
    TEST_ASSERT_EQUAL_UINT32(1, ring_buf_size_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(0, ring_buf_space_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(1, ring_buf_get(&rb, &read, 1));
    TEST_ASSERT_EQUAL_UINT8(data, read);
    TEST_ASSERT_TRUE(ring_buf_is_empty(&rb));
}

/**
 * Empty buffer read validation
 * 
 * Description:
 * - Ensures that attempting to read from an empty buffer returns 
 *   no data and does not modify the buffer state.
 * 
 * Steps:
 * - Initialize a buffer of 5 bytes.
 * - Attempt to read data without writing any data to the buffer.
 * 
 * Expected result:
 * - Read operation returns 0 bytes, and the buffer remains empty.
 */
void test_empty_buffer_read_validation(void)
{
    struct ring_buf rb;
    uint8_t buff[5];
    uint8_t read[5] = {0};

    ring_buf_init(&rb, sizeof(buff), buff);

    TEST_ASSERT_EQUAL_UINT32(0, ring_buf_get(&rb, read, 5));
    TEST_ASSERT_TRUE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(5, ring_buf_space_get(&rb));
}

/**
 * Stress test with large buffer
 * 
 * Description:
 * - Tests the behavior of a large buffer under heavy load.
 * 
 * Steps:
 * - Initialize a buffer of 1000 bytes.
 * - Write 1000 bytes to the buffer.
 * - Read all 1000 bytes back and validate data integrity.
 * 
 * Expected result:
 * - The buffer handles the large data set correctly, preserving data 
 *   integrity.
 */
void test_large_buffer_stress(void)
{
    struct ring_buf rb;
    uint8_t buff[1000];
    uint8_t data[1000];
    uint8_t read[1000] = {0};

    for (uint32_t i = 0; i < 1000; i++) {
        data[i] = (uint8_t) (i % 256);
    }

    ring_buf_init(&rb, sizeof(buff), buff);

    TEST_ASSERT_EQUAL_UINT32(1000, ring_buf_put(&rb, data, 1000));
    TEST_ASSERT_EQUAL_UINT32(1000, ring_buf_size_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(1000, ring_buf_get(&rb, read, 1000));
    TEST_ASSERT_EQUAL_MEMORY(data, read, 1000);
    TEST_ASSERT_TRUE(ring_buf_is_empty(&rb));
}

/**
 * High-frequency alternating read/write stress test
 * 
 * Description:
 * - Simulates high-frequency alternating writes and reads to validate the 
 *   buffer's stability under rapid operations.
 * 
 * Steps:
 * - Initialize a buffer of 5 bytes.
 * - Perform alternating writes and reads with varying sizes in a loop.
 * - Verify no data corruption occurs.
 * 
 * Expected result:
 * - The buffer operates without errors under rapid, alternating access.
 */
void test_high_frequency_alternating_read_write(void)
{
    struct ring_buf rb;
    uint8_t buff[5];
    uint8_t data[5] = {0x01, 0x02, 0x03, 0x04, 0x05};
    uint8_t read[3] = {0};

    ring_buf_init(&rb, sizeof(buff), buff);

    for (int i = 0; i < 10; i++) {
        ring_buf_put(&rb, data, 3);
        ring_buf_get(&rb, read, 3);
        TEST_ASSERT_EQUAL_MEMORY(data, read, 3);
    }

    TEST_ASSERT_TRUE(ring_buf_is_empty(&rb));
}

/**
 * Write and read all-zero bytes
 * 
 * Description:
 * - Verifies the behavior of the buffer when all-zero bytes are 
 *   written and read.
 * 
 * Steps:
 * - Initialize a buffer of 5 bytes.
 * - Write all-zero bytes to the buffer.
 * - Read the bytes back and verify they are all zeros.
 * 
 * Expected result:
 * - The buffer correctly handles zero-byte data.
 */
void test_write_and_read_all_zero_bytes(void)
{
    struct ring_buf rb;
    uint8_t buff[5];
    uint8_t data[5] = {0};
    uint8_t read[5] = {0};

    ring_buf_init(&rb, sizeof(buff), buff);

    TEST_ASSERT_EQUAL_UINT32(5, ring_buf_put(&rb, data, 5));
    TEST_ASSERT_EQUAL_UINT32(5, ring_buf_get(&rb, read, 5));
    TEST_ASSERT_EQUAL_MEMORY(data, read, 5);
    TEST_ASSERT_TRUE(ring_buf_is_empty(&rb));
}

/**
 * Peek at front of buffer
 * 
 * Description:
 * - Verifies the behavior of the buffer when all-zero bytes are 
 *   written and read.
 * 
 * Steps:
 * - Initializes a buffer of 5 bytes and writes 3 bytes of data to it.
 * - Calls peek to look at the first 3 bytes.
 * 
 * Expected result:
 * - Ensures that the data returned by peek matches the written data and 
 *   confirms the buffer's state remains unchanged.
 */
void test_peek_at_front_of_buffer(void)
{
    struct ring_buf rb;
    uint8_t buff[5];
    uint8_t data[5] = {0x01, 0x02, 0x03, 0x04, 0x05};
    uint8_t peek_data[3];

    ring_buf_init(&rb, sizeof(buff), buff);

    ring_buf_put(&rb, data, 3);

    TEST_ASSERT_EQUAL_UINT32(3, ring_buf_peek(&rb, peek_data, 3));

    TEST_ASSERT_EQUAL_MEMORY(data, peek_data, 3);

    TEST_ASSERT_EQUAL_UINT32(2, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(3, ring_buf_size_get(&rb));
}

/**
 *  Peek on empty buffer
 * 
 * Description:
 * - Verifies the behavior of the buffer when all-zero bytes are 
 *   written and read.
 * 
 * Steps:
 * - Initializes an empty buffer.
 * - Calls peek on an empty buffer
 * 
 * Expected result:
 * -  Ensures peek returns 0 and doesn't modify the buffer data 
 *    (which should remain unchanged). Additionally, checks that 
 *    the buffer's space and size remain the same.
 */
void test_peek_on_empty_buffer(void)
{
    struct ring_buf rb;
    uint8_t buff[5];
    uint8_t peek_data[3] = {0xff, 0xff, 0xff};

    ring_buf_init(&rb, sizeof(buff), buff);

    TEST_ASSERT_EQUAL_UINT32(0, ring_buf_peek(&rb, peek_data, 3));
  
    for (int i = 0; i < 3; i++) {
        TEST_ASSERT_EQUAL_UINT8(0xff, peek_data[i]);
    }

    TEST_ASSERT_EQUAL_UINT32(5, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(0, ring_buf_size_get(&rb));
}

/**
 * Simple wraparound case (boundary test)
 * 
 * Description:
 * - Fill the buffer until it's nearly full, then attempt to insert more 
 *   data to trigger wraparound at the buffer's end.
 * - Then try reading enough data to cause the get pointer to wrap around as well.
 * 
 * Steps:
 * - Initialize a ring buffer with a small size (e.g., 10 bytes).
 * - Write 8 bytes to the buffer.
 * - Read 3 bytes to advance the get_tail.
 * - Write 5 more bytes to the buffer (should wrap around).
 * - Attempt to read and ensure data integrity.
 * 
 * Expected result:
 * - After wraparound, put_head and get_tail correctly reflect positions within 
 *   the buffer.
 * - Available space and size calculations should still be accurate after 
 *   wraparound.
 */
void test_simple_wraparound_case(void)
{
    struct ring_buf rb;
    uint8_t buff[10];
    uint8_t data[10] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa};
    uint8_t data_2[10] = {0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca};
    uint8_t read[10] = {0};

    ring_buf_init(&rb, sizeof(buff), buff);

    TEST_ASSERT_TRUE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(10, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(0, ring_buf_size_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(8, ring_buf_put(&rb, data, 8));

    TEST_ASSERT_FALSE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(2, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(8, ring_buf_size_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(3, ring_buf_get(&rb, read, 3));
    TEST_ASSERT_EQUAL_MEMORY(data, read, 3);
    
    memset(read, 0, sizeof(read));

    TEST_ASSERT_FALSE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(5, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(5, ring_buf_size_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(5, ring_buf_put(&rb, data_2, 5));

    TEST_ASSERT_FALSE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(0, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(10, ring_buf_size_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(10, ring_buf_get(&rb, read, 10));
    TEST_ASSERT_EQUAL_MEMORY(&data[3], read, 5);
    TEST_ASSERT_EQUAL_MEMORY(data_2, &read[5], 5);
    
    TEST_ASSERT_TRUE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(10, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(0, ring_buf_size_get(&rb));
}

/**
 * Edge case with exact buffer capacity
 * 
 * Description:
 * - Fill the buffer exactly to its capacity to check if wraparound calculations
 *   manage full buffer correctly.
 * 
 * Steps:
 * - Initialize a ring buffer with a size of 10 bytes.
 * - Write exactly 10 bytes.
 * - Attempt a further write, which should fail as the buffer is full.
 * - Read all 10 bytes from the buffer.
 * - Verify that the buffer now has its full capacity available again.
 *  
 * Expected result:
 * - When the buffer is full, the space calculation should return zero, and any 
 *   write should fail.
 * - After reading all bytes, the space should again be the full buffer size, with 
 *   get_tail and put_head correctly reset to start positions.
 */
void test_edge_case_with_exact_buffer_capacity(void)
{
    struct ring_buf rb;
    uint8_t buff[10];
    uint8_t data[10] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa};
    uint8_t data_2[10] = {0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca};
    uint8_t read[10] = {0};

    ring_buf_init(&rb, sizeof(buff), buff);

    TEST_ASSERT_TRUE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(10, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(0, ring_buf_size_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(10, ring_buf_put(&rb, data, 10));

    TEST_ASSERT_FALSE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(0, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(10, ring_buf_size_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(0, ring_buf_put(&rb, data_2, 10));
    
    TEST_ASSERT_FALSE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(0, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(10, ring_buf_size_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(10, ring_buf_get(&rb, read, 10));
    TEST_ASSERT_EQUAL_MEMORY(data, read, 10);

    TEST_ASSERT_TRUE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(10, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(0, ring_buf_size_get(&rb));
}

/**
 * Underflow check (subtraction edge case)
 * 
 * Description:
 * - Test a scenario where subtracting put_head - get_tail or 
 *   get_tail - put_head might produce unexpected results due 
 *   to unsigned arithmetic.
 * 
 * Steps:
 * - Initialize a ring buffer with a size of 10 bytes.
 * - Write 3 bytes to advance put_head.
 * - Read 3 bytes to advance get_tail, bringing both put_head and get_tail 
 *   back to zero.
 * - Attempt to read again without writing, which should correctly return zero 
 *   size without underflow.
 *  
 * Expected result:
 * - Size and space calculations handle the zero state correctly without 
 *   any underflow or unexpected values.
 * - Buffer space and size return correct values.
 */
void test_underflow_check(void)
{
    struct ring_buf rb;
    uint8_t buff[10];
    uint8_t data[10] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa};
    uint8_t read[10] = {0};

    ring_buf_init(&rb, sizeof(buff), buff);

    TEST_ASSERT_TRUE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(10, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(0, ring_buf_size_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(3, ring_buf_put(&rb, data, 3));
    
    TEST_ASSERT_FALSE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(7, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(3, ring_buf_size_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(3, ring_buf_get(&rb, read, 3));
    TEST_ASSERT_EQUAL_MEMORY(data, read, 3);

    TEST_ASSERT_TRUE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(10, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(0, ring_buf_size_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(0, ring_buf_get(&rb, read, 3));

    TEST_ASSERT_TRUE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(10, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(0, ring_buf_size_get(&rb));
}

/**
 * Partial fill and wraparound write (size calculation)
 * 
 * Description:
 * - Test filling the buffer partially, reading some data, then 
 *   writing more data to check wraparound handling in the 
 *   ring_buf_size_get
 * 
 * Steps:
 * - Initialize a ring buffer of 10 bytes.
 * - Write 7 bytes (advances put_head).
 * - Read 4 bytes (advances get_tail).
 * - Write 5 more bytes to force wraparound on put_head.
 *  
 * Expected result:
 * - ring_buf_size_get should accurately calculate the size, reflecting 
 *   only the available data in the correct order despite the wraparound.
 * - The read order should follow FIFO without data corruption.
 */
void test_partial_fill_and_wraparound_write(void)
{
    struct ring_buf rb;
    uint8_t buff[10];
    uint8_t data[10] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa};
    uint8_t data_2[10] = {0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca};
    uint8_t read[10] = {0};

    ring_buf_init(&rb, sizeof(buff), buff);

    TEST_ASSERT_TRUE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(10, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(0, ring_buf_size_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(7, ring_buf_put(&rb, data, 7));
    
    TEST_ASSERT_FALSE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(3, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(7, ring_buf_size_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(4, ring_buf_get(&rb, read, 4));
    TEST_ASSERT_EQUAL_MEMORY(data, read, 4);
    
    memset(read, 0, sizeof(read));

    TEST_ASSERT_FALSE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(7, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(3, ring_buf_size_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(5, ring_buf_put(&rb, data_2, 5));

    TEST_ASSERT_FALSE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(2, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(8, ring_buf_size_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(8, ring_buf_get(&rb, read, 8));
    TEST_ASSERT_EQUAL_MEMORY(&data[4], read, 3);
    TEST_ASSERT_EQUAL_MEMORY(data_2, &read[3], 5);

    TEST_ASSERT_TRUE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(10, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(0, ring_buf_size_get(&rb));
}

/**
 * Full wraparound cycle (completeness test)
 * 
 * Description:
 * - This test fills the buffer, empties it, and refills it to 
 *   ensure that wraparound logic works seamlessly over multiple cycles.
 * 
 * Steps:
 * - Initialize a ring buffer of 10 bytes.
 * - Write 10 bytes (fill the buffer).
 * - Read 10 bytes (empty the buffer, advancing both pointers back to zero).
 * - Repeat the above steps to fill and empty the buffer twice more.
 *  
 * Expected result:
 * - ring_buf_space_get and ring_buf_size_get should accurately reflect a full 
 *   buffer (zero space) and an empty buffer (full space) after each cycle.
 * - All values reset correctly, with no data corruption or unexpected values.
 */
void test_full_wraparound_cycle(void)
{
    struct ring_buf rb;
    uint8_t buff[10];
    uint8_t data[10] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa};
    uint8_t read[10] = {0};

    ring_buf_init(&rb, sizeof(buff), buff);

    TEST_ASSERT_TRUE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(10, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(0, ring_buf_size_get(&rb));
    
    TEST_ASSERT_EQUAL_UINT32(10, ring_buf_put(&rb, data, 10));
    
    TEST_ASSERT_FALSE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(0, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(10, ring_buf_size_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(10, ring_buf_get(&rb, read, 10));
    TEST_ASSERT_EQUAL_MEMORY(data, read, 10);
    
    memset(read, 0, sizeof(read));

    TEST_ASSERT_TRUE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(10, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(0, ring_buf_size_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(10, ring_buf_put(&rb, data, 10));
    
    TEST_ASSERT_FALSE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(0, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(10, ring_buf_size_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(10, ring_buf_get(&rb, read, 10));
    TEST_ASSERT_EQUAL_MEMORY(data, read, 10);
    
    memset(read, 0, sizeof(read));

    TEST_ASSERT_TRUE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(10, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(0, ring_buf_size_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(10, ring_buf_put(&rb, data, 10));
    
    TEST_ASSERT_FALSE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(0, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(10, ring_buf_size_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(10, ring_buf_get(&rb, read, 10));
    TEST_ASSERT_EQUAL_MEMORY(data, read, 10);
    
    memset(read, 0, sizeof(read));

    TEST_ASSERT_TRUE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(10, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(0, ring_buf_size_get(&rb));
}

/**
 * Wraparound with interleaved reads and writes (robustness check)
 * 
 * Description:
 * - This test interleaves reads and writes to stress-test the wraparound 
 *   behavior.
 * 
 * Steps:
 * - Initialize a ring buffer of 10 bytes.
 * - Write 6 bytes (advance put_head).
 * - Read 2 bytes (advance get_tail).
 * - Write 4 more bytes to fill the buffer and cause wraparound.
 * - Read 3 bytes (advance get_tail), creating additional free space.
 * - Write 3 more bytes to check how wraparound affects free space handling.
 *  
 * Expected result:
 * - All operations (reads and writes) should succeed without error, even 
 *   with wraparound.
 * - After each operation, verify that ring_buf_space_get and ring_buf_size_get 
 *   return expected values.
 */
void test_wraparound_with_interleaved_reads_and_writes(void)
{
    struct ring_buf rb;
    uint8_t buff[10];
    uint8_t data[10] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa};
    uint8_t data_2[10] = {0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca};
    uint8_t read[10] = {0};

    ring_buf_init(&rb, sizeof(buff), buff);
    
    TEST_ASSERT_TRUE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(10, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(0, ring_buf_size_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(6, ring_buf_put(&rb, data, 6));
    
    TEST_ASSERT_FALSE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(4, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(6, ring_buf_size_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(2, ring_buf_get(&rb, read, 2));
    TEST_ASSERT_EQUAL_MEMORY(data, read, 2);
    
    memset(read, 0, sizeof(read));

    TEST_ASSERT_FALSE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(6, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(4, ring_buf_size_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(4, ring_buf_put(&rb, data, 4));
    
    TEST_ASSERT_FALSE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(2, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(8, ring_buf_size_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(3, ring_buf_get(&rb, read, 3));
    TEST_ASSERT_EQUAL_MEMORY(&data[2], read, 3);
    
    memset(read, 0, sizeof(read));

    TEST_ASSERT_FALSE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(5, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(5, ring_buf_size_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(3, ring_buf_put(&rb, data_2, 3));
    
    TEST_ASSERT_FALSE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(2, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(8, ring_buf_size_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(8, ring_buf_get(&rb, read, 8));
    
    TEST_ASSERT_EQUAL_MEMORY(&data[5], read, 1);
    TEST_ASSERT_EQUAL_MEMORY(data, &read[1], 4);
    TEST_ASSERT_EQUAL_MEMORY(data_2, &read[5], 3);

    TEST_ASSERT_TRUE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(10, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(0, ring_buf_size_get(&rb));
}

/**
 * Stress test with randomized operations
 * 
 * Description:
 * - Run a sequence of random reads and writes to simulate real-world usage 
 *   and ensure the buffer remains consistent.
 * 
 * Steps:
 * - Initialize a ring buffer of 10 bytes.
 * - Randomly perform reads or writes within the buffer's capacity.
 * - After each operation, check ring_buf_space_get, ring_buf_size_get, and 
 *   that data integrity is maintained.
 *  
 * Expected result:
 * - All operations should handle wraparound without any assertion failures or 
 *   data inconsistencies.
 */
void test_stress_test_with_randomized_operations(void)
{
    struct ring_buf rb;
    uint8_t buff[10];
    uint8_t data[10] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa};
    uint8_t data_2[10] = {0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca};
    uint8_t read[10] = {0};

    ring_buf_init(&rb, sizeof(buff), buff);
    
    TEST_ASSERT_TRUE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(10, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(0, ring_buf_size_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(6, ring_buf_put(&rb, data, 6));
    
    TEST_ASSERT_FALSE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(4, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(6, ring_buf_size_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(2, ring_buf_put(&rb, &data[6], 2));
    
    TEST_ASSERT_FALSE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(2, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(8, ring_buf_size_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(0, ring_buf_put(&rb, data_2, 0));
    
    TEST_ASSERT_FALSE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(2, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(8, ring_buf_size_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(0, ring_buf_get(&rb, read, 0));
    
    TEST_ASSERT_FALSE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(2, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(8, ring_buf_size_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(8, ring_buf_peek(&rb, read, UINT32_MAX));
    TEST_ASSERT_EQUAL_MEMORY(data, read, 8);
    
    memset(read, 0, sizeof(read));

    TEST_ASSERT_FALSE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(2, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(8, ring_buf_size_get(&rb));

    TEST_ASSERT_EQUAL_UINT32(8, ring_buf_get(&rb, read, 8));
    TEST_ASSERT_EQUAL_MEMORY(data, read, 8);
    
    TEST_ASSERT_TRUE(ring_buf_is_empty(&rb));
    TEST_ASSERT_EQUAL_UINT32(10, ring_buf_space_get(&rb));
    TEST_ASSERT_EQUAL_UINT32(0, ring_buf_size_get(&rb));
}

/**
 * Test cases for item mode functions.
 */

/**
 * Item space test
 * 
 * Description:
 * - This test ensures ring_buf_item_space_get functionality method 
 *   for item mode.
 * 
 * Steps:
 * - Initialize a ring buffer of 16 32-bit words.
 * - Read the space of the ring buffer.
 * 
 * Expected result:
 * - ring_buf_item_space_get returns 16 in this case.
 * 
 */
void test_item_space(void)
{
    struct ring_buf rb;
    uint32_t buff[16];

    ring_buf_item_init(&rb, sizeof(buff), buff);
    TEST_ASSERT_EQUAL_UINT32(sizeof(buff), ring_buf_item_space_get(&rb));
}

/**
 * Single write and single read for item mode (basic functionality test)
 * 
 * Description:
 * - This test ensures that a single write followed by a single read works 
 *   as expected, and verifies that the buffer correctly maintains data 
 *   integrity.
 * 
 * Steps:
 * - Initialize a ring buffer of 10 32-bit words.
 * - Write 1 item of 9 32-bits words.
 * - Read 1 item of 9 32-bits words.
 * 
 * Expected result:
 * - The data read from the buffer matches the data written.
 * 
 */
void test_single_write_and_single_read_for_item_mode(void)
{
    struct ring_buf rb;
    uint32_t buff[10];
    uint32_t data[9] = {1, 2, 3, 4, 5, 6, 7, 8 , 9};
    uint32_t read[9] = {0};

    ring_buf_item_init(&rb, sizeof(buff) / sizeof(uint32_t), buff);
    TEST_ASSERT_EQUAL_INT32(0, ring_buf_item_put(&rb, 19, 8, data, 9));

    uint16_t type = 0;
    uint8_t value = 0;
    uint8_t read_len = 9;
    TEST_ASSERT_EQUAL_INT32(0, ring_buf_item_get(&rb, &type, &value, read, &read_len));
    TEST_ASSERT_EQUAL_UINT8(8, value);
    TEST_ASSERT_EQUAL_UINT16(19, type);
    TEST_ASSERT_EQUAL_UINT8(9, read_len);

    TEST_ASSERT_EQUAL_MEMORY(data, read, sizeof(data));
}

/**
 * Single write and single read with insufficient space for item mode
 * 
 * Description:
 * - This test ensures that ring_buf_item_get method reject a read operation
 *   when size of data area data is too small and size32 now contains the number 
 *   of 32-bit words needed.
 * 
 * Steps:
 * - Initialize a ring buffer of 10 32-bit words.
 * - Write 1 item of 9 32-bits words.
 * - Read 1 item of 9 32-bits words with insufficient space (which should fail).
 * 
 * Expected result:
 * - ring_buf_item_get returns -2.
 * - size32 now contains the number 9 for this case.
 */
void test_single_write_and_single_read_with_insufficient_space_for_item_mode(void)
{
    struct ring_buf rb;
    uint32_t buff[10];
    uint32_t data[9] = {1, 2, 3, 4, 5, 6, 7, 8 , 9};
    uint32_t read[8] = {0};

    ring_buf_item_init(&rb, sizeof(buff) / sizeof(uint32_t), buff);
    TEST_ASSERT_EQUAL_INT32(0, ring_buf_item_put(&rb, 19, 8, data, 9));

    uint16_t type = 0;
    uint8_t value = 0;
    uint8_t read_len = 8;
    TEST_ASSERT_EQUAL_INT32(-2, ring_buf_item_get(&rb, &type, &value, read, &read_len));
    TEST_ASSERT_EQUAL_UINT8(0, value);
    TEST_ASSERT_EQUAL_UINT16(0, type);
    TEST_ASSERT_EQUAL_UINT8(9, read_len);
}

/**
 * Fill buffer exactly and attempt overwrite for item mode (full capacity test)
 * 
 * Description:
 * - This test confirms that the buffer can be filled to its exact capacity and 
 *   that further writes are correctly rejected when the buffer is full.
 * 
 * Steps:
 * - Initialize a ring buffer of 10 32-bit words.
 * - Write 1 item of 9 32-bits words.
 * - Attempt to write 1 additional item of 9 32-bits words (which should fail).
 * - Read 1 item of 9 32-bits words.
 * - Ensure data inegrity.
 * 
 * Expected result:
 *  - The attempt to write an additional byte fails, as the buffer is full.
 *    ring_buf_item_get returns -2.
 */
void test_fill_buffer_exactly_and_attempt_overwrite_for_item_mode(void)
{
    struct ring_buf rb;
    uint32_t buff[10];
    uint32_t data[9] = {1, 2, 3, 4, 5, 6, 7, 8 , 9};
    uint32_t data_2[9] = {11, 12, 13, 14, 15, 16, 17, 18 , 19};
    uint32_t read[9] = {0};

    ring_buf_item_init(&rb, sizeof(buff) / sizeof(uint32_t), buff);
    
    TEST_ASSERT_EQUAL_INT32(0, ring_buf_item_put(&rb, 19, 8, data, 9));
    TEST_ASSERT_EQUAL_INT32(-2, ring_buf_item_put(&rb, 10, 24, data_2, 9));

    uint16_t type = 0;
    uint8_t value = 0;
    uint8_t read_len = 9;
    TEST_ASSERT_EQUAL_INT32(0, ring_buf_item_get(&rb, &type, &value, read, &read_len));
    TEST_ASSERT_EQUAL_UINT8(8, value);
    TEST_ASSERT_EQUAL_UINT16(19, type);
    TEST_ASSERT_EQUAL_UINT8(9, read_len);
    TEST_ASSERT_EQUAL_MEMORY(data, read, sizeof(data));
}

/**
 * Stress test with randomized operations for item mode.
 * 
 * Description:
 * - Run a sequence of random reads and writes to simulate real-world usage 
 *   and ensure the buffer remains consistent.
 * 
 * Steps:
 * - Initialize a ring buffer of 10 32-bit words.
 * - Randomly perform reads or writes within the buffer's capacity.
 * - After each operation, check return values an data integrity.
 *  
 * Expected result:
 * - All operations should handle wraparound without any assertion failures or 
 *   data inconsistencies.
 */
void test_stress_test_with_randomized_operations_for_item_mode(void)
{
    struct ring_buf rb;
    uint32_t buff[10];
    uint32_t data[9] = {1, 2, 3, 4, 5, 6, 7, 8 , 9};
    uint32_t data_2[9] = {11, 12, 13, 14, 15, 16, 17, 18 , 19};
    uint32_t read[9] = {0};

    ring_buf_item_init(&rb, sizeof(buff) / sizeof(uint32_t), buff);
    
    uint16_t type = 0;
    uint8_t value = 0;
    uint8_t read_len = 9;
    TEST_ASSERT_EQUAL_INT32(-1, ring_buf_item_get(&rb, &type, &value, read, &read_len));

    TEST_ASSERT_EQUAL_INT32(0, ring_buf_item_put(&rb, 19, 8, data, 9));

    type = 0;
    value = 0;
    read_len = 9;
    TEST_ASSERT_EQUAL_INT32(0, ring_buf_item_get(&rb, &type, &value, read, &read_len));
    TEST_ASSERT_EQUAL_UINT8(8, value);
    TEST_ASSERT_EQUAL_UINT16(19, type);
    TEST_ASSERT_EQUAL_UINT8(9, read_len);
    TEST_ASSERT_EQUAL_MEMORY(data, read, sizeof(data));

    TEST_ASSERT_EQUAL_INT32(-1, ring_buf_item_get(&rb, &type, &value, read, &read_len));

    TEST_ASSERT_EQUAL_INT32(0, ring_buf_item_put(&rb, 1, 2, data_2, 9));
    TEST_ASSERT_EQUAL_INT32(-2, ring_buf_item_put(&rb, 19, 8, data, 9));

    type = 0;
    value = 0;
    read_len = 9;
    TEST_ASSERT_EQUAL_INT32(0, ring_buf_item_get(&rb, &type, &value, read, &read_len));
    TEST_ASSERT_EQUAL_UINT8(2, value);
    TEST_ASSERT_EQUAL_UINT16(1, type);
    TEST_ASSERT_EQUAL_UINT8(9, read_len);
    TEST_ASSERT_EQUAL_MEMORY(data_2, read, sizeof(data_2));
}
