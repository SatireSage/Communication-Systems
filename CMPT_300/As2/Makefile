SOURCES = candykids.c bbuff.c stats.c
TARGET = candykids

CC_C = gcc
CFLAGS = -Wall -Werror -Wshadow -pthread -lrt -g

all: $(SOURCES)
	$(CC_C) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET) *.o