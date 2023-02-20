#ifndef __LIST_H__
#define __LIST_H__

#include <stddef.h>
#include <stdlib.h>

struct headerBlock
{
    size_t size;
};

struct memoryBlock
{
    void *size;
    struct memoryBlock *next;
};

struct memoryBlock *List_createBlock(void *chunk);
void List_insertBlock(struct memoryBlock **headRef, struct memoryBlock *chunk);
struct memoryBlock *List_findBlock(struct memoryBlock *head, void *chunk);
void List_deleteBlock(struct memoryBlock **headRef, struct memoryBlock *chunk);
void List_destroy(struct memoryBlock **headRef);

#endif