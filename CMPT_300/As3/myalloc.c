#include "myalloc.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"

#include <string.h>
#include <pthread.h>

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct Myalloc
{
  enum allocation_algorithm aalgorithm;
  int size;
  void *memory;
  // Some other data members you want,
  // such as lists to record allocated/free memory
  struct list *allocated_mem;
  struct list *free_mem;
};

struct Myalloc myalloc;

void initialize_allocator(int _size, enum allocation_algorithm _aalgorithm)
{
  assert(_size > 0);
  myalloc.aalgorithm = _aalgorithm;
  myalloc.size = _size;
  myalloc.memory = malloc((size_t)myalloc.size);

  // Add some other initialization
  pthread_mutex_init(&mutex, NULL);
  memset(myalloc.memory, 0, (size_t)myalloc.size);
}

void destroy_allocator()
{
  free(myalloc.memory);

  // free other dynamic allocated memory to avoid memory leak
}

void *allocate(int _size)
{
  void *ptr = NULL;

  // Allocate memory from myalloc.memory
  // ptr = address of allocated memory

  return ptr;
}

void deallocate(void *_ptr)
{
  assert(_ptr != NULL);

  // Free allocated memory
  // Note: _ptr points to the user-visible memory. The size information is
  // stored at (char*)_ptr - 8.
}

int compact_allocation(void **_before, void **_after)
{
  int compacted_size = 0;

  // compact allocated memory
  // update _before, _after and compacted_size

  return compacted_size;
}

int available_memory()
{
  int available_memory_size = 0;
  // Calculate available memory size
  return available_memory_size;
}

void get_statistics(struct Stats *_stat)
{
  // Populate struct Stats with the statistics
}
