/*
 * Copyright (c) 2015 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @cond INTERNAL_HIDDEN */
/* The limit is used by algorithm for distinguishing between empty and full
 * state.
 */
#define RING_BUFFER_MAX_SIZE 0x80000000u
/** @endcond */

/**
 * @brief A structure to represent a ring buffer
 */
struct ring_buf {
    /** @cond INTERNAL_HIDDEN */
    uint8_t *buffer;
    uint32_t put_head;
    uint32_t put_tail;
    uint32_t put_base;
    uint32_t get_head;
    uint32_t get_tail;
    uint32_t get_base;
    uint32_t size;
    /** @endcond */
};

/**
 * @brief Initialize a ring buffer for byte data.
 *
 * This routine initializes a ring buffer, prior to its first use. It is only
 * used for ring buffers not defined using RING_BUF_DECLARE.
 *
 * @param rb Address of ring buffer.
 * @param size Ring buffer size (in bytes).
 * @param data Ring buffer data area (uint8_t data[size]).
 */
void ring_buf_init(struct ring_buf *rb, uint32_t size, uint8_t *data);

/**
 * @brief Initialize an "item based" ring buffer.
 *
 * This routine initializes a ring buffer, prior to its first use. It is only
 * used for ring buffers not defined using RING_BUF_ITEM_DECLARE.
 *
 * Each data item is an array of 32-bit words (from zero to 1020 bytes in
 * length), coupled with a 16-bit type identifier and an 8-bit integer value.
 *
 * @param rb Address of ring buffer.
 * @param size Ring buffer size (in 32-bit words)
 * @param data Ring buffer data area (uint32_t data[size]).
 */
void ring_buf_item_init(struct ring_buf *rb, uint32_t size, uint32_t *data);

/**
 * @brief Determine if a ring buffer is empty.
 *
 * @param rb Address of ring buffer.
 *
 * @return true if the ring buffer is empty, or false if not.
 */
bool ring_buf_is_empty(struct ring_buf *rb);

/**
 * @brief Reset ring buffer state.
 *
 * @param rb Address of ring buffer.
 */
void ring_buf_reset(struct ring_buf *rb);

/**
 * @brief Determine free space in a ring buffer.
 *
 * @param rb Address of ring buffer.
 *
 * @return Ring buffer free space (in bytes).
 */
uint32_t ring_buf_space_get(struct ring_buf *rb);

/**
 * @brief Determine free space in an "item based" ring buffer.
 *
 * @param rb Address of ring buffer.
 *
 * @return Ring buffer free space (in 32-bit words).
 */
uint32_t ring_buf_item_space_get(struct ring_buf *rb);

/**
 * @brief Return ring buffer capacity.
 *
 * @param rb Address of ring buffer.
 *
 * @return Ring buffer capacity (in bytes).
 */
uint32_t ring_buf_capacity_get(struct ring_buf *rb);

/**
 * @brief Determine used space in a ring buffer.
 *
 * @param rb Address of ring buffer.
 *
 * @return Ring buffer space used (in bytes).
 */
uint32_t ring_buf_size_get(struct ring_buf *rb);

/**
 * @brief Allocate buffer for writing data to a ring buffer.
 *
 * With this routine, memory copying can be reduced since internal ring buffer
 * can be used directly by the user. Once data is written to allocated area
 * number of bytes written must be confirmed (see @ref ring_buf_put_finish).
 *
 * @warning
 * Use cases involving multiple writers to the ring buffer must prevent
 * concurrent write operations, either by preventing all writers from
 * being preempted or by using a mutex to govern writes to the ring buffer.
 *
 * @warning
 * Ring buffer instance should not mix byte access and item access
 * (calls prefixed with ring_buf_item_).
 *
 * @param[in]  rb  Address of ring buffer.
 * @param[out] data Pointer to the address. It is set to a location within
 *		    ring buffer.
 * @param[in]  size Requested allocation size (in bytes).
 *
 * @return Size of allocated buffer which can be smaller than requested if
 *	   there is not enough free space or buffer wraps.
 */
uint32_t ring_buf_put_claim(struct ring_buf *rb, uint8_t **data, uint32_t size);

/**
 * @brief Indicate number of bytes written to allocated buffers.
 *
 * The number of bytes must be equal to or lower than the sum corresponding
 * to all preceding @ref ring_buf_put_claim invocations (or even 0). Surplus
 * bytes will be returned to the available free buffer space.
 *
 * @warning
 * Use cases involving multiple writers to the ring buffer must prevent
 * concurrent write operations, either by preventing all writers from
 * being preempted or by using a mutex to govern writes to the ring buffer.
 *
 * @warning
 * Ring buffer instance should not mix byte access and item access
 * (calls prefixed with ring_buf_item_).
 *
 * @param  rb  Address of ring buffer.
 * @param  size Number of valid bytes in the allocated buffers.
 *
 * @retval 0 Successful operation.
 * @retval -1 Provided @a size exceeds free space in the ring buffer.
 */
int32_t ring_buf_put_finish(struct ring_buf *rb, uint32_t size);

/**
 * @brief Write (copy) data to a ring buffer.
 *
 * This routine writes data to a ring buffer @a buf.
 *
 * @warning
 * Use cases involving multiple writers to the ring buffer must prevent
 * concurrent write operations, either by preventing all writers from
 * being preempted or by using a mutex to govern writes to the ring buffer.
 *
 * @warning
 * Ring buffer instance should not mix byte access and item access
 * (calls prefixed with ring_buf_item_).
 *
 * @param buf Address of ring buffer.
 * @param data Address of data.
 * @param size Data size (in bytes).
 *
 * @retval Number of bytes written.
 */
uint32_t ring_buf_put(struct ring_buf *rb, const uint8_t *data, uint32_t size);

/**
 * @brief Get address of a valid data in a ring buffer.
 *
 * With this routine, memory copying can be reduced since internal ring buffer
 * can be used directly by the user. Once data is processed it must be freed
 * using @ref ring_buf_get_finish.
 *
 * @warning
 * Use cases involving multiple reads of the ring buffer must prevent
 * concurrent read operations, either by preventing all readers from
 * being preempted or by using a mutex to govern reads to the ring buffer.
 *
 * @warning
 * Ring buffer instance should not mix byte access and item access
 * (calls prefixed with ring_buf_item_).
 *
 * @param[in]  buf  Address of ring buffer.
 * @param[out] data Pointer to the address. It is set to a location within
 *		    ring buffer.
 * @param[in]  size Requested size (in bytes).
 *
 * @return Number of valid bytes in the provided buffer which can be smaller
 *	   than requested if there is not enough free space or buffer wraps.
 */
uint32_t ring_buf_get_claim(struct ring_buf *rb, uint8_t **data,
			                uint32_t size);

/**
 * @brief Indicate number of bytes read from claimed buffer.
 *
 * The number of bytes must be equal or lower than the sum corresponding to
 * all preceding @ref ring_buf_get_claim invocations (or even 0). Surplus
 * bytes will remain available in the buffer.
 *
 * @warning
 * Use cases involving multiple reads of the ring buffer must prevent
 * concurrent read operations, either by preventing all readers from
 * being preempted or by using a mutex to govern reads to the ring buffer.
 *
 * @warning
 * Ring buffer instance should not mix byte access and  item mode
 * (calls prefixed with ring_buf_item_).
 *
 * @param  buf  Address of ring buffer.
 * @param  size Number of bytes that can be freed.
 *
 * @retval 0 Successful operation.
 * @retval -1 Provided @a size exceeds valid bytes in the ring buffer.
 */
int32_t ring_buf_get_finish(struct ring_buf *rb, uint32_t size);

/**
 * @brief Read data from a ring buffer.
 *
 * This routine reads data from a ring buffer @a buf.
 *
 * @warning
 * Use cases involving multiple reads of the ring buffer must prevent
 * concurrent read operations, either by preventing all readers from
 * being preempted or by using a mutex to govern reads to the ring buffer.
 *
 * @warning
 * Ring buffer instance should not mix byte access and  item mode
 * (calls prefixed with ring_buf_item_).
 *
 * @param buf  Address of ring buffer.
 * @param data Address of the output buffer. Can be NULL to discard data.
 * @param size Data size (in bytes).
 *
 * @retval Number of bytes written to the output buffer.
 */
uint32_t ring_buf_get(struct ring_buf *rb, uint8_t *data, uint32_t size);

/**
 * @brief Peek at data from a ring buffer.
 *
 * This routine reads data from a ring buffer @a buf without removal.
 *
 * @warning
 * Use cases involving multiple reads of the ring buffer must prevent
 * concurrent read operations, either by preventing all readers from
 * being preempted or by using a mutex to govern reads to the ring buffer.
 *
 * @warning
 * Ring buffer instance should not mix byte access and  item mode
 * (calls prefixed with ring_buf_item_).
 *
 * @warning
 * Multiple calls to peek will result in the same data being 'peeked'
 * multiple times. To remove data, use either @ref ring_buf_get or
 * @ref ring_buf_get_claim followed by @ref ring_buf_get_finish with a
 * non-zero `size`.
 *
 * @param buf  Address of ring buffer.
 * @param data Address of the output buffer. Cannot be NULL.
 * @param size Data size (in bytes).
 *
 * @retval Number of bytes written to the output buffer.
 */
uint32_t ring_buf_peek(struct ring_buf *rb, uint8_t *data, uint32_t size);

/**
 * @brief Write a data item to a ring buffer.
 *
 * This routine writes a data item to ring buffer @a buf. The data item
 * is an array of 32-bit words (from zero to 1020 bytes in length),
 * coupled with a 16-bit type identifier and an 8-bit integer value.
 *
 * @warning
 * Use cases involving multiple writers to the ring buffer must prevent
 * concurrent write operations, either by preventing all writers from
 * being preempted or by using a mutex to govern writes to the ring buffer.
 *
 * @param buf Address of ring buffer.
 * @param type Data item's type identifier (application specific).
 * @param value Data item's integer value (application specific).
 * @param data Address of data item.
 * @param size32 Data item size (number of 32-bit words).
 *
 * @retval 0 Data item was written.
 * @retval -2 Ring buffer has insufficient free space.
 */
int32_t ring_buf_item_put(struct ring_buf *rb, uint16_t type, uint8_t value,
                          uint32_t *data, uint8_t size32);

/**
 * @brief Read a data item from a ring buffer.
 *
 * This routine reads a data item from ring buffer @a buf. The data item
 * is an array of 32-bit words (up to 1020 bytes in length),
 * coupled with a 16-bit type identifier and an 8-bit integer value.
 *
 * @warning
 * Use cases involving multiple reads of the ring buffer must prevent
 * concurrent read operations, either by preventing all readers from
 * being preempted or by using a mutex to govern reads to the ring buffer.
 *
 * @param buf Address of ring buffer.
 * @param type Area to store the data item's type identifier.
 * @param value Area to store the data item's integer value.
 * @param data Area to store the data item. Can be NULL to discard data.
 * @param size32 Size of the data item storage area (number of 32-bit chunks).
 *
 * @retval 0 Data item was fetched; @a size32 now contains the number of
 *         32-bit words read into data area @a data.
 * @retval -1 Ring buffer is empty.
 * @retval -2 Data area @a data is too small; @a size32 now contains
 *         the number of 32-bit words needed.
 */
int32_t ring_buf_item_get(struct ring_buf *rb, uint16_t *type, uint8_t *value,
                          uint32_t *data, uint8_t *size32);

#ifdef __cplusplus
}
#endif

#endif /* RING_BUFFER_H */
