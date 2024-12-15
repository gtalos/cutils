/*
 * Copyright (c) 2024 gtalos
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "bit.h"


void bitcpy(void *dst, size_t dst_offset, const void* src, size_t src_offset, size_t bit_len)
{
	uint8_t *d = dst;
	const uint8_t *s = src;

	for (size_t i = 0; i < bit_len; i++) {
		/* Calculate the source and destination bytes we are working on. */
		size_t src_byte = (src_offset + i) / 8;
		size_t dst_byte = (dst_offset + i) / 8;

		/* Calculate the bit offsets. */
		size_t src_bit = (src_offset + i) % 8;
		size_t dst_bit = (dst_offset + i) % 8;

		/* Calculate the state of the specified source bit. */
		uint32_t src_state = s[src_byte] & (1u << (7u - src_bit));

		if (src_state != 0) {
			/* The src bit was set, set the corresponding destination bit. */
			d[dst_byte] |= (1u << (7u - dst_bit));
		} else {
			/* The src bit was off, turn off the corresponding destination bit. */
			d[dst_byte] &= ~(1u << (7u - dst_bit));
		}
	}
}

uint32_t bit_reverse_u32(uint32_t value)
{
    return ((value & 0x000000ff) << 24) | 
           ((value & 0x0000ff00) << 8)  | 
           ((value & 0x00ff0000) >> 8)  | 
           ((value & 0xff000000) >> 24);
}

uint16_t bit_reverse_u16(uint16_t value)
{
    return ((value & 0x00ff) << 8) | 
           ((value & 0xff00) >> 8);
}

bool is_little_endian(void)
{
    uint32_t test_value = 0x1;
    uint8_t *byte_ptr = (uint8_t *) &test_value;

    return *byte_ptr == 0x1;
}