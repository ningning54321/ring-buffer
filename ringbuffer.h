/***********************************************************************
 * Copyright (C)2016 Nathan NING                                       *
 * Author: nathan.y.ning@gmail.com                                     *
 *                                                                     *
 * Header File for Ring Buffer                                         *
 *                                                                     *
 ***********************************************************************/

#ifndef RING_BUFFER_H
#define RING_BUFFER_H

typedef struct {
    char *buffer;
    char full;
    unsigned int length;
    unsigned int start;
    unsigned int end;
} RB;

RB *RB_create(unsigned int length);
void RB_destroy(RB *buf);
unsigned int RB_read(RB *buf, char *target, unsigned int max_len);
void RB_write(RB *buf, char *data, unsigned int len);
void RB_commit_read(RB *buf, unsigned int len);
void RB_reset(RB *buf);

#define RB_available_data(B) (((B)->end == (B)->start) ? \
                              ((B)->full ? (B)->length : 0) : \
                              (((B)->end - (B)->start + (B)->length) % (B)->length))
#define RB_available_space(B) (((B)->end == (B)->start) ? \
                              ((B)->full ? 0 : (B)->length) : \
                              (((B)->start - (B)->end + (B)->length) % (B)->length))
#define RB_starts_at(B) ((B)->buffer + (B)->start)
#define RB_ends_at(B) ((B)->buffer + (B)->end)

#endif

