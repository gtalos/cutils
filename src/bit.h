/*
 * Copyright (c) 2024 gtalos
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef BIT_H
#define BIT_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Copies a specified number of bits from the source buffer to the destination buffer.
 * 
 * This function allows bitwise copying of bits from one memory region to another, 
 * starting from specified bit offsets within the source and destination buffers.
 *
 * @param dst Pointer to the destination buffer.
 * @param dst_offset Bit offset in the destination buffer where copying begins.
 * @param src Pointer to the source buffer.
 * @param src_offset Bit offset in the source buffer where copying starts.
 * @param bit_len Number of bits to copy.
 */
void bit_copy(void *dst, size_t dst_offset, const void* src, size_t src_offset, size_t bit_len);

/**
 * @brief Toggles the endianess of a 16-bit unsigned integer.
 * 
 * This function swaps the byte order of the given 16-bit value. The lower byte and 
 * upper byte are exchanged.
 *
 * @param value 16-bit unsigned integer to toggle endianess.
 * @return The 16-bit value with its byte order swapped.
 */
uint16_t bit_toggle_endian_u16(uint16_t value);

/**
 * @brief Toggles the endianess of a 32-bit unsigned integer.
 * 
 * This function swaps the byte order of the given 32-bit value. The 4 bytes are 
 * rearranged in reverse order.
 *
 * @param value 32-bit unsigned integer to toggle endianess.
 * @return The 32-bit value with its byte order swapped.
 */
uint32_t bit_toggle_endian_u32(uint32_t value);

/**
 * @brief Toggles the endianess of a 64-bit unsigned integer.
 * 
 * This function swaps the byte order of the given 64-bit value. The 8 bytes are 
 * rearranged in reverse order.
 *
 * @param value 64-bit unsigned integer to toggle endianess.
 * @return The 64-bit value with its byte order swapped.
 */
uint64_t bit_toggle_endian_u64(uint64_t value);

/**
 * @brief Reverses the bit order of a 16-bit unsigned integer.
 * 
 * This function performs a bitwise reversal of a 16-bit value, inverting the bit positions.
 *
 * @param value 16-bit unsigned integer whose bits will be reversed.
 * @return The 16-bit value with its bits reversed.
 */
uint16_t bit_reverse_u16(uint16_t value);

/**
 * @brief Reverses the bit order of a 32-bit unsigned integer.
 * 
 * This function performs a bitwise reversal of a 32-bit value, inverting the bit positions.
 *
 * @param value 32-bit unsigned integer whose bits will be reversed.
 * @return The 32-bit value with its bits reversed.
 */
uint32_t bit_reverse_u32(uint32_t value);

/**
 * @brief Reverses the bit order of a 64-bit unsigned integer.
 * 
 * This function performs a bitwise reversal of a 64-bit value, inverting the bit positions.
 *
 * @param value 64-bit unsigned integer whose bits will be reversed.
 * @return The 64-bit value with its bits reversed.
 */
uint64_t bit_reverse_u64(uint64_t value);

/**
 * @brief Checks if the system is little-endian.
 * 
 * This function tests the byte order of the system by checking how a 32-bit value 
 * is stored in memory.
 *
 * @return `true` if the system is little-endian, `false` if it is big-endian.
 */
bool bit_is_little_endian(void);

/**
 * @brief Checks if the system is big-endian.
 * 
 * This function tests the byte order of the system by checking how a 32-bit value 
 * is stored in memory.
 *
 * @return `true` if the system is big-endian, `false` if it is little-endian.
 */
bool bit_is_big_endian(void);

#ifdef __cplusplus
}
#endif

#endif 