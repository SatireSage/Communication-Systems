#include "bbuff.h"

static void *buffer[BUFFER_SIZE];
static int empty_num_remaining = 0;
static sem_t full_slots, empty_slots;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void bbuff_init(void)
{
    // Initialize available slots
    empty_num_remaining = BUFFER_SIZE;
    // Initialize semaphores
    sem_init(&full_slots, 0, 0);
    sem_init(&empty_slots, 0, BUFFER_SIZE);
    // Initialize mutex
    pthread_mutex_init(&mutex, NULL);
}

void bbuff_cleanup(void)
{
    // Destroy semaphores
    sem_destroy(&full_slots);
    sem_destroy(&empty_slots);
    // Unlock mutex before destroying
    pthread_mutex_unlock(&mutex);
    // Destroy mutex
    pthread_mutex_destroy(&mutex);
}

void bbuff_blocking_insert(void *item)
{
    // wait for empty slot and lock mutex
    sem_wait(&empty_slots);
    pthread_mutex_lock(&mutex);

    // insert item and update available slots
    int insert_index = BUFFER_SIZE - empty_num_remaining;
    buffer[insert_index] = item;
    empty_num_remaining--;

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
    int index = BUFFER_SIZE - (empty_num_remaining + 1);
    void *item = buffer[index];
    buffer[index] = NULL;
    empty_num_remaining++;

    // unlock mutex and signal empty slot
    pthread_mutex_unlock(&mutex);
    sem_post(&empty_slots);
    // return item extracted
    return item;
}

_Bool bbuff_is_empty(void)
{
    // return true if buffer is empty (if there empty slots is equal to buffer size then buffer is empty)
    return empty_num_remaining == BUFFER_SIZE;
}