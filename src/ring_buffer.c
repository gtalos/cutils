/* ring_buffer.c: Simple ring buffer API */

/*
 * Copyright (c) 2015 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ring_buffer.h"

#include <string.h>
#include <assert.h>

#define RING_BUFFER_SIZE_ASSERT_MSG "Size too big"
#define MIN(a, b) (((a) < (b)) ? (a) : (b))


/**
 * Internal data structure for a buffer header.
 *
 * We want all of this to fit in a single uint32_t. Every item stored in the
 * ring buffer will be one of these headers plus any extra data supplied
 */
struct ring_element {
    uint16_t type;   /**< Application-specific */
    uint8_t length;  /**< length in 32-bit chunks */
    uint8_t value;   /**< Room for small integral values */
};


static void ring_buf_internal_reset(struct ring_buf *rb, uint32_t value);
static uint32_t ring_element_to_uint32(struct ring_element *re);
static struct ring_element uint32_to_ring_element(uint32_t header);


void ring_buf_init(struct ring_buf *rb, uint32_t size, uint8_t *data)
{
    assert(size < RING_BUFFER_MAX_SIZE && RING_BUFFER_SIZE_ASSERT_MSG);

    rb->size = size;
    rb->buffer = data;
    ring_buf_internal_reset(rb, 0);
}

void ring_buf_item_init(struct ring_buf *rb, uint32_t size, uint32_t *data)
{
    assert(size < RING_BUFFER_MAX_SIZE / sizeof(uint32_t) && RING_BUFFER_SIZE_ASSERT_MSG);
    
    ring_buf_init(rb, size * sizeof(uint32_t), (uint8_t *) data);
}

bool ring_buf_is_empty(struct ring_buf *rb)
{
    return rb->get_head == rb->put_tail;
}

void ring_buf_reset(struct ring_buf *rb)
{
    ring_buf_internal_reset(rb, 0);
}

uint32_t ring_buf_space_get(struct ring_buf *rb)
{
    return rb->size - (rb->put_head - rb->get_tail);
}

uint32_t ring_buf_item_space_get(struct ring_buf *rb)
{
    return ring_buf_space_get(rb) / sizeof(uint32_t);
}

uint32_t ring_buf_capacity_get(struct ring_buf *rb)
{
    return rb->size;
}

uint32_t ring_buf_size_get(struct ring_buf *rb)
{
    return rb->put_tail - rb->get_head;
}

uint32_t ring_buf_put_claim(struct ring_buf *rb, uint8_t **data, uint32_t size)
{
    uint32_t base = rb->put_base;
    uint32_t wrap_size = rb->put_head - base;

    if (wrap_size >= rb->size) {
        /* put_base is not yet adjusted */
        wrap_size -= rb->size;
        base += rb->size;
    }

    wrap_size = rb->size - wrap_size;

    uint32_t free_space = ring_buf_space_get(rb);

    size = MIN(size, free_space);
    size = MIN(size, wrap_size);

    *data = &rb->buffer[rb->put_head - base];
    rb->put_head += size;

    return size;
}

int32_t ring_buf_put_finish(struct ring_buf *rb, uint32_t size)
{
    uint32_t finish_space = rb->put_head - rb->put_tail;

    if (size > finish_space) {
        return -1;
    }

    rb->put_tail += size;
    rb->put_head = rb->put_tail;

    uint32_t wrap_size = rb->put_tail - rb->put_base;

    if (wrap_size >= rb->size) {
        /* we wrapped: adjust put_base */
        rb->put_base += rb->size;
    }

    return 0;
}

uint32_t ring_buf_put(struct ring_buf *rb, const uint8_t *data, uint32_t size)
{
    uint32_t total_size = 0;
    uint32_t partial_size;

    do {
        uint8_t *dst;
        partial_size = ring_buf_put_claim(rb, &dst, size);
        memcpy(dst, data, partial_size);
        total_size += partial_size;
        size -= partial_size;
        data += partial_size;
    } while (size && partial_size);

    int32_t finish_result = ring_buf_put_finish(rb, total_size);
    assert(finish_result == 0);
    (void) finish_result;

    return total_size;
}

uint32_t ring_buf_get_claim(struct ring_buf *rb, uint8_t **data, uint32_t size)
{
    uint32_t base = rb->get_base;
    uint32_t wrap_size = rb->get_head - base;

    if (wrap_size >= rb->size) {
        /* get_base is not yet adjusted */
        wrap_size -= rb->size;
        base += rb->size;
    }

    wrap_size = rb->size - wrap_size;

    uint32_t available_size = ring_buf_size_get(rb);

    size = MIN(size, available_size);
    size = MIN(size, wrap_size);

    *data = &rb->buffer[rb->get_head - base];
    rb->get_head += size;

    return size;
}

int32_t ring_buf_get_finish(struct ring_buf *rb, uint32_t size)
{
    uint32_t finish_space = rb->get_head - rb->get_tail;

    if (size > finish_space) {
        return -1;
    }

    rb->get_tail += size;
    rb->get_head = rb->get_tail;

    uint32_t wrap_size = rb->get_tail - rb->get_base;

    if (wrap_size >= rb->size) {
        /* we wrapped: adjust get_base */
        rb->get_base += rb->size;
    }

    return 0;
}

uint32_t ring_buf_get(struct ring_buf *rb, uint8_t *data, uint32_t size)
{
    uint32_t total_size = 0;
    uint32_t partial_size;

    do {
        uint8_t *src;
        partial_size = ring_buf_get_claim(rb, &src, size);

        if (data) {
            memcpy(data, src, partial_size);
            data += partial_size;
        }

        total_size += partial_size;
        size -= partial_size;
    } while (size && partial_size);

    int32_t finish_result = ring_buf_get_finish(rb, total_size);
    assert(finish_result == 0);
    (void) finish_result;

    return total_size;
}

uint32_t ring_buf_peek(struct ring_buf *rb, uint8_t *data, uint32_t size)
{
    uint32_t total_size = 0;

    size = MIN(size, ring_buf_size_get(rb));

    uint32_t partial_size;
    do {
        uint8_t *src;
        partial_size = ring_buf_get_claim(rb, &src, size);
        assert(data != NULL);
        memcpy(data, src, partial_size);
        data += partial_size;
        total_size += partial_size;
        size -= partial_size;
    } while (size && partial_size);

    /* effectively unclaim total_size bytes */
    int32_t finish_result = ring_buf_get_finish(rb, 0);
    assert(finish_result == 0);
    (void) finish_result;

    return total_size;
}

int32_t ring_buf_item_put(struct ring_buf *rb, uint16_t type, uint8_t value,
                          uint32_t *data32, uint8_t size32)
{
    uint8_t *data = (uint8_t *) data32;

    uint32_t space = ring_buf_space_get(rb);
    uint32_t size = size32 * sizeof(uint32_t);

    if (size + sizeof(uint32_t) > space) {
        return -2;
    }

    uint8_t *dst;
    uint32_t claim_result = ring_buf_put_claim(rb, &dst, sizeof(uint32_t));
    assert(claim_result == sizeof(uint32_t));
    (void) claim_result;

    struct ring_element header = {
        .type = type,
        .length = size32,
        .value = value,
    };

    uint32_t header_u32 = ring_element_to_uint32(&header);
    memcpy(dst, &header_u32, sizeof(header_u32));

    uint32_t total_size = sizeof(uint32_t);

    uint32_t partial_size;
    do {
        partial_size = ring_buf_put_claim(rb, &dst, size);
        memcpy(dst, data, partial_size);
        size -= partial_size;
        total_size += partial_size;
        data += partial_size;
    } while (size && partial_size);

    assert(size == 0);

    int32_t finish_result = ring_buf_put_finish(rb, total_size);
    assert(finish_result == 0);
    (void) finish_result;

    return 0;
}

int32_t ring_buf_item_get(struct ring_buf *rb, uint16_t *type, uint8_t *value,
                          uint32_t *data32, uint8_t *size32)
{
    uint8_t *data = (uint8_t *) data32;

    if (ring_buf_is_empty(rb)) {
        return -1;
    }

    uint8_t *src;
    uint32_t claim_result = ring_buf_get_claim(rb, &src, sizeof(uint32_t));
    assert(claim_result == sizeof(uint32_t));
    (void) claim_result;

    uint32_t header_u32;
    memcpy(&header_u32, src, sizeof(uint32_t));
    struct ring_element header = uint32_to_ring_element(header_u32);

    if (data && (header.length > *size32)) {
        *size32 = header.length;
        ring_buf_get_finish(rb, 0);

        return -2;
    }

    *size32 = header.length;
    *type = header.type;
    *value = header.value;

    uint32_t total_size = sizeof(struct ring_element);
    uint32_t size = *size32 * sizeof(uint32_t);

    uint32_t partial_size;
    do {
        partial_size = ring_buf_get_claim(rb, &src, size);

        if (data) {
            memcpy(data, src, partial_size);
            data += partial_size;
        }

        total_size += partial_size;
        size -= partial_size;
    } while (size && partial_size);

    int32_t finish_result = ring_buf_get_finish(rb, total_size);
    assert(finish_result == 0);
    (void) finish_result;

    return 0;
}

static void ring_buf_internal_reset(struct ring_buf *rb, uint32_t value)
{
    rb->put_head = value;
    rb->put_tail = value;
    rb->put_base = value;
    rb->get_head = value;
    rb->get_tail = value;
    rb->get_base = value;
}

static uint32_t ring_element_to_uint32(struct ring_element *re) {
    uint32_t type = re->type;
    uint32_t value = re->value;
    uint32_t length = re->length;

    uint32_t header = type << 16;
    header |= value << 8;
    header |= length;

    return header;
}

static struct ring_element uint32_to_ring_element(uint32_t header) {
    uint32_t type = header >> 16;
    uint32_t value = header >> 8;
    uint32_t length = header;

    struct ring_element re = {
        .type = (uint16_t) type,
        .length = (uint8_t) length,
        .value = (uint8_t) value,
    };
    
    return re;
}
