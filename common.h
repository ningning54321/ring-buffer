/***********************************************************************
 * Copyright (C)2016 Nathan NING                                       *
 * Author: nathan.y.ning@gmail.com                                     *
 *                                                                     *
 * Header File for common APIs                                         *
 *                                                                     *
 ***********************************************************************/

#ifndef COMMON_H
#define COMMON_H

#define ENABLE_DBG                             0

#define DATA_BUFFER_MAX                        64
#define DATA_PRODUCER_MAX                      50
#define SUCCESS                                1
#define FAILURE                                0
#define LOG_FILE                               "/tmp/app_output.txt"

void debugf(char *fmt, ...);
unsigned int get_data(char *buff);
int save_data(char *data);
void exit_with_err(int err_no, char *api_name);

#endif //COMMON_H

