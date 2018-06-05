#
# Simple makefile for Linux Application Layer test
#

CC = gcc

LINKS = -lpthread
OBJS = common.o ringbuffer.o
HEADERS = common.h ringbuffer.h
FLAGS = -O2 -Wall -Wextra -Werror

APP = app

all: $(OBJS) $(APP)

app: $(OBJS) app.c common.c ringbuffer.c $(HEADERS)
	$(CC) $(FLAGS) -o app $(OBJS) app.c $(LINKS)

common.o: common.c
	$(CC) -c common.c

ringbuffer.o: ringbuffer.c
	$(CC) -c ringbuffer.c

clean:
	rm -f *.o
	rm -f core*
	rm -f $(APP)

