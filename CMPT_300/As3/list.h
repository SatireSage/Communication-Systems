#ifndef __LIST_H__
#define __LIST_H__

#include <stddef.h>
#include <stdlib.h>

struct nodeStruct
{
    void *data;
    struct nodeStruct *next;
};

struct nodeStruct *List_createNode(void *chunk);                             // Create a node for the free memory
void List_insertHead(struct nodeStruct **headRef, struct nodeStruct *chunk); // Add the node to the free memory list at the head
void List_insertTail(struct nodeStruct **headRef, struct nodeStruct *chunk); // Add the node to the free memory list at the tail
void List_destroy(struct nodeStruct **headRef);                              // Destroy the list passed in
struct nodeStruct *List_findNode(struct nodeStruct *head, void *node);       // Search for a node in the list
void List_deleteNode(struct nodeStruct **headRef, struct nodeStruct *chunk); // Delete a node from the list
void List_sort(struct nodeStruct **headRef);                                 // Sort the list in ascending order

#endif