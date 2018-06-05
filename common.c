/***********************************************************************
 * Copyright (C)2016 Nathan NING                                       *
 * Author: nathan.y.ning@gmail.com                                     *
 *                                                                     *
 * Implementations on common APIs                                      *
 *                                                                     *
 ***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "common.h"

void debugf(char *fmt, ...) {
    va_list vap;
    char output[1024];

    if (!ENABLE_DBG)
    {
        return;
    }

    va_start(vap, fmt);
    vsnprintf(output, 1024, fmt, vap);
    va_end(vap);
    printf("%s\n", output);
}

unsigned int get_data(char *buff)
{
    int len, i;
    const char char_set[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int char_set_len = 26+26+10;
    int char_index;

    len = rand() % DATA_PRODUCER_MAX;
    for(i=0; i<=len; i++)
    {
        char_index = rand() % char_set_len;
        buff[i] = char_set[char_index];
    }
    buff[i] = '\0';
    printf("%s\n", buff);

    return i;
}

int save_data(char *data)
{
    FILE *fp;

    fp = fopen(LOG_FILE, "a");
    if (NULL == fp)
    {
        return FAILURE;
    }
    fputs(data, fp);
    fprintf(fp, "\n");

    fclose(fp);
    return SUCCESS;
}

void exit_with_err(int err_no, char *api_name)
{
    printf("Failed to call: %s, with err(%d)\n", api_name, err_no);
    exit(1);
}

