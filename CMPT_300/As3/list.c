#include "list.h"

/*
 * Allocate memory for a node of type struct nodeStruct and initialize
 * it with the value item. Return a pointer to the new node.
 */
struct memoryBlock *List_createBlock(void *chunk)
{
    struct memoryBlock *node = malloc(sizeof(struct memoryBlock));
    node->size = chunk;
    node->next = NULL;
    return node;
}

/*
 * Insert node into the list in sorted order. The list is sorted order
 */
void List_insertBlock(struct memoryBlock **headRef, struct memoryBlock *chunk)
{
    struct memoryBlock *current = *headRef;
    // If the list is empty or the node to be inserted is smaller than the head then insert it at the head
    if (current == NULL || chunk->size < current->size)
    {
        chunk->next = current;
        *headRef = chunk;
    }
    else
    {
        // Find the node before the node to be inserted and insert it to keep the list intact and sorted
        while (current->next != NULL && current->next->size < chunk->size)
            current = current->next;
        chunk->next = current->next;
        current->next = chunk;
    }
}

/*
 * Return the first node holding the value item, return NULL if none found
 */
struct memoryBlock *List_findBlock(struct memoryBlock *head, void *chunk)
{
    struct memoryBlock *temp = head;
    while (temp != NULL)
    {
        if (temp->size == chunk)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

/*
 * Delete node from the list and free memory allocated to it.
 */
void List_deleteBlock(struct memoryBlock **headRef, struct memoryBlock *chunk)
{
    if (*headRef == chunk)
        if (chunk->next == NULL)
            *headRef = NULL;
        else
            *headRef = chunk->next;
    else
    {
        struct memoryBlock *current = *headRef;
        while (current->next != chunk)
            current = current->next;
        current->next = chunk->next;
    }
}

/*
 * Delete the entire list and free memory allocated to each node.
 */
void List_destroy(struct memoryBlock **headRef)
{
    while (*headRef != NULL)
    {
        struct memoryBlock *temp = *headRef;
        *headRef = (*headRef)->next;
        free(temp);
    }
    *headRef = NULL;
}