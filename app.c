/***********************************************************************
 * Copyright (C)2016 Nathan NING                                       *
 * Author: nathan.y.ning@gmail.com                                     *
 *                                                                     *
 * Linux Application layer test                                        *
 *                                                                     *
 ***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include "common.h"
#include "ringbuffer.h"

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static RB *ring_buff = NULL;

static void * producer()
{
    char data_buff[DATA_PRODUCER_MAX + 1] = {0};
    unsigned int available_space;
    int num_to_write = 0;
    int ret;

    for (;;)
    {
        if (0 == num_to_write)
        {
            num_to_write = get_data(data_buff);
        }
        ret = pthread_mutex_lock(&mtx);
        if (ret != 0)
        {
            exit_with_err(ret, "producer::pthread_mutex_lock");
        }

        available_space = RB_available_space(ring_buff);
        while (available_space < (unsigned int)num_to_write)
        {
            ret = pthread_cond_wait(&cond, &mtx);
            if (ret != 0)
            {
                exit_with_err(ret, "producer::pthread_cond_wait");
            }
            available_space = RB_available_space(ring_buff);
        }

        RB_write(ring_buff, data_buff, num_to_write);
        num_to_write = 0;
        ret = pthread_mutex_unlock(&mtx);
        if (ret != 0)
        {
            exit_with_err(ret, "producer::pthread_mutex_unlock");
        }

        /* More data appended, time to wake up any sleeping consumer */
        ret = pthread_cond_signal(&cond);
        if (ret != 0)
        {
            exit_with_err(ret, "producer::pthread_cond_signal");
        }
        usleep(5000);
    }

    return NULL;
}

static void * consumer()
{
    char data_buff[DATA_BUFFER_MAX + 1] = {0};
    unsigned int available_data;
    int num_to_read;
    int ret;

    for (;;)
    {
        ret = pthread_mutex_lock(&mtx);
        if (ret != 0)
        {
            exit_with_err(ret, "consumer::pthread_mutex_lock");
        }

        available_data = RB_available_data(ring_buff);
        while (available_data == 0)
        {
            ret = pthread_cond_wait(&cond, &mtx);
            if (ret != 0)
            {
                exit_with_err(ret, "consumer::pthread_cond_wait");
            }
            available_data = RB_available_data(ring_buff);
        }

        num_to_read = rand() % (available_data + 1);
        RB_read(ring_buff, data_buff, num_to_read);
        data_buff[num_to_read] = '\0';
        ret = save_data(data_buff);
        if (SUCCESS == ret)
        {
            RB_commit_read(ring_buff, num_to_read);
        }

        ret = pthread_mutex_unlock(&mtx);
        if (ret != 0)
        {
            exit_with_err(ret, "consumer::pthread_mutex_unlock");
        }

        /* More buffer released, time to wake up any sleeping producer */
        ret = pthread_cond_signal(&cond);
        if (ret != 0)
        {
            exit_with_err(ret, "consumer::pthread_cond_signal");
        }
        usleep(5000);
    }

    return NULL;
}

void signal_handler(sig)
int sig;
{
    switch(sig) {
    case SIGHUP:
    case SIGTERM:
    case SIGINT:
        RB_destroy(ring_buff);
        exit(0);
    }
}

int main()
{
    pthread_t t1, t2;
    int ret;

    ring_buff = RB_create(DATA_BUFFER_MAX);
    srand(time(NULL)); /* Init random number generator */
    signal(SIGHUP,signal_handler); /* catch hangup signal */
    signal(SIGTERM,signal_handler); /* catch kill signal */
    signal(SIGINT,signal_handler); /* catch interrupt signal */

    ret = pthread_create(&t1, NULL, producer, NULL);
    if (ret != 0)
    {
        exit_with_err(ret, "producer::pthread_create");
    }

    ret = pthread_create(&t2, NULL, consumer, NULL);
    if (ret != 0)
    {
        exit_with_err(ret, "consumer::pthread_create");
    }

    ret = pthread_join(t1, NULL);
    if (ret != 0)
    {
        exit_with_err(ret, "producer::pthread_join");
    }

    ret = pthread_join(t2, NULL);
    if (ret != 0)
    {
        exit_with_err(ret, "consumer::pthread_join");
    }

    exit(0);
}

