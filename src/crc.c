/*
 * Copyright (c) 2024 gtalos
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "crc.h"


void crc_ieee_802_3_init(struct crc_ieee_802_3_ctx *ctx)
{
    ctx->state = 0xffffffff;
}

void crc_ieee_802_3_update(struct crc_ieee_802_3_ctx *ctx, const void *data, size_t len)
{
    const uint8_t *d = data;

    while(len--) {
        ctx->state ^= *d++;
        for(uint32_t bit = 0; bit < 8; bit++) {
            if ((ctx->state & 1u) != 0) {
                ctx->state = (ctx->state >> 1u) ^ 0xedb88320;
            } else {
                ctx->state >>= 1u;
            }
        }
    }
}

uint32_t crc_ieee_802_3_final(struct crc_ieee_802_3_ctx *ctx)
{
    return ~ctx->state;
}

void crc_modbus_init(struct crc_modbus_ctx *ctx)
{
    ctx->state = 0xffff;
}

void crc_modbus_update(struct crc_modbus_ctx *ctx, const void *data, size_t len)
{
    const uint8_t *d = data;

    for (size_t i = 0; i < len; i++) {
        ctx->state ^= d[i];
        for (size_t j = 0; j < 8; j++) {
            if ((ctx->state & 0x0001) != 0) {
                ctx->state = (ctx->state >> 1) ^ 0xa001;
            } else {
                ctx->state >>= 1;
            }
        }
    }
}

uint32_t crc_modbus_finish(struct crc_modbus_ctx *ctx)
{
    return ctx->state;
}
