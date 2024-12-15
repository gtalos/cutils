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

void bit_cpy(void *dst, size_t dst_offset, const void* src, size_t src_offset, size_t bit_len);

uint32_t bit_reverse_u32(uint32_t value);

uint16_t bit_reverse_u16(uint16_t value);

bool is_little_endian(void);

#ifdef __cplusplus
}
#endif

#endif 