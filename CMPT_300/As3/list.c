#include "list.h"

/*
 * Allocate memory for a node of type struct nodeStruct and initialize
 * it with the value item. Return a pointer to the new node.
 */
struct nodeStruct *List_createNode(void *chunk)
{
    struct nodeStruct *node = malloc(sizeof(struct nodeStruct));
    node->data = chunk;
    node->next = NULL;
    return node;
}

/*
 * Insert node at the head of the list.
 */
void List_insertHead(struct nodeStruct **headRef, struct nodeStruct *chunk)
{
    if (*headRef == NULL)
        *headRef = chunk;
    else
    {
        chunk->next = *headRef;
        *headRef = chunk;
    }
}

/*
 * Insert node after the tail of the list.
 */
void List_insertTail(struct nodeStruct **headRef, struct nodeStruct *chunk)
{
    if (*headRef == NULL)
        *headRef = chunk;
    else
    {
        struct nodeStruct *current = *headRef;
        while (current->next != NULL)
            current = current->next;
        current->next = chunk;
    }
}

/*
 * Delete the entire list and free memory allocated to each node.
 */
void List_destroy(struct nodeStruct **headRef)
{
    while (*headRef != NULL)
    {
        struct nodeStruct *temp = *headRef;
        *headRef = (*headRef)->next;
        free(temp);
    }
    *headRef = NULL;
}

/*
 * Return the first node holding the value item, return NULL if none found
 */
struct nodeStruct *List_findNode(struct nodeStruct *head, void *node)
{
    struct nodeStruct *temp = head;
    while (temp != NULL)
    {
        if (temp->data == node)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

/*
 * Delete node from the list and free memory allocated to it.
 * This function assumes that node has been properly set to a valid node
 * in the list. For example, the client code may have found it by calling
 * List_findNode(). If the list contains only one node, the head of the list
 * should be set to NULL.
 */
void List_deleteNode(struct nodeStruct **headRef, struct nodeStruct *chunk)
{
    if (*headRef == chunk)
        if (chunk->next == NULL)
            *headRef = NULL;
        else
            *headRef = chunk->next;
    else
    {
        struct nodeStruct *current = *headRef;
        while (current->next != chunk)
            current = current->next;
        current->next = chunk->next;
    }
    free(chunk);
}

/*
 * Sort the list in ascending order based on the item field.
 */
void List_sort(struct nodeStruct **headRef)
{
    struct nodeStruct *current = *headRef;
    while (current != NULL)
    {
        struct nodeStruct *next = current->next;
        while (next != NULL)
        {
            if (current->data > next->data)
            {
                void *temp = current->data;
                current->data = next->data;
                next->data = temp;
            }
            next = next->next;
        }
        current = current->next;
    }
}