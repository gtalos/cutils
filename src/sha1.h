/*
 * Copyright (c) The Mbed TLS Contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef SHA1_H
#define SHA1_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief Storage type used to save SHA-1 context. */
struct sha1_ctx {
    uint32_t total[2];
    uint32_t state[5];
    uint8_t buffer[64];
};

/**
 * @brief This function initializes a SHA-1 context.
 * 
 * @param ctx The SHA-1 context to initialize.
 */
void sha1_init(struct sha1_ctx *ctx);

/**
 * @brief This function feeds an input buffer into an ongoing SHA-1
 *        calculation.
 * 
 * @param ctx The SHA-1 context to use. This must be initialized and
 *            have a hash operation started.
 * @param input The buffer holding the input data.
 * @param len The length of the input data in bytes.
 */
void sha1_update(struct sha1_ctx *ctx, const void *input, size_t len);

/**
 * @brief This function finishes the SHA-1 operation, and writes
 *        the result to the output buffer.
 * 
 * @param ctx The SHA-1 context to use. This must be initialized and
 *            have a hash operation started.
 * @param output The SHA-1 checksum result. This must be a writable
 *               buffer of at least 20 bytes.
 */
void sha1_final(struct sha1_ctx *ctx, void *output);

#ifdef __cplusplus
}
#endif

#endif /* SHA1_H */
