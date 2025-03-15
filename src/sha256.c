/*
 * Copyright (c) The Mbed TLS Contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */
 
#include "sha256.h"
#include "endian.h"

#include <string.h>


static uint32_t reverse_u32(uint32_t value);
static void write_u32_be(uint32_t value, void *data, size_t offset);
static uint32_t read_u32_be(const void *data, size_t offset);
static void internal_process(struct sha256_ctx *ctx, const void *data);
static size_t internal_process_many(struct sha256_ctx *ctx, const uint8_t *data, size_t len);


static const uint32_t K[] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};


void sha256_init(struct sha256_ctx *ctx)
{
    ctx->total[0] = 0;
    ctx->total[1] = 0;

    ctx->state[0] = 0x6a09e667;
    ctx->state[1] = 0xbb67ae85;
    ctx->state[2] = 0x3c6ef372;
    ctx->state[3] = 0xa54ff53a;
    ctx->state[4] = 0x510e527f;
    ctx->state[5] = 0x9b05688c;
    ctx->state[6] = 0x1f83d9ab;
    ctx->state[7] = 0x5be0cd19;

    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

void sha256_update(struct sha256_ctx *ctx, const void *input, size_t len)
{
    if (len == 0) {
        return;
    }

    uint32_t left = ctx->total[0] & 0x3f;
    const size_t fill = 64 - left;

    ctx->total[0] += (uint32_t) len;

    if (ctx->total[0] < (uint32_t) len) {
        ctx->total[1]++;
    }

    const uint8_t *in = input;

    if (left != 0 && len >= fill) {
        memcpy(ctx->buffer + left, in, fill);

        internal_process(ctx, ctx->buffer);

        in += fill;
        len -= fill;
        left = 0;
    }

    while (len >= 64) {
        size_t processed = internal_process_many(ctx, in, len);

        in += processed;
        len -= processed;
    }

    if (len > 0) {
        memcpy(ctx->buffer + left, in, len);
    }
}

void sha256_final(struct sha256_ctx *ctx, void *output)
{
    /* Add padding: 0x80 then 0x00 until 8 bytes remain for the length */
    uint32_t used = ctx->total[0] & 0x3f;

    ctx->buffer[used++] = 0x80;

    if (used <= 56) {
        /* Enough room for padding + length in current block */
        memset(ctx->buffer + used, 0, 56 - used);
    } else {
        /* We'll need an extra block */
        memset(ctx->buffer + used, 0, 64 - used);

        internal_process(ctx, ctx->buffer);

        memset(ctx->buffer, 0, 56);
    }

    /* Add message length */
    const uint32_t high = (ctx->total[0] >> 29) | (ctx->total[1] << 3);
    const uint32_t low = ctx->total[0] << 3;

    write_u32_be(high, ctx->buffer, 56);
    write_u32_be(low,  ctx->buffer, 60);

    internal_process(ctx, ctx->buffer);

    /* Output final state */
    write_u32_be(ctx->state[0], output,  0);
    write_u32_be(ctx->state[1], output,  4);
    write_u32_be(ctx->state[2], output,  8);
    write_u32_be(ctx->state[3], output, 12);
    write_u32_be(ctx->state[4], output, 16);
    write_u32_be(ctx->state[5], output, 20);
    write_u32_be(ctx->state[6], output, 24);
    write_u32_be(ctx->state[7], output, 28);
}

static uint32_t reverse_u32(uint32_t value)
{
    return ((value & 0x000000ff) << 24) | 
           ((value & 0x0000ff00) <<  8) | 
           ((value & 0x00ff0000) >>  8) | 
           ((value & 0xff000000) >> 24);
}

static void write_u32_be(uint32_t value, void *data, size_t offset)
{
    uint32_t to_write;
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    to_write = value;
    (void) reverse_u32;
#else
    to_write = reverse_u32(value);
#endif
    memcpy((uint8_t *) data + offset, &to_write, sizeof(to_write));
}

static uint32_t read_u32_be(const void *data, size_t offset)
{
    uint32_t n;
    memcpy(&n, (uint8_t *) data + offset, sizeof(n));
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    (void) reverse_u32;
    return n;
#else
    return reverse_u32(n);
#endif
}

static void internal_process(struct sha256_ctx *ctx, const void *data)
{
    struct {
        uint32_t temp1; 
        uint32_t temp2; 
        uint32_t W[64];
        uint32_t A[8];
    } local;

#define  SHR(x, n) (((x) & 0xffffffff) >> (n))
#define ROTR(x, n) (SHR(x, n) | ((x) << (32 - (n))))

#define S0(x) (ROTR(x, 7) ^ ROTR(x, 18) ^  SHR(x, 3))
#define S1(x) (ROTR(x, 17) ^ ROTR(x, 19) ^  SHR(x, 10))

#define S2(x) (ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define S3(x) (ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25))

#define F0(x, y, z) (((x) & (y)) | ((z) & ((x) | (y))))
#define F1(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))

#define R(t)                                                            \
        (                                                               \
            local.W[t] = S1(local.W[(t) -  2]) + local.W[(t) -  7] +    \
            S0(local.W[(t) - 15]) + local.W[(t) - 16]                   \
        )

#define P(a, b, c, d, e, f, g, h, x, K)                                 \
        do {                                                            \
            local.temp1 = (h) + S3(e) + F1((e), (f), (g)) + (K) + (x);  \
            local.temp2 = S2(a) + F0((a), (b), (c));                    \
            (d) += local.temp1; (h) = local.temp1 + local.temp2;        \
        } while (0)

    for (size_t i = 0; i < 8; i++) {
        local.A[i] = ctx->state[i];
    }

    for (size_t i = 0; i < 64; i++) {
        if (i < 16) {
            local.W[i] = read_u32_be(data, 4 * i);
        } else {
            R(i);
        }

        P(local.A[0], local.A[1], local.A[2], local.A[3], local.A[4],
          local.A[5], local.A[6], local.A[7], local.W[i], K[i]);

        local.temp1 = local.A[7]; 
        local.A[7] = local.A[6];
        local.A[6] = local.A[5]; 
        local.A[5] = local.A[4];
        local.A[4] = local.A[3]; 
        local.A[3] = local.A[2];
        local.A[2] = local.A[1]; 
        local.A[1] = local.A[0];
        local.A[0] = local.temp1;
    }

    for (size_t i = 0; i < 8; i++) {
        ctx->state[i] += local.A[i];
    }
}

static size_t internal_process_many(struct sha256_ctx *ctx, const uint8_t *data, size_t len)
{
    size_t processed = 0;

    while (len >= 64) {
        internal_process(ctx, data);

        data += 64;
        len -= 64;
        processed += 64;
    }

    return processed;
}
