#include "myalloc.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// Included list structure for the free and allocated memory lists
#include "list.h"

// Included additional header files
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
  struct nodeStruct *allocated_mem, *free_mem; // Lists to record allocated/free memory
};

struct Myalloc myalloc;

void initialize_allocator(int _size, enum allocation_algorithm _aalgorithm)
{
  assert(_size > 0);
  myalloc.aalgorithm = _aalgorithm;
  myalloc.size = _size;
  myalloc.memory = malloc((size_t)myalloc.size);

  // Add some other initialization

  pthread_mutex_init(&mutex, NULL);                // Initialize mutex
  memset(myalloc.memory, 0, (size_t)myalloc.size); // memory is initialized to 0

  myalloc.allocated_mem = NULL; // Initialize allocated memory list to NULL
  myalloc.free_mem = NULL;      // Initialize free memory list to NULL

  struct nodeStruct *node = List_createNode(myalloc.memory); // Create a node for the free memory
  List_insertHead(&myalloc.free_mem, node);                  // Add the node to the free memory list
}

void destroy_allocator()
{
  free(myalloc.memory);

  // free other dynamic allocated memory to avoid memory leak

  List_destroy(&myalloc.allocated_mem); // Destroy allocated memory list
  List_destroy(&myalloc.free_mem);      // Destroy free memory list
  pthread_mutex_unlock(&mutex);         // Unlock mutex before destroying
  pthread_mutex_destroy(&mutex);        // Destroy mutex
}

void *allocate(int _size)
{
  void *ptr = NULL;
  if (_size <= 0) // Invalid size
    return ptr;

  // Allocate memory from myalloc.memory
  // ptr = address of allocated memory

  pthread_mutex_lock(&mutex); // Lock mutex before allocating memory

  // Find the first node in the free memory list that has enough space to allocate
  if (myalloc.aalgorithm == FIRST_FIT)
  {
    // TODO: Implement first fit
  }
  else if (myalloc.aalgorithm == BEST_FIT)
  {
    // TODO: Implement best fit
  }
  else if (myalloc.aalgorithm == WORST_FIT)
  {
    // TODO: Implement worst fit
  }

  pthread_mutex_unlock(&mutex); // Unlock mutex after allocating memory
  return ptr;
}

void deallocate(void *_ptr)
{
  assert(_ptr != NULL);

  // Free allocated memory
  // Note: _ptr points to the user-visible memory. The size information is
  // stored at (char*)_ptr - 8.

  pthread_mutex_lock(&mutex); // Lock mutex before allocating memory

  // TODO: Implement deallocate

  pthread_mutex_unlock(&mutex); // Unlock mutex after allocating memory
}

int compact_allocation(void **_before, void **_after)
{
  int compacted_size = 0;

  // compact allocated memory
  // update _before, _after and compacted_size

  pthread_mutex_lock(&mutex); // Lock mutex before allocating memory

  // TODO: Implement compact_allocation

  pthread_mutex_unlock(&mutex); // Unlock mutex after allocating memory
  return compacted_size;
}

int available_memory()
{
  int available_memory_size = 0;
  // Calculate available memory size
  pthread_mutex_lock(&mutex); // Lock mutex before accessing free memory list
  struct nodeStruct *current = myalloc.free_mem;
  while (current != NULL)
  {
    available_memory_size += (int)*(long *)(current->data - 8) - 8;
    current = current->data;
  }
  pthread_mutex_unlock(&mutex); // Unlock mutex after accessing free memory list
  return available_memory_size;
}

void get_statistics(struct Stats *_stat)
{
  // Populate struct Stats with the statistics
  // TODO: Implement get_statistics
}
