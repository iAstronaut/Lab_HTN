/*
 * ring_buff.h
 *
 *  Created on: Nov 23, 2023
 *      Author: huaho
 */

#ifndef INC_RING_BUFF_H_
#define INC_RING_BUFF_H_

#include "stdint.h"
#include "stddef.h"


typedef struct
{
    void *buffer;     // data buffer
    void *buffer_end; // end of data buffer
    size_t capacity;  // maximum number of items in the buffer
    size_t count;     // number of items in the buffer
    size_t sz;        // size of each item in the buffer
    void *head;       // pointer to head
    void *tail;       // pointer to tail
} ring_buffer;

void ring_buffer_init(ring_buffer *rb, size_t capacity, size_t sz);
void ring_buffer_free(ring_buffer *rb);
void ring_buffer_push_back(ring_buffer *rb, const void *item);
int ring_buffer_pop_front(ring_buffer *rb, void *item);

#endif /* INC_RING_BUFF_H_ */
