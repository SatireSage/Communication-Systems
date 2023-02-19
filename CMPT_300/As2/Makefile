SOURCES = candykids.c bbuff.c stats.c
TARGET = candykids

CC_C = gcc
CFLAGS = -Wall -g -Werror -Wshadow -pthread -lrt -std=c99 -D_POSIX_C_SOURCE=200809L

all: clean $(SOURCES)
	$(CC_C) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET) *.o