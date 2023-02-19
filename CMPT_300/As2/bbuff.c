#include "bbuff.h"

static void *buffer[BUFFER_SIZE];
static int position_insert, position_extract;
static sem_t full_slots, empty_slots;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void bbuff_init(void)
{
    // Initialize position counters
    position_insert = 0;
    position_extract = 0;
    // Initialize semaphores
    sem_init(&full_slots, 0, 0);
    sem_init(&empty_slots, 0, BUFFER_SIZE);
    // Initialize mutex
    pthread_mutex_init(&mutex, NULL);
}

void bbuff_blocking_insert(void *item)
{
    // wait for empty slot and lock mutex
    sem_wait(&empty_slots);
    pthread_mutex_lock(&mutex);
    // insert item and update position
    buffer[position_insert] = item;
    position_insert = (position_insert + 1) % BUFFER_SIZE;
    // unlock mutex and signal full slot
    pthread_mutex_unlock(&mutex);
    sem_post(&full_slots);
}

void *bbuff_blocking_extract(void)
{
    // wait for full slot and lock mutex
    sem_wait(&full_slots);
    pthread_mutex_lock(&mutex);
    // extract item and update position
    void *item = buffer[position_extract];
    buffer[position_extract] = NULL;
    position_extract = (position_extract + 1) % BUFFER_SIZE;
    // unlock mutex and signal empty slot
    pthread_mutex_unlock(&mutex);
    sem_post(&empty_slots);
    // return item extracted
    return item;
}

_Bool bbuff_is_empty(void)
{
    // return true if buffer is empty (if insert and extract are at the same position then buffer is empty)
    return position_insert == position_extract;
}