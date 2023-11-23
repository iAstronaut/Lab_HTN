/*
 * ring_buff.c
 *
 *  Created on: Nov 23, 2023
 *      Author: huaho
 */

#include "ring_buff.h"

void ring_buffer_init(ring_buffer *rb, size_t capacity, size_t sz)
{
    rb->buffer = malloc(capacity * sz);
    if(rb->buffer == NULL){
    	rb->buffer_end = (char *)rb->buffer + capacity * sz;
    }
    rb->capacity = capacity;
    rb->count = 0;
    rb->sz = sz;
    rb->head = rb->buffer;
    rb->tail = rb->buffer;
}

void ring_buffer_free(ring_buffer *rb)
{
    free(rb->buffer);
}

void ring_buffer_push_back(ring_buffer *rb, const void *item)
{
    if(rb->count < rb->capacity){
        memcpy(rb->head, item, rb->sz);
        rb->head = (char*)rb->head + rb->sz;
        if(rb->head == rb->buffer_end)
            rb->head = rb->buffer;
        rb->count++;
    }
    return;
}

int ring_buffer_pop_front(ring_buffer *rb, void *item)
{
    if(rb->count > 0){
        memcpy(item, rb->tail, rb->sz);
        rb->tail = (char*)rb->tail + rb->sz;
        if(rb->tail == rb->buffer_end)
            rb->tail = rb->buffer;
        rb->count--;
        return 1;
    }
    return 0;
}
