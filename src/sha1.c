/*
 * Copyright (c) The Mbed TLS Contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "sha1.h"

#include <string.h>


static uint32_t reverse_u32(uint32_t value);
static void write_u32_be(uint32_t value, void *data, size_t offset);
static uint32_t read_u32_be(const void *data, size_t offset);
static void internal_process(struct sha1_ctx *ctx, const void *data);


void sha1_init(struct sha1_ctx *ctx)
{
    ctx->total[0] = 0;
    ctx->total[1] = 0;

    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xefcdab89;
    ctx->state[2] = 0x98badcfe;
    ctx->state[3] = 0x10325476;
    ctx->state[4] = 0xc3d2e1f0;

    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

void sha1_update(struct sha1_ctx *ctx, const void *input, size_t len)
{
    if (len == 0) {
        return;
    }

    size_t left = ctx->total[0] & 0x3f;
    const uint32_t fill = 64 - left;

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
        internal_process(ctx, in);

        in += 64;
        len -= 64;
    }

    if (len > 0) {
        memcpy(ctx->buffer + left, in, len);
    }
}

void sha1_final(struct sha1_ctx *ctx, void *output)
{
    /* Add padding: 0x80 then 0x00 until 8 bytes remain for the length. */
    uint32_t used = ctx->total[0] & 0x3f;

    ctx->buffer[used++] = 0x80;

    if (used <= 56) {
        /* Enough room for padding + length in current block. */
        memset(ctx->buffer + used, 0, 56 - used);
    } else {
        /* We'll need an extra block. */
        memset(ctx->buffer + used, 0, 64 - used);

        internal_process(ctx, ctx->buffer);

        memset(ctx->buffer, 0, 56);
    }

    /* Add message length. */
    const uint32_t high = (ctx->total[0] >> 29) | (ctx->total[1] << 3);
    const uint32_t low = ctx->total[0] << 3;

    write_u32_be(high, ctx->buffer, 56);
    write_u32_be(low,  ctx->buffer, 60);

    internal_process(ctx, ctx->buffer);

    /* Output final state. */
    write_u32_be(ctx->state[0], output,  0);
    write_u32_be(ctx->state[1], output,  4);
    write_u32_be(ctx->state[2], output,  8);
    write_u32_be(ctx->state[3], output, 12);
    write_u32_be(ctx->state[4], output, 16);
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

static void internal_process(struct sha1_ctx *ctx, const void *data)
{
    struct {
        uint32_t A; 
        uint32_t B; 
        uint32_t C; 
        uint32_t D; 
        uint32_t E;
        uint32_t temp;
        uint32_t W[16]; 
    } local;

    local.W[0]  = read_u32_be(data,  0);
    local.W[1]  = read_u32_be(data,  4);
    local.W[2]  = read_u32_be(data,  8);
    local.W[3]  = read_u32_be(data, 12);
    local.W[4]  = read_u32_be(data, 16);
    local.W[5]  = read_u32_be(data, 20);
    local.W[6]  = read_u32_be(data, 24);
    local.W[7]  = read_u32_be(data, 28);
    local.W[8]  = read_u32_be(data, 32);
    local.W[9]  = read_u32_be(data, 36);
    local.W[10] = read_u32_be(data, 40);
    local.W[11] = read_u32_be(data, 44);
    local.W[12] = read_u32_be(data, 48);
    local.W[13] = read_u32_be(data, 52);
    local.W[14] = read_u32_be(data, 56);
    local.W[15] = read_u32_be(data, 60);

#define S(x, n) (((x) << (n)) | (((x) & 0xffffffff) >> (32 - (n))))

#define R(t)                                               \
        (                                                  \
            local.temp = local.W[((t) -  3) & 0x0f] ^      \
            local.W[((t) -  8) & 0x0f] ^                   \
            local.W[((t) - 14) & 0x0f] ^                   \
            local.W[(t)        & 0x0f],                    \
            (local.W[(t) & 0x0f] = S(local.temp, 1))       \
        )

#define P(a, b, c, d, e, x)                                \
        do {                                               \
            (e) += S((a), 5) + F((b), (c), (d)) + K + (x); \
            (b) = S((b), 30);                              \
        } while (0)

    local.A = ctx->state[0];
    local.B = ctx->state[1];
    local.C = ctx->state[2];
    local.D = ctx->state[3];
    local.E = ctx->state[4];

#define F(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))
#define K 0x5a827999

    P(local.A, local.B, local.C, local.D, local.E, local.W[0]);
    P(local.E, local.A, local.B, local.C, local.D, local.W[1]);
    P(local.D, local.E, local.A, local.B, local.C, local.W[2]);
    P(local.C, local.D, local.E, local.A, local.B, local.W[3]);
    P(local.B, local.C, local.D, local.E, local.A, local.W[4]);
    P(local.A, local.B, local.C, local.D, local.E, local.W[5]);
    P(local.E, local.A, local.B, local.C, local.D, local.W[6]);
    P(local.D, local.E, local.A, local.B, local.C, local.W[7]);
    P(local.C, local.D, local.E, local.A, local.B, local.W[8]);
    P(local.B, local.C, local.D, local.E, local.A, local.W[9]);
    P(local.A, local.B, local.C, local.D, local.E, local.W[10]);
    P(local.E, local.A, local.B, local.C, local.D, local.W[11]);
    P(local.D, local.E, local.A, local.B, local.C, local.W[12]);
    P(local.C, local.D, local.E, local.A, local.B, local.W[13]);
    P(local.B, local.C, local.D, local.E, local.A, local.W[14]);
    P(local.A, local.B, local.C, local.D, local.E, local.W[15]);
    P(local.E, local.A, local.B, local.C, local.D, R(16));
    P(local.D, local.E, local.A, local.B, local.C, R(17));
    P(local.C, local.D, local.E, local.A, local.B, R(18));
    P(local.B, local.C, local.D, local.E, local.A, R(19));

#undef K
#undef F

#define F(x, y, z) ((x) ^ (y) ^ (z))
#define K 0x6ed9eba1

    P(local.A, local.B, local.C, local.D, local.E, R(20));
    P(local.E, local.A, local.B, local.C, local.D, R(21));
    P(local.D, local.E, local.A, local.B, local.C, R(22));
    P(local.C, local.D, local.E, local.A, local.B, R(23));
    P(local.B, local.C, local.D, local.E, local.A, R(24));
    P(local.A, local.B, local.C, local.D, local.E, R(25));
    P(local.E, local.A, local.B, local.C, local.D, R(26));
    P(local.D, local.E, local.A, local.B, local.C, R(27));
    P(local.C, local.D, local.E, local.A, local.B, R(28));
    P(local.B, local.C, local.D, local.E, local.A, R(29));
    P(local.A, local.B, local.C, local.D, local.E, R(30));
    P(local.E, local.A, local.B, local.C, local.D, R(31));
    P(local.D, local.E, local.A, local.B, local.C, R(32));
    P(local.C, local.D, local.E, local.A, local.B, R(33));
    P(local.B, local.C, local.D, local.E, local.A, R(34));
    P(local.A, local.B, local.C, local.D, local.E, R(35));
    P(local.E, local.A, local.B, local.C, local.D, R(36));
    P(local.D, local.E, local.A, local.B, local.C, R(37));
    P(local.C, local.D, local.E, local.A, local.B, R(38));
    P(local.B, local.C, local.D, local.E, local.A, R(39));

#undef K
#undef F

#define F(x, y, z) (((x) & (y)) | ((z) & ((x) | (y))))
#define K 0x8f1bbcdc

    P(local.A, local.B, local.C, local.D, local.E, R(40));
    P(local.E, local.A, local.B, local.C, local.D, R(41));
    P(local.D, local.E, local.A, local.B, local.C, R(42));
    P(local.C, local.D, local.E, local.A, local.B, R(43));
    P(local.B, local.C, local.D, local.E, local.A, R(44));
    P(local.A, local.B, local.C, local.D, local.E, R(45));
    P(local.E, local.A, local.B, local.C, local.D, R(46));
    P(local.D, local.E, local.A, local.B, local.C, R(47));
    P(local.C, local.D, local.E, local.A, local.B, R(48));
    P(local.B, local.C, local.D, local.E, local.A, R(49));
    P(local.A, local.B, local.C, local.D, local.E, R(50));
    P(local.E, local.A, local.B, local.C, local.D, R(51));
    P(local.D, local.E, local.A, local.B, local.C, R(52));
    P(local.C, local.D, local.E, local.A, local.B, R(53));
    P(local.B, local.C, local.D, local.E, local.A, R(54));
    P(local.A, local.B, local.C, local.D, local.E, R(55));
    P(local.E, local.A, local.B, local.C, local.D, R(56));
    P(local.D, local.E, local.A, local.B, local.C, R(57));
    P(local.C, local.D, local.E, local.A, local.B, R(58));
    P(local.B, local.C, local.D, local.E, local.A, R(59));

#undef K
#undef F

#define F(x, y, z) ((x) ^ (y) ^ (z))
#define K 0xca62c1d6

    P(local.A, local.B, local.C, local.D, local.E, R(60));
    P(local.E, local.A, local.B, local.C, local.D, R(61));
    P(local.D, local.E, local.A, local.B, local.C, R(62));
    P(local.C, local.D, local.E, local.A, local.B, R(63));
    P(local.B, local.C, local.D, local.E, local.A, R(64));
    P(local.A, local.B, local.C, local.D, local.E, R(65));
    P(local.E, local.A, local.B, local.C, local.D, R(66));
    P(local.D, local.E, local.A, local.B, local.C, R(67));
    P(local.C, local.D, local.E, local.A, local.B, R(68));
    P(local.B, local.C, local.D, local.E, local.A, R(69));
    P(local.A, local.B, local.C, local.D, local.E, R(70));
    P(local.E, local.A, local.B, local.C, local.D, R(71));
    P(local.D, local.E, local.A, local.B, local.C, R(72));
    P(local.C, local.D, local.E, local.A, local.B, R(73));
    P(local.B, local.C, local.D, local.E, local.A, R(74));
    P(local.A, local.B, local.C, local.D, local.E, R(75));
    P(local.E, local.A, local.B, local.C, local.D, R(76));
    P(local.D, local.E, local.A, local.B, local.C, R(77));
    P(local.C, local.D, local.E, local.A, local.B, R(78));
    P(local.B, local.C, local.D, local.E, local.A, R(79));

#undef K
#undef F

    ctx->state[0] += local.A;
    ctx->state[1] += local.B;
    ctx->state[2] += local.C;
    ctx->state[3] += local.D;
    ctx->state[4] += local.E;
}
