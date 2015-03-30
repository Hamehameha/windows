CC = sh4-linux-gcc

export SYSROOT=$(shell readlink -f `$(CC) -print-prog-name=gcc` | sed -e s!/usr/sh4-linux-uclibc/.*!!)

CFLAGS = -Wall `sh4-linux-directfb-config --cflags`
LDFLAGS = `sh4-linux-directfb-config --libs` -lasound -lts


OBJS = windows.o directFB_init.o show_img.o

PROG = windows

all: $(OBJS) $(PROG)

$(PROG) : $(OBJS)
	$(CC) $(OBJS) -o $(PROG) $(LDFLAGS)
	
.SUFFIXES : .c .o

.c.o :
	$(CC) -c $< -o $@ $(CFLAGS)

clean :
	-rm $(PROG)
	-rm *.o

