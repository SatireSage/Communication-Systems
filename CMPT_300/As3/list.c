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
 * Insert node into the list in sorted order. The list is sorted in ascending order.
 */
void List_insert(struct nodeStruct **headRef, struct nodeStruct *node)
{
    // If the list is empty or the node should be inserted at the head then insert at the head
    if (*headRef == NULL || (*headRef)->data > node->data)
    {
        node->next = *headRef;
        *headRef = node;
    }
    else
    {
        struct nodeStruct *current = *headRef;
        struct nodeStruct *next = (*headRef)->next;
        // Find the node after which the new node should be inserted and insert it
        while (next != NULL && next->data < node->data)
        {
            current = next;
            next = next->next;
        }
        current->next = node;
        current->next->next = next;
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
    while (temp->data != node)
        temp = temp->next;
    return temp;
}

/*
 * Delete node from the list and free memory allocated to it.
 */
void List_deleteNode(struct nodeStruct **headRef, struct nodeStruct *node)
{
    // If the node to be deleted is the head then delete the head
    if (*headRef == node)
        *headRef = node->next;
    else
    {
        struct nodeStruct *current = *headRef;
        // Find the node before the node to be deleted and delete it to keep the list intact and sorted
        while (current->next != NULL && current->next != node)
            current = current->next;
        current->next = current->next->next;
    }
}