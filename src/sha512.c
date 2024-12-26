/*
 * Copyright (c) The Mbed TLS Contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "sha512.h"

#include <string.h>


static uint64_t reverse_u64(uint64_t value);
static void write_u64_be(uint64_t value, void *data, size_t offset);
static uint64_t read_u64_be(const void *data, size_t offset);
static size_t internal_process_many(struct sha512_ctx *ctx, const void *data, size_t len);
static void internal_process(struct sha512_ctx *ctx, const void *data);


static const uint64_t K[] = {
    UINT64_C(0x428a2f98d728ae22), UINT64_C(0x7137449123ef65cd),
    UINT64_C(0xb5c0fbcfec4d3b2f), UINT64_C(0xe9b5dba58189dbbc),
    UINT64_C(0x3956c25bf348b538), UINT64_C(0x59f111f1b605d019),
    UINT64_C(0x923f82a4af194f9b), UINT64_C(0xab1c5ed5da6d8118),
    UINT64_C(0xd807aa98a3030242), UINT64_C(0x12835b0145706fbe),
    UINT64_C(0x243185be4ee4b28c), UINT64_C(0x550c7dc3d5ffb4e2),
    UINT64_C(0x72be5d74f27b896f), UINT64_C(0x80deb1fe3b1696b1),
    UINT64_C(0x9bdc06a725c71235), UINT64_C(0xc19bf174cf692694),
    UINT64_C(0xe49b69c19ef14ad2), UINT64_C(0xefbe4786384f25e3),
    UINT64_C(0x0fc19dc68b8cd5b5), UINT64_C(0x240ca1cc77ac9c65),
    UINT64_C(0x2de92c6f592b0275), UINT64_C(0x4a7484aa6ea6e483),
    UINT64_C(0x5cb0a9dcbd41fbd4), UINT64_C(0x76f988da831153b5),
    UINT64_C(0x983e5152ee66dfab), UINT64_C(0xa831c66d2db43210),
    UINT64_C(0xb00327c898fb213f), UINT64_C(0xbf597fc7beef0ee4),
    UINT64_C(0xc6e00bf33da88fc2), UINT64_C(0xd5a79147930aa725),
    UINT64_C(0x06ca6351e003826f), UINT64_C(0x142929670a0e6e70),
    UINT64_C(0x27b70a8546d22ffc), UINT64_C(0x2e1b21385c26c926),
    UINT64_C(0x4d2c6dfc5ac42aed), UINT64_C(0x53380d139d95b3df),
    UINT64_C(0x650a73548baf63de), UINT64_C(0x766a0abb3c77b2a8),
    UINT64_C(0x81c2c92e47edaee6), UINT64_C(0x92722c851482353b),
    UINT64_C(0xa2bfe8a14cf10364), UINT64_C(0xa81a664bbc423001),
    UINT64_C(0xc24b8b70d0f89791), UINT64_C(0xc76c51a30654be30),
    UINT64_C(0xd192e819d6ef5218), UINT64_C(0xd69906245565a910),
    UINT64_C(0xf40e35855771202a), UINT64_C(0x106aa07032bbd1b8),
    UINT64_C(0x19a4c116b8d2d0c8), UINT64_C(0x1e376c085141ab53),
    UINT64_C(0x2748774cdf8eeb99), UINT64_C(0x34b0bcb5e19b48a8),
    UINT64_C(0x391c0cb3c5c95a63), UINT64_C(0x4ed8aa4ae3418acb),
    UINT64_C(0x5b9cca4f7763e373), UINT64_C(0x682e6ff3d6b2b8a3),
    UINT64_C(0x748f82ee5defb2fc), UINT64_C(0x78a5636f43172f60),
    UINT64_C(0x84c87814a1f0ab72), UINT64_C(0x8cc702081a6439ec),
    UINT64_C(0x90befffa23631e28), UINT64_C(0xa4506cebde82bde9),
    UINT64_C(0xbef9a3f7b2c67915), UINT64_C(0xc67178f2e372532b),
    UINT64_C(0xca273eceea26619c), UINT64_C(0xd186b8c721c0c207),
    UINT64_C(0xeada7dd6cde0eb1e), UINT64_C(0xf57d4f7fee6ed178),
    UINT64_C(0x06f067aa72176fba), UINT64_C(0x0a637dc5a2c898a6),
    UINT64_C(0x113f9804bef90dae), UINT64_C(0x1b710b35131c471b),
    UINT64_C(0x28db77f523047d84), UINT64_C(0x32caab7b40c72493),
    UINT64_C(0x3c9ebe0a15c9bebc), UINT64_C(0x431d67c49c100d4c),
    UINT64_C(0x4cc5d4becb3e42b6), UINT64_C(0x597f299cfc657e2a),
    UINT64_C(0x5fcb6fab3ad6faec), UINT64_C(0x6c44198c4a475817)
};


void sha512_init(struct sha512_ctx *ctx)
{
    ctx->total[0] = 0;
    ctx->total[1] = 0;

    ctx->state[0] = UINT64_C(0x6a09e667f3bcc908);
    ctx->state[1] = UINT64_C(0xbb67ae8584caa73b);
    ctx->state[2] = UINT64_C(0x3c6ef372fe94f82b);
    ctx->state[3] = UINT64_C(0xa54ff53a5f1d36f1);
    ctx->state[4] = UINT64_C(0x510e527fade682d1);
    ctx->state[5] = UINT64_C(0x9b05688c2b3e6c1f);
    ctx->state[6] = UINT64_C(0x1f83d9abfb41bd6b);
    ctx->state[7] = UINT64_C(0x5be0cd19137e2179);

    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

void sha512_update(struct sha512_ctx *ctx, const void *input, size_t len)
{
    if (len == 0) {
        return;
    }

    uint32_t left = (uint32_t) (ctx->total[0] & 0x7f);
    size_t fill = 128 - left;

    ctx->total[0] += (uint64_t) len;

    if (ctx->total[0] < (uint64_t) len) {
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

    while (len >= 128) {
        size_t processed = internal_process_many(ctx, in, len);

        in += processed;
        len -= processed;
    }

    if (len > 0) {
        memcpy(ctx->buffer + left, in, len);
    }
}

void sha512_final(struct sha512_ctx *ctx, void *output)
{
    /* Add padding: 0x80 then 0x00 until 16 bytes remain for the length. */
    uint32_t used = ctx->total[0] & 0x7f;

    ctx->buffer[used++] = 0x80;

    if (used <= 112) {
        /* Enough room for padding + length in current block */
        memset(ctx->buffer + used, 0, 112 - used);
    } else {
        /* We'll need an extra block */
        memset(ctx->buffer + used, 0, 128 - used);

        internal_process(ctx, ctx->buffer);

        memset(ctx->buffer, 0, 112);
    }

    /* Add message length */
    const uint64_t high = (ctx->total[0] >> 61) | (ctx->total[1] << 3);
    const uint64_t low = ctx->total[0] << 3;

    write_u64_be(high, ctx->buffer, 112);
    write_u64_be(low,  ctx->buffer, 120);

    internal_process(ctx, ctx->buffer);

    /* Output final state */
    write_u64_be(ctx->state[0], output,  0);
    write_u64_be(ctx->state[1], output,  8);
    write_u64_be(ctx->state[2], output, 16);
    write_u64_be(ctx->state[3], output, 24);
    write_u64_be(ctx->state[4], output, 32);
    write_u64_be(ctx->state[5], output, 40);
    write_u64_be(ctx->state[6], output, 48);
    write_u64_be(ctx->state[7], output, 56);
}

static uint64_t reverse_u64(uint64_t value)
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

static void write_u64_be(uint64_t value, void *data, size_t offset)
{
    uint64_t to_write;
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    to_write = value;
    (void) reverse_u64;
#else
    to_write = reverse_u64(value);
#endif
    memcpy((uint8_t *) data + offset, &to_write, sizeof(to_write));
}

static uint64_t read_u64_be(const void *data, size_t offset)
{
    uint64_t n;
    memcpy(&n, (uint8_t *) data + offset, sizeof(n));

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    (void) reverse_u64;
    return n;
#else
    return reverse_u64(n);
#endif
}

static size_t internal_process_many(struct sha512_ctx *ctx, const void *data, size_t len)
{
    size_t processed = 0;
    const uint8_t *d = data;

    while (len >= 128) {
        internal_process(ctx, d);
        d += 128;
        len -= 128;

        processed += 128;
    }

    return processed;
}

static void internal_process(struct sha512_ctx *ctx, const void *data)
{
    struct {
        uint64_t temp1;
        uint64_t temp2;
        uint64_t W[80];
        uint64_t A[8];
    } local;

#define  SHR(x, n) ((x) >> (n))
#define ROTR(x, n) (SHR((x), (n)) | ((x) << (64 - (n))))

#define S0(x) (ROTR(x, 1) ^ ROTR(x, 8) ^  SHR(x, 7))
#define S1(x) (ROTR(x, 19) ^ ROTR(x, 61) ^  SHR(x, 6))

#define S2(x) (ROTR(x, 28) ^ ROTR(x, 34) ^ ROTR(x, 39))
#define S3(x) (ROTR(x, 14) ^ ROTR(x, 18) ^ ROTR(x, 41))

#define F0(x, y, z) (((x) & (y)) | ((z) & ((x) | (y))))
#define F1(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))

#define P(a, b, c, d, e, f, g, h, x, K)                                 \
        do {                                                            \
            local.temp1 = (h) + S3(e) + F1((e), (f), (g)) + (K) + (x);  \
            local.temp2 = S2(a) + F0((a), (b), (c));                    \
            (d) += local.temp1; (h) = local.temp1 + local.temp2;        \
        } while (0)

    size_t i;

    for (i = 0; i < 8; i++) {
        local.A[i] = ctx->state[i];
    }

    for (i = 0; i < 16; i++) {
        local.W[i] = read_u64_be(data, i << 3);
    }

    for (; i < 80; i++) {
        local.W[i] = S1(local.W[i -  2]) + local.W[i -  7] +
                     S0(local.W[i - 15]) + local.W[i - 16];
    }

    i = 0;
    do {
        P(local.A[0], local.A[1], local.A[2], local.A[3], local.A[4],
          local.A[5], local.A[6], local.A[7], local.W[i], K[i]);
          i++;
        P(local.A[7], local.A[0], local.A[1], local.A[2], local.A[3],
          local.A[4], local.A[5], local.A[6], local.W[i], K[i]);
          i++;
        P(local.A[6], local.A[7], local.A[0], local.A[1], local.A[2],
          local.A[3], local.A[4], local.A[5], local.W[i], K[i]);
          i++;
        P(local.A[5], local.A[6], local.A[7], local.A[0], local.A[1],
          local.A[2], local.A[3], local.A[4], local.W[i], K[i]);
          i++;
        P(local.A[4], local.A[5], local.A[6], local.A[7], local.A[0],
          local.A[1], local.A[2], local.A[3], local.W[i], K[i]);
          i++;
        P(local.A[3], local.A[4], local.A[5], local.A[6], local.A[7],
          local.A[0], local.A[1], local.A[2], local.W[i], K[i]);
          i++;
        P(local.A[2], local.A[3], local.A[4], local.A[5], local.A[6],
          local.A[7], local.A[0], local.A[1], local.W[i], K[i]);
          i++;
        P(local.A[1], local.A[2], local.A[3], local.A[4], local.A[5],
          local.A[6], local.A[7], local.A[0], local.W[i], K[i]);
          i++;
    } while (i < 80);

    for (i = 0; i < 8; i++) {
        ctx->state[i] += local.A[i];
    }
}
