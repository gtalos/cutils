/*
 * Copyright (c) 2024
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CRC_H
#define CRC_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Context structure for IEEE 802.3 CRC calculations.
 */
struct crc_ieee_802_3_ctx {
	uint32_t state;
};

/**
 * @brief Context structure for Modbus CRC calculations.
 */
struct crc_modbus_ctx {
	uint32_t state;
};

/**
 * @brief Initializes the context for IEEE 802.3 CRC calculation.
 *
 * @param ctx Pointer to the IEEE 802.3 CRC context structure.
 */
void crc_ieee_802_3_init(struct crc_ieee_802_3_ctx *ctx);

/**
 * @brief Updates the IEEE 802.3 CRC calculation with new data.
 *
 * @param ctx Pointer to the IEEE 802.3 CRC context structure.
 * @param data Pointer to the data to process.
 * @param len Length of the data in bytes.
 */
void crc_ieee_802_3_update(struct crc_ieee_802_3_ctx *ctx, const void *data, size_t len);

/**
 * @brief Updates the IEEE 802.3 CRC calculation with new data.
 * 
 * This function differs from crc_ieee_802_3_update in that it 
 * is implemented based on a table. This improves speed at the 
 * cost of using more program memory.
 *
 * @param ctx Pointer to the IEEE 802.3 CRC context structure.
 * @param data Pointer to the data to process.
 * @param len Length of the data in bytes.
 */
void crc_ieee_802_3_update_fast(struct crc_ieee_802_3_ctx *ctx, const void *data, size_t len);

/**
 * @brief Finalizes the IEEE 802.3 CRC calculation.
 *
 * @param ctx Pointer to the IEEE 802.3 CRC context structure.
 * @return The final CRC value.
 */
uint32_t crc_ieee_802_3_final(struct crc_ieee_802_3_ctx *ctx);

/**
 * @brief Initializes the context for Modbus CRC calculation.
 *
 * @param ctx Pointer to the Modbus CRC context structure.
 */
void crc_modbus_init(struct crc_modbus_ctx *ctx);

/**
 * @brief Updates the Modbus CRC calculation with new data.
 *
 * @param ctx Pointer to the Modbus CRC context structure.
 * @param data Pointer to the data to process.
 * @param len Length of the data in bytes.
 */
void crc_modbus_update(struct crc_modbus_ctx *ctx, const void *data, size_t len);

/**
 * @brief Updates the Modbus CRC calculation with new data.
 *
 * This function differs from crc_modbus_update in that it 
 * is implemented based on a table. This improves speed at the 
 * cost of using more program memory.
 * 
 * @param ctx Pointer to the Modbus CRC context structure.
 * @param data Pointer to the data to process.
 * @param len Length of the data in bytes.
 */
void crc_modbus_update_fast(struct crc_modbus_ctx *ctx, const void *data, size_t len);

/**
 * @brief Finalizes the Modbus CRC calculation.
 *
 * @param ctx Pointer to the Modbus CRC context structure.
 * @return The final CRC value.
 */
uint32_t crc_modbus_final(struct crc_modbus_ctx *ctx);

#ifdef __cplusplus
}
#endif

#endif /* CRC_H */
