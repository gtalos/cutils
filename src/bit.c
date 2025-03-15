/*
 * Copyright (c) 2024
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "bit.h"


void bit_copy(void *dst, size_t dst_offset, const void* src, size_t src_offset, size_t bit_len)
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

uint16_t bit_toggle_endian_u16(uint16_t value)
{   
    return ((value & 0x00ff) << 8) |
           ((value & 0xff00) >> 8);
}

uint32_t bit_toggle_endian_u32(uint32_t value)
{
    return ((value & 0x000000ff) << 24) |
           ((value & 0x0000ff00) <<  8) |
           ((value & 0x00ff0000) >>  8) |
           ((value & 0xff000000) >> 24);
}

uint64_t bit_toggle_endian_u64(uint64_t value)
{
    return ((value & UINT64_C(0x00000000000000ff)) << 56) |
           ((value & UINT64_C(0x000000000000ff00)) << 40) |
           ((value & UINT64_C(0x0000000000ff0000)) << 24) |
           ((value & UINT64_C(0x00000000ff000000)) <<  8) |
           ((value & UINT64_C(0x000000ff00000000)) >>  8) |
           ((value & UINT64_C(0x0000ff0000000000)) >> 24) |
           ((value & UINT64_C(0x00ff000000000000)) >> 40) |
           ((value & UINT64_C(0xff00000000000000)) >> 56);
}

uint16_t bit_reverse_u16(uint16_t value)
{
    value = ((value & 0x5555) << 1) | ((value & 0xaaaa) >> 1);
    value = ((value & 0x3333) << 2) | ((value & 0xcccc) >> 2);
    value = ((value & 0x0f0f) << 4) | ((value & 0xf0f0) >> 4);
    value = (value << 8) | (value >> 8);
    
    return value;
}

uint32_t bit_reverse_u32(uint32_t value)
{
    value = ((value & 0x55555555) << 1) | ((value & 0xaaaaaaaa) >> 1);
    value = ((value & 0x33333333) << 2) | ((value & 0xcccccccc) >> 2);
    value = ((value & 0x0f0f0f0f) << 4) | ((value & 0xf0f0f0f0) >> 4);
    value = ((value & 0x00ff00ff) << 8) | ((value & 0xff00ff00) >> 8);
    value = (value << 16) | (value >> 16);

    return value;
}

uint64_t bit_reverse_u64(uint64_t value)
{
    value = ((value & UINT64_C(0x5555555555555555)) <<  1) |
            ((value & UINT64_C(0xaaaaaaaaaaaaaaaa)) >>  1);
    value = ((value & UINT64_C(0x3333333333333333)) <<  2) |
            ((value & UINT64_C(0xcccccccccccccccc)) >>  2);
    value = ((value & UINT64_C(0x0f0f0f0f0f0f0f0f)) <<  4) |
            ((value & UINT64_C(0xf0f0f0f0f0f0f0f0)) >>  4);
    value = ((value & UINT64_C(0x00ff00ff00ff00ff)) <<  8) |
            ((value & UINT64_C(0xff00ff00ff00ff00)) >>  8);
    value = ((value & UINT64_C(0x0000ffff0000ffff)) << 16) |
            ((value & UINT64_C(0xffff0000ffff0000)) >> 16);
    value = (value << 32) | (value >> 32);

    return value;
}

bool bit_is_little_endian(void)
{
    uint32_t test_value = 0x1;
    uint8_t *byte_ptr = (uint8_t *) &test_value;

    return byte_ptr[0] == 0x01 && 
           byte_ptr[1] == 0x00 && 
           byte_ptr[2] == 0x00 && 
           byte_ptr[3] == 0x00;
}

bool bit_is_big_endian(void)
{
    uint32_t test_value = 0x1;
    uint8_t *byte_ptr = (uint8_t *) &test_value;

    return byte_ptr[0] == 0x00 && 
           byte_ptr[1] == 0x00 && 
           byte_ptr[2] == 0x00 && 
           byte_ptr[3] == 0x01;
}
