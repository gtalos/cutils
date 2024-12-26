#ifndef SHA512_H
#define SHA512_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief Storage type used to save SHA-512 context. */
struct sha512_ctx {
    uint64_t total[2];
    uint64_t state[8];
    uint8_t buffer[128];
};

/**
 * @brief This function initializes a SHA-512 context.
 * 
 * @param ctx The SHA-512 context to initialize.
 */
void sha512_init(struct sha512_ctx *ctx);

/**
 * @brief This function feeds an input buffer into an ongoing SHA-1
 *        calculation.
 * 
 * @param ctx The SHA-512 context to use. This must be initialized and
 *            have a hash operation started.
 * @param input The buffer holding the input data.
 * @param len The length of the input data in bytes.
 */
void sha512_update(struct sha512_ctx *ctx, const void *input, size_t len);

/**
 * @brief This function finishes the SHA-1 operation, and writes
 *        the result to the output buffer.
 * 
 * @param ctx The SHA-512 context to use. This must be initialized and
 *            have a hash operation started.
 * @param output The SHA-512 checksum result. This must be a writable
 *               buffer of at least 64 bytes.
 */
void sha512_final(struct sha512_ctx *ctx, void *output);

#ifdef __cplusplus
}
#endif

#endif /* SHA512_H */
