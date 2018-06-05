/***********************************************************************
 * Copyright (C)2016 Nathan NING                                       *
 * Author: nathan.y.ning@gmail.com                                     *
 *                                                                     *
 * APIs for Ring Buffer                                                *
 *                                                                     *
 ***********************************************************************/

#include <stdlib.h>
#include <string.h>
#include "ringbuffer.h"

extern void debugf(char *fmt, ...);

RB *RB_create(unsigned int length)
{
    RB *buffer = calloc(1, sizeof(RB));
    buffer->length  = length;
    buffer->start = 0;
    buffer->end = 0;
    buffer->buffer = calloc(buffer->length+1, 1);
    buffer->full = 0;

    return buffer;
}

void RB_destroy(RB *buffer)
{
    if(buffer) {
        free(buffer->buffer);
        free(buffer);
        buffer = NULL;
    }
}

static void RB_commit_write(RB *buffer, unsigned int length)
{
    debugf("RB_commit_write: start: %u, end: %u, is_full: %d\n",
           buffer->start, buffer->end, buffer->full);
    if (length < 1) return;
    buffer->end = (buffer->end + length) % buffer->length;
    if (buffer->end == buffer->start) {
        buffer->full = 1;
    }
    debugf("RB_commit_write: data length: %u, new end: %u, is_full: %d\n",
           length, buffer->end, buffer->full);
}

void RB_commit_read(RB *buffer, unsigned int length)
{
    debugf("RB_commit_read: start: %u, end: %u, is_full: %d\n",
           buffer->start, buffer->end, buffer->full);
    if (length < 1) return;
    buffer->start = (buffer->start + length) % buffer->length;

    buffer->full = 0;
    //reset the pointer to faster the RB read/write
    if (buffer->end == buffer->start) {
        buffer->start = buffer->end = 0;
    }
    debugf("RB_commit_read: new start: %u, data length: %u, is_full: %d\n",
           buffer->start, length, buffer->full);
}

//when there is no enough space, always overwrite the old data
void RB_write(RB *buffer, char *data, unsigned int length)
{
    int buff_to_end = buffer->length - buffer->end;

    debugf("RB_write: start: %u, end: %u, new data: %u\n",
           buffer->start, buffer->end, length);

    if(length > RB_available_space(buffer)) {
        debugf("Not enough space: %u request, %u available\n",
               length, RB_available_data(buffer));
        //adjust the start (read pointer)
        RB_commit_read(buffer, length-RB_available_space(buffer));
    }

    if (length <= buff_to_end) {
        memcpy(RB_ends_at(buffer), data, length);
    } else {
        memcpy(RB_ends_at(buffer), data, buff_to_end);
        memcpy(buffer->buffer, data + buff_to_end, length - buff_to_end);
    }

    RB_commit_write(buffer, length);
}

unsigned int RB_read(RB *buffer, char *target, unsigned int max_length)
{
    unsigned int data_to_end = buffer->length - buffer->start;
    unsigned int data_to_read = (RB_available_data(buffer) > max_length) ?
                                max_length : RB_available_data(buffer);

    debugf("RB_read: start: %u, end: %u, max: %u\n",
           buffer->start, buffer->end, max_length);

    if (data_to_read <= data_to_end) {
        memcpy(target, RB_starts_at(buffer), data_to_read);
    } else {
        memcpy(target, RB_starts_at(buffer), data_to_end);
        memcpy(target + data_to_end, buffer->buffer, data_to_read - data_to_end);
    }

    return data_to_read;
}

void RB_reset(RB *buffer)
{
    //reset the pointer to faster the RB read/write
    if (buffer->end == buffer->start) {
        buffer->start = buffer->end = 0;
    }
}

