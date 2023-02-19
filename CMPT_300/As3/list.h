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
void List_insert(struct nodeStruct **headRef, struct nodeStruct *chunk);     // Add the node to the list
void List_destroy(struct nodeStruct **headRef);                              // Destroy the list passed in
struct nodeStruct *List_findNode(struct nodeStruct *head, void *node);       // Search for a node in the list
void List_deleteNode(struct nodeStruct **headRef, struct nodeStruct *chunk); // Delete a node from the list

#endif