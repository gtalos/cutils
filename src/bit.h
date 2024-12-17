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

void bit_copy(void *dst, size_t dst_offset, const void* src, size_t src_offset, size_t bit_len);

uint16_t bit_toggle_endian_u16(uint16_t value);

uint32_t bit_toggle_endian_u32(uint32_t value);

uint64_t bit_toggle_endian_u64(uint64_t value);

uint16_t bit_reverse_u16(uint16_t value);

uint32_t bit_reverse_u32(uint32_t value);

uint64_t bit_reverse_u64(uint64_t value);

bool bit_is_little_endian(void);

#ifdef __cplusplus
}
#endif

#endif 