#include "bbuff.h"
#include <stddef.h>
#include <pthread.h>

static void *buffer[BUFFER_SIZE];
static int position = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void bbuff_init(void)
{
    // initialize buffer
    position = 0;
    pthread_mutex_init(&mutex, NULL);
}

void bbuff_blocking_insert(void *item)
{
    // insert item into buffer
    pthread_mutex_lock(&mutex);
    buffer[position] = item;
    position = (position + 1) % BUFFER_SIZE;
    pthread_mutex_unlock(&mutex);
}

void *bbuff_blocking_extract(void)
{
    // check if buffer is empty
    if (bbuff_is_empty())
        return NULL;
    // extract item from buffer
    pthread_mutex_lock(&mutex);
    void *item = buffer[position];
    buffer[position] = NULL;
    position--; // MIGHT USE:   position = (position - 1) % BUFFER_SIZE;
    pthread_mutex_unlock(&mutex);
    return item;
}

bool bbuff_is_empty(void)
{
    if (position == 0)
        return true;
    else
        return false;
}