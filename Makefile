# LinuxRogue makefile
# by Ashwin N <ashwin@despammed.com> 

SHELL = /bin/bash
CC = gcc

CFLAGS = -O2 -fomit-frame-pointer -Wall

LDFLAGS = -lncurses 

SOURCES = hit.c init.c instruct.c inventory.c level.c machdep.c main.c \
		  message.c monster.c move.c object.c pack.c play.c random.c ring.c \
		  room.c save.c score.c special_hit.c throw.c trap.c use.c zap.c
OBJECTS = $(SOURCES:.c=.o)
#DFILES = $(SOURCES:.c=.d)

rogue: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o rogue

.c.o:
	$(CC) $(CFLAGS) -c $<

#include: $(DFILES)

#%.d: %.c
#	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
#    sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
#	rm -f $@.$$$$

#.PHONY: clean
clean:
	rm -f $(OBJECTS) $(DFILES) rogue
