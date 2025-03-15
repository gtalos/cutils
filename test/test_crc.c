/*
 * Copyright (c) 2024
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "unity.h"
#include "crc.h"


/**
 * Test case 1 IEEE 802.3 CRC
 * 
 * Description:
 * - This test ensures that the IEEE 802.3 CRC algorithm produces the 
 *   expected CRC.
 * 
 * Steps:
 * - Initialize the CRC context with crc_ieee_802_3_init.
 * - Call crc_ieee_802_3_update with input bytes.
 * - Finalize the CRC with crc_ieee_802_3_final.
 *  
 * Expected result:
 * - Check if the result is correct.
 */
void test_crc_ieee_802_3_case_1(void)
{
    struct crc_ieee_802_3_ctx ctx;

    crc_ieee_802_3_init(&ctx);
    crc_ieee_802_3_update(&ctx, "", 0);
    uint32_t crc = crc_ieee_802_3_final(&ctx);

    TEST_ASSERT_EQUAL_HEX32(0x00000000, crc);
}

/**
 * Test case 2 IEEE 802.3 CRC
 * 
 * Description:
 * - This test ensures that the IEEE 802.3 CRC algorithm produces the 
 *   expected CRC.
 * 
 * Steps:
 * - Initialize the CRC context with crc_ieee_802_3_init.
 * - Call crc_ieee_802_3_update with input bytes.
 * - Finalize the CRC with crc_ieee_802_3_final.
 *  
 * Expected result:
 * - Check if the result is correct.
 */
void test_crc_ieee_802_3_case_2(void)
{
    struct crc_ieee_802_3_ctx ctx;
    uint8_t data = 0x01;

    crc_ieee_802_3_init(&ctx);
    crc_ieee_802_3_update(&ctx, &data, sizeof(data));
    uint32_t crc = crc_ieee_802_3_final(&ctx);

    TEST_ASSERT_EQUAL_HEX32(0xa505df1b, crc);
}

/**
 * Test case 3 IEEE 802.3 CRC
 * 
 * Description:
 * - This test ensures that the IEEE 802.3 CRC algorithm produces the 
 *   expected CRC.
 * 
 * Steps:
 * - Initialize the CRC context with crc_ieee_802_3_init.
 * - Call crc_ieee_802_3_update with input bytes.
 * - Finalize the CRC with crc_ieee_802_3_final.
 *  
 * Expected result:
 * - Check if the result is correct.
 */
void test_crc_ieee_802_3_case_3(void)
{
    struct crc_ieee_802_3_ctx ctx;
    uint8_t data[] = {0xde, 0xad, 0xbe, 0xef};

    crc_ieee_802_3_init(&ctx);
    crc_ieee_802_3_update(&ctx, data, sizeof(data));
    uint32_t crc = crc_ieee_802_3_final(&ctx);

    TEST_ASSERT_EQUAL_HEX32(0x7c9ca35a, crc);
}

/**
 * Test case 4 IEEE 802.3 CRC
 * 
 * Description:
 * - This test ensures that the IEEE 802.3 CRC algorithm produces the 
 *   expected CRC.
 * 
 * Steps:
 * - Initialize the CRC context with crc_ieee_802_3_init.
 * - Call crc_ieee_802_3_update with input bytes.
 * - Finalize the CRC with crc_ieee_802_3_final.
 *  
 * Expected result:
 * - Check if the result is correct.
 */
void test_crc_ieee_802_3_case_4(void)
{
    struct crc_ieee_802_3_ctx ctx;
    uint8_t data[] = {0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef};

    crc_ieee_802_3_init(&ctx);
    crc_ieee_802_3_update(&ctx, data, 4);
    crc_ieee_802_3_update(&ctx, &data[4], 4);
    uint32_t crc = crc_ieee_802_3_final(&ctx);

    TEST_ASSERT_EQUAL_HEX32(0x32692d55, crc);
}

/**
 * Test case 5 IEEE 802.3 CRC
 * 
 * Description:
 * - This test ensures that the IEEE 802.3 CRC algorithm produces the 
 *   expected CRC.
 * 
 * Steps:
 * - Initialize the CRC context with crc_ieee_802_3_init.
 * - Call crc_ieee_802_3_update with input bytes.
 * - Finalize the CRC with crc_ieee_802_3_final.
 *  
 * Expected result:
 * - Check if the result is correct.
 */
void test_crc_ieee_802_3_case_5(void)
{
    struct crc_ieee_802_3_ctx ctx;
    uint8_t data[] = {0xff, 0xff, 0xff, 0xff};

    crc_ieee_802_3_init(&ctx);
    crc_ieee_802_3_update(&ctx, data, sizeof(data));
    uint32_t crc = crc_ieee_802_3_final(&ctx);

    TEST_ASSERT_EQUAL_HEX32(0xffffffff, crc);
}

/**
 * Test case 1 IEEE 802.3 CRC (fast)
 * 
 * Description:
 * - This test ensures that the IEEE 802.3 CRC algorithm produces the 
 *   expected CRC.
 * 
 * Steps:
 * - Initialize the CRC context with crc_ieee_802_3_init.
 * - Call crc_ieee_802_3_update_fast with input bytes.
 * - Finalize the CRC with crc_ieee_802_3_final.
 *  
 * Expected result:
 * - Check if the result is correct.
 */
void test_crc_ieee_802_3_case_1_fast(void)
{
    struct crc_ieee_802_3_ctx ctx;

    crc_ieee_802_3_init(&ctx);
    crc_ieee_802_3_update_fast(&ctx, "", 0);
    uint32_t crc = crc_ieee_802_3_final(&ctx);

    TEST_ASSERT_EQUAL_HEX32(0x00000000, crc);
}

/**
 * Test case 2 IEEE 802.3 CRC (fast)
 * 
 * Description:
 * - This test ensures that the IEEE 802.3 CRC algorithm produces the 
 *   expected CRC.
 * 
 * Steps:
 * - Initialize the CRC context with crc_ieee_802_3_init.
 * - Call crc_ieee_802_3_update_fast with input bytes.
 * - Finalize the CRC with crc_ieee_802_3_final.
 *  
 * Expected result:
 * - Check if the result is correct.
 */
void test_crc_ieee_802_3_case_2_fast(void)
{
    struct crc_ieee_802_3_ctx ctx;
    uint8_t data = 0x01;

    crc_ieee_802_3_init(&ctx);
    crc_ieee_802_3_update_fast(&ctx, &data, sizeof(data));
    uint32_t crc = crc_ieee_802_3_final(&ctx);

    TEST_ASSERT_EQUAL_HEX32(0xa505df1b, crc);
}

/**
 * Test case 3 IEEE 802.3 CRC (fast)
 * 
 * Description:
 * - This test ensures that the IEEE 802.3 CRC algorithm produces the 
 *   expected CRC.
 * 
 * Steps:
 * - Initialize the CRC context with crc_ieee_802_3_init.
 * - Call crc_ieee_802_3_update_fast with input bytes.
 * - Finalize the CRC with crc_ieee_802_3_final.
 *  
 * Expected result:
 * - Check if the result is correct.
 */
void test_crc_ieee_802_3_case_3_fast(void)
{
    struct crc_ieee_802_3_ctx ctx;
    uint8_t data[] = {0xde, 0xad, 0xbe, 0xef};

    crc_ieee_802_3_init(&ctx);
    crc_ieee_802_3_update_fast(&ctx, data, sizeof(data));
    uint32_t crc = crc_ieee_802_3_final(&ctx);

    TEST_ASSERT_EQUAL_HEX32(0x7c9ca35a, crc);
}

/**
 * Test case 4 IEEE 802.3 CRC (fast)
 * 
 * Description:
 * - This test ensures that the IEEE 802.3 CRC algorithm produces the 
 *   expected CRC.
 * 
 * Steps:
 * - Initialize the CRC context with crc_ieee_802_3_init.
 * - Call crc_ieee_802_3_update_fast with input bytes.
 * - Finalize the CRC with crc_ieee_802_3_final.
 *  
 * Expected result:
 * - Check if the result is correct.
 */
void test_crc_ieee_802_3_case_4_fast(void)
{
    struct crc_ieee_802_3_ctx ctx;
    uint8_t data[] = {0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef};

    crc_ieee_802_3_init(&ctx);
    crc_ieee_802_3_update_fast(&ctx, data, 4);
    crc_ieee_802_3_update_fast(&ctx, &data[4], 4);
    uint32_t crc = crc_ieee_802_3_final(&ctx);

    TEST_ASSERT_EQUAL_HEX32(0x32692d55, crc);
}

/**
 * Test case 5 IEEE 802.3 CRC (fast)
 * 
 * Description:
 * - This test ensures that the IEEE 802.3 CRC algorithm produces the 
 *   expected CRC.
 * 
 * Steps:
 * - Initialize the CRC context with crc_ieee_802_3_init.
 * - Call crc_ieee_802_3_update_fast with input bytes.
 * - Finalize the CRC with crc_ieee_802_3_final.
 *  
 * Expected result:
 * - Check if the result is correct.
 */
void test_crc_ieee_802_3_case_5_fast(void)
{
    struct crc_ieee_802_3_ctx ctx;
    uint8_t data[] = {0xff, 0xff, 0xff, 0xff};

    crc_ieee_802_3_init(&ctx);
    crc_ieee_802_3_update_fast(&ctx, data, sizeof(data));
    uint32_t crc = crc_ieee_802_3_final(&ctx);

    TEST_ASSERT_EQUAL_HEX32(0xffffffff, crc);
}

/**
 * Test case 1 Modbus
 * 
 * Description:
 * - This test ensures that the Modbus CRC algorithm produces the 
 *   expected CRC.
 * 
 * Steps:
 * - Initialize the CRC context with crc_modbus_init.
 * - Call crc_modbus_update with input bytes.
 * - Finalize the CRC with crc_modbus_final.
 *  
 * Expected result:
 * - Check if the result is correct.
 */
void test_crc_modbus_case_1(void)
{
    struct crc_modbus_ctx ctx;

    crc_modbus_init(&ctx);
    crc_modbus_update(&ctx, "", 0);
    uint32_t crc = crc_modbus_final(&ctx);

    TEST_ASSERT_EQUAL_HEX32(0xffff, crc);
}

/**
 * Test case 2 Modbus CRC
 * 
 * Description:
 * - This test ensures that the Modbus CRC algorithm produces the 
 *   expected CRC.
 * 
 * Steps:
 * - Initialize the CRC context with crc_modbus_init.
 * - Call crc_modbus_update with input bytes.
 * - Finalize the CRC with crc_modbus_final.
 *  
 * Expected result:
 * - Check if the result is correct.
 */
void test_crc_modbus_case_2(void)
{
    struct crc_modbus_ctx ctx;
    uint8_t data = 0x01;

    crc_modbus_init(&ctx);
    crc_modbus_update(&ctx, &data, sizeof(data));
    uint32_t crc = crc_modbus_final(&ctx);

    TEST_ASSERT_EQUAL_HEX32(0x807e, crc);
}

/**
 * Test case 3 Modbus CRC
 * 
 * Description:
 * - This test ensures that the Modbus CRC algorithm produces the 
 *   expected CRC.
 * 
 * Steps:
 * - Initialize the CRC context with crc_modbus_init.
 * - Call crc_modbus_update with input bytes.
 * - Finalize the CRC with crc_modbus_final.
 *  
 * Expected result:
 * - Check if the result is correct.
 */
void test_crc_modbus_case_3(void)
{
    struct crc_modbus_ctx ctx;
    uint8_t data[] = {0xde, 0xad, 0xbe, 0xef};

    crc_modbus_init(&ctx);
    crc_modbus_update(&ctx, data, sizeof(data));
    uint32_t crc = crc_modbus_final(&ctx);

    TEST_ASSERT_EQUAL_HEX32(0xc19b, crc);
}

/**
 * Test case 4 Modbus CRC
 * 
 * Description:
 * - This test ensures that the Modbus CRC algorithm produces the 
 *   expected CRC.
 * 
 * Steps:
 * - Initialize the CRC context with crc_modbus_init.
 * - Call crc_modbus_update with input bytes.
 * - Finalize the CRC with crc_modbus_final.
 *  
 * Expected result:
 * - Check if the result is correct.
 */
void test_crc_modbus_case_4(void)
{
    struct crc_modbus_ctx ctx;
    uint8_t data[] = {0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef};

    crc_modbus_init(&ctx);
    crc_modbus_update(&ctx, data, 4);
    crc_modbus_update(&ctx, &data[4], 4);
    uint32_t crc = crc_modbus_final(&ctx);

    TEST_ASSERT_EQUAL_HEX32(0xdd50, crc);
}

/**
 * Test case 5 Modbus CRC
 * 
 * Description:
 * - This test ensures that the Modbus CRC algorithm produces the 
 *   expected CRC.
 * 
 * Steps:
 * - Initialize the CRC context with crc_modbus_init.
 * - Call crc_modbus_update with input bytes.
 * - Finalize the CRC with crc_modbus_final.
 *  
 * Expected result:
 * - Check if the result is correct.
 */
void test_crc_modbus_case_5(void)
{
    struct crc_modbus_ctx ctx;
    uint8_t data[] = {0xff, 0xff, 0xff, 0xff};

    crc_modbus_init(&ctx);
    crc_modbus_update(&ctx, data, sizeof(data));
    uint32_t crc = crc_modbus_final(&ctx);

    TEST_ASSERT_EQUAL_HEX32(0xb001, crc);
}

/**
 * Test case 1 Modbus (fast)
 * 
 * Description:
 * - This test ensures that the Modbus CRC algorithm produces the 
 *   expected CRC.
 * 
 * Steps:
 * - Initialize the CRC context with crc_modbus_init.
 * - Call crc_modbus_update_fast with input bytes.
 * - Finalize the CRC with crc_modbus_final.
 *  
 * Expected result:
 * - Check if the result is correct.
 */
void test_crc_modbus_case_1_fast(void)
{
    struct crc_modbus_ctx ctx;

    crc_modbus_init(&ctx);
    crc_modbus_update_fast(&ctx, "", 0);
    uint32_t crc = crc_modbus_final(&ctx);

    TEST_ASSERT_EQUAL_HEX32(0xffff, crc);
}

/**
 * Test case 2 Modbus CRC (fast)
 * 
 * Description:
 * - This test ensures that the Modbus CRC algorithm produces the 
 *   expected CRC.
 * 
 * Steps:
 * - Initialize the CRC context with crc_modbus_init.
 * - Call crc_modbus_update_fast with input bytes.
 * - Finalize the CRC with crc_modbus_final.
 *  
 * Expected result:
 * - Check if the result is correct.
 */
void test_crc_modbus_case_2_fast(void)
{
    struct crc_modbus_ctx ctx;
    uint8_t data = 0x01;

    crc_modbus_init(&ctx);
    crc_modbus_update_fast(&ctx, &data, sizeof(data));
    uint32_t crc = crc_modbus_final(&ctx);

    TEST_ASSERT_EQUAL_HEX32(0x807e, crc);
}

/**
 * Test case 3 Modbus CRC (fast)
 * 
 * Description:
 * - This test ensures that the Modbus CRC algorithm produces the 
 *   expected CRC.
 * 
 * Steps:
 * - Initialize the CRC context with crc_modbus_init.
 * - Call crc_modbus_update_fast with input bytes.
 * - Finalize the CRC with crc_modbus_final.
 *  
 * Expected result:
 * - Check if the result is correct.
 */
void test_crc_modbus_case_3_fast(void)
{
    struct crc_modbus_ctx ctx;
    uint8_t data[] = {0xde, 0xad, 0xbe, 0xef};

    crc_modbus_init(&ctx);
    crc_modbus_update_fast(&ctx, data, sizeof(data));
    uint32_t crc = crc_modbus_final(&ctx);

    TEST_ASSERT_EQUAL_HEX32(0xc19b, crc);
}

/**
 * Test case 4 Modbus CRC (fast)
 * 
 * Description:
 * - This test ensures that the Modbus CRC algorithm produces the 
 *   expected CRC.
 * 
 * Steps:
 * - Initialize the CRC context with crc_modbus_init.
 * - Call crc_modbus_update_fast with input bytes.
 * - Finalize the CRC with crc_modbus_final.
 *  
 * Expected result:
 * - Check if the result is correct.
 */
void test_crc_modbus_case_4_fast(void)
{
    struct crc_modbus_ctx ctx;
    uint8_t data[] = {0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef};

    crc_modbus_init(&ctx);
    crc_modbus_update_fast(&ctx, data, 4);
    crc_modbus_update_fast(&ctx, &data[4], 4);
    uint32_t crc = crc_modbus_final(&ctx);

    TEST_ASSERT_EQUAL_HEX32(0xdd50, crc);
}

/**
 * Test case 5 Modbus CRC (fast)
 * 
 * Description:
 * - This test ensures that the Modbus CRC algorithm produces the 
 *   expected CRC.
 * 
 * Steps:
 * - Initialize the CRC context with crc_modbus_init.
 * - Call crc_modbus_update_fast with input bytes.
 * - Finalize the CRC with crc_modbus_final.
 *  
 * Expected result:
 * - Check if the result is correct.
 */
void test_crc_modbus_case_5_fast(void)
{
    struct crc_modbus_ctx ctx;
    uint8_t data[] = {0xff, 0xff, 0xff, 0xff};

    crc_modbus_init(&ctx);
    crc_modbus_update_fast(&ctx, data, sizeof(data));
    uint32_t crc = crc_modbus_final(&ctx);

    TEST_ASSERT_EQUAL_HEX32(0xb001, crc);
}
