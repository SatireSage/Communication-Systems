#include "myalloc.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// Included list structure for the free and allocated memory lists
#include "list.h"

// Included additional header files
#include <string.h>
#include <pthread.h>

#define HEADER_SIZE sizeof(size_t) // Size of header: 8 bytes (unsigned long)

// Mutex for thread safety of the allocator
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct Myalloc
{
  enum allocation_algorithm aalgorithm;
  int size;
  void *memory;
  // Some other data members you want,
  // such as lists to record allocated/free memory
  struct memoryBlock *allocated, *free;
};

struct Myalloc myalloc;

void initialize_allocator(int _size, enum allocation_algorithm _aalgorithm)
{
  assert(_size > 0);
  myalloc.aalgorithm = _aalgorithm;
  myalloc.size = _size;
  myalloc.memory = malloc((size_t)myalloc.size);

  // Add some other initialization

  pthread_mutex_init(&mutex, NULL);        // Initialize mutex
  memset(myalloc.memory, 0, myalloc.size); // Initialize memory to 0

  myalloc.allocated = NULL; // Initialize allocated list to NULL
  myalloc.free = NULL;      // Initialize free list to NULL

  // Add the initial header to the free list
  struct memoryBlock *chunk = List_createBlock(myalloc.memory + HEADER_SIZE);
  List_insertBlock(&myalloc.free, chunk);

  // Add the size of header to the memory
  struct headerBlock header;
  header.size = (size_t)myalloc.size - HEADER_SIZE;
  memcpy(myalloc.memory, &header, HEADER_SIZE);
}

void destroy_allocator()
{
  free(myalloc.memory);

  // free other dynamic allocated memory to avoid memory leak
  List_destroy(&myalloc.allocated); // Destroy allocated memory list
  List_destroy(&myalloc.free);      // Destroy free memory list
  pthread_mutex_unlock(&mutex);     // Unlock mutex before destroying
  pthread_mutex_destroy(&mutex);    // Destroy mutex
}

void *allocate(int _size)
{
  void *ptr = NULL;

  // Allocate memory from myalloc.memory
  // ptr = address of allocated memory

  pthread_mutex_lock(&mutex); // Lock mutex before allocating

  // TODO: Implement allocation

  pthread_mutex_unlock(&mutex); // Unlock mutex after allocating
  return ptr;
}

void deallocate(void *_ptr)
{
  assert(_ptr != NULL);

  // Free allocated memory
  // Note: _ptr points to the user-visible memory. The size information is
  // stored at (char*)_ptr - 8.

  pthread_mutex_lock(&mutex); // Lock mutex before deallocation

  // TODO: Implement deallocation

  pthread_mutex_unlock(&mutex); // Unlock mutex after deallocation
}

int compact_allocation(void **_before, void **_after)
{
  int compacted_size = 0;

  // compact allocated memory
  // update _before, _after and compacted_size

  pthread_mutex_lock(&mutex); // Lock mutex before reallocating

  // TODO: Implement reallocating

  pthread_mutex_unlock(&mutex); // Unlock mutex after reallocating
  return compacted_size;
}

int available_memory()
{
  int available_memory_size = 0;
  // Calculate available memory size
  pthread_mutex_lock(&mutex); // Lock mutex before calculating
  struct memoryBlock *current = myalloc.free;
  while (current != NULL)
  {
    available_memory_size += (int)((struct headerBlock *)(current->size - HEADER_SIZE))->size;
    current = current->next;
  }
  pthread_mutex_unlock(&mutex); // Unlock mutex after calculating
  return available_memory_size;
}

void get_statistics(struct Stats *_stat)
{
  // Populate struct Stats with the statistics
  int allocated_size = 0;
  int allocated_chunks = 0;
  int free_size = 0;
  int free_chunks = 0;
  int smallest_free_chunk_size = myalloc.size;
  int largest_free_chunk_size = 0;

  pthread_mutex_lock(&mutex); // Lock mutex before accessing memory

  struct memoryBlock *current_allocated = myalloc.allocated;
  struct memoryBlock *current_free = myalloc.free;

  // Calculate allocated size and chunks
  while (current_allocated != NULL)
  {
    allocated_size += (int)((struct headerBlock *)(current_allocated->size - HEADER_SIZE))->size;
    allocated_chunks++;
    current_allocated = current_allocated->next;
  }

  // Calculate free size and chunks
  while (current_free != NULL)
  {
    free_size += (int)((struct headerBlock *)(current_free->size - HEADER_SIZE))->size;
    int temp = (int)((struct headerBlock *)(current_free->size - HEADER_SIZE))->size;
    if (temp < smallest_free_chunk_size)
      smallest_free_chunk_size = temp;
    if (temp > largest_free_chunk_size)
      largest_free_chunk_size = temp;
    free_chunks++;
    current_free = current_free->next;
  }

  pthread_mutex_unlock(&mutex); // Unlock mutex after accessing memory

  // Populate stats
  _stat->allocated_size = allocated_size;
  _stat->allocated_chunks = allocated_chunks;
  _stat->free_size = free_size;
  _stat->free_chunks = free_chunks;
  _stat->smallest_free_chunk_size = smallest_free_chunk_size;
  _stat->largest_free_chunk_size = largest_free_chunk_size;
}