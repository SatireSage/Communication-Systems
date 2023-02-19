#ifndef BBUFF_H
#define BBUFF_H

#include <stdbool.h>
#include <stddef.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 10

void bbuff_init(void);
void bbuff_cleanup(void);
void bbuff_blocking_insert(void *item);
void *bbuff_blocking_extract(void);
_Bool bbuff_is_empty(void);

#endif