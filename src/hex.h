/*
 * Copyright (c) 2024
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef HEX_H
#define HEX_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Converts a binary buffer to a hexadecimal string.
 *
 * This function takes a binary input and converts it into a hexadecimal
 * representation. The output hex string is not null-terminated.
 *
 * @param bin Pointer to the binary data.
 * @param bin_len Length of the binary data.
 * @param upper If true, output hex is in uppercase; otherwise, lowercase.
 * @param hex Output buffer for the hex string. Must be at least
 *            (bin_len * 2) bytes long.
 */
void hex_from_bytes(const void *bin, size_t bin_len, bool upper, char *hex);

/**
 * @brief Converts a hexadecimal string to a binary buffer.
 *
 * This function decodes a hexadecimal string into its binary representation.
 *
 * @param hex Pointer to the hex string.
 * @param len Length of the hex string (must be even, excluding null terminator).
 * @param bin Output buffer for binary data. Must be at least (len / 2) bytes long.
 *
 * @return Number of bytes written to bin on success, or -1 on failure (e.g., invalid input).
 */
int32_t hex_to_bytes(const char *hex, size_t len, void *bin);

#ifdef __cplusplus
}
#endif

#endif /* HEX_H */
