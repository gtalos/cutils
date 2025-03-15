/*
 * Copyright (c) 2024 The Mbed TLS Contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef AES_H
#define AES_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief The AES context type definition. */
struct aes_ctx {
    uint32_t nr;
    size_t rk_offset;                              
    uint32_t buf[68];
};

/**
 * @brief This function initializes the specified AES context.
 * 
 * @param ctx The AES context to initialize.
 */
void aes_init(struct aes_ctx *ctx);

/**
 * @brief This function sets the encryption key.
 * 
 * @param ctx The AES context to which the key should be bound. 
 *            It must be initialized.
 * @param key The encryption key.
 * @param key_len The size of data passed in bytes. Valid options are
 *                16, 20 and 32 bytes.
 * @return enum aes_status_t Status of the key configuration operation.
 */
int32_t aes_enc_set_key(struct aes_ctx *ctx, const void *key, uint32_t key_len);

/**
 * @brief This function sets the decryption key.
 * 
 * @param ctx The AES context to which the key should be bound. 
 *            It must be initialized.
 * @param key The decryption key.
 * @param key_len The size of data passed in bytes. Valid options are
 *                16, 20 and 32 bytes.
 * @return enum aes_status_t Status of the key configuration operation.
 */
int32_t aes_dec_set_key(struct aes_ctx *ctx, const void *key, uint32_t key_len);

/**
 * @brief This function performs an AES-CBC encryption operation on 
 *        full blocks.
 * 
 * @param ctx The AES context to use for decryption.
 * @param iv Initialization vector (updated after use). It must be a 
 *           readable and writeable buffer of 16 bytes.
 * @param pt The buffer holding the input data. It must be readable and 
 *           of size pt_len bytes.
 * @param pt_len The length of the input data in bytes. This must be a
 *               multiple of the block size (16 bytes).
 * @param ct The buffer holding the output data. It must be writeable and of 
 *           size pt_len bytes.
 * @return enum aes_status_t Status of the encryption operation.
 */
int32_t aes_enc_cbc(struct aes_ctx *ctx, void *iv, const void *pt, size_t pt_len, void *ct);

/**
 * @brief This function performs an AES-CBC decryption operation on 
 *        full blocks.
 * 
 * @param ctx The AES context to use for encryption.
 * @param iv Initialization vector (updated after use). It must be a 
 *           readable and writeable buffer of 16 bytes.
 * @param ct The buffer holding the input data. It must be readable and of size 
 *           ct_len bytes.
 * @param ct_len The length of the input data in bytes. This must be a
 *               multiple of the block size (16 bytes).
 * @param pt The buffer holding the output data. It must be writeable and of 
 *           size ct_len bytes.
 * @return enum aes_status_t Status of the decryption operation.
 */
int32_t aes_dec_cbc(struct aes_ctx *ctx, void *iv, const void *ct, size_t ct_len, void *pt);

#ifdef __cplusplus
}
#endif

#endif /* AES_H */
