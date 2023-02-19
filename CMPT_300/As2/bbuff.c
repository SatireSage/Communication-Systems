#include "bbuff.h"
#include <stddef.h>
#include <pthread.h>

static void *buffer[BUFFER_SIZE];
static int position_insert, position_extract, item_counter = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void bbuff_init(void)
{
    // initialize buffer
    position_insert = 0;
    position_extract = 0;
    item_counter = 0;
    pthread_mutex_init(&mutex, NULL);
}

void bbuff_blocking_insert(void *item)
{
    // insert item into buffer
    pthread_mutex_lock(&mutex);
    buffer[position_insert] = item;
    position_insert = (position_insert + 1) % BUFFER_SIZE;
    item_counter++;
    pthread_mutex_unlock(&mutex);
}

void *bbuff_blocking_extract(void)
{
    // extract item from buffer
    pthread_mutex_lock(&mutex);
    void *item = buffer[position_extract];
    buffer[position_extract] = NULL;
    position_extract = (position_extract + 1) % BUFFER_SIZE;
    item_counter--;
    pthread_mutex_unlock(&mutex);
    return item;
}

_Bool bbuff_is_empty(void)
{
    if (item_counter == 0)
        return true;
    else
        return false;
}