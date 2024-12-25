/*
 * Copyright (c) The Mbed TLS Contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef SHA256_H
#define SHA256_H

#include <stdint.h>
#include <stddef.h>

/*! @brief Storage type used to save SHA-256 context. */
typedef struct {
    uint32_t total[2];
    uint32_t state[8];
    uint8_t buffer[64];
} sha256_ctx_t;

/**
 * @brief This function initializes a SHA-256 context.
 * 
 * @param ctx The SHA-256 context to initialize.
 */
void sha256_init(sha256_ctx_t *ctx);

/**
 * @brief This function feeds an input buffer into an ongoing SHA-256
 *        calculation.
 * 
 * @param ctx The SHA-256 context to use. This must be initialized and
 *            have a hash operation started.
 * @param input The buffer holding the input data.
 * @param len The length of the input data in bytes.
 */
void sha256_update(sha256_ctx_t *ctx, const void *input, size_t len);

/**
 * @brief This function finishes the SHA-256 operation, and writes
 *        the result to the output buffer.
 * 
 * @param ctx The SHA-256 context to use. This must be initialized and
 *            have a hash operation started.
 * @param output The SHA-256 checksum result. This must be a writable
 *               buffer of at least 32 bytes.
 */
void sha256_finish(sha256_ctx_t *ctx, void *output);

#endif /* SHA256_H */
