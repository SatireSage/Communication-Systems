#ifndef __LIST_H__
#define __LIST_H__

#include <stddef.h>
#include <stdlib.h>

struct list
{
    void *data_chunk;
    struct list *next_chunk;
    struct list *prev_chunk;
};

struct list *create_node(void *chunk);                   // Create a node for the free memory
void add_node(struct list **head, struct list *node);    // Add the node to the free memory list
void remove_node(struct list **head, struct list *node); // Remove the node from the free memory list
void destroy_list(struct list **head);                   // Destroy the list passed in
struct list *list_search(struct list *head, void *node); // Search for a node in the list
void sort_list_ascending(struct list **head);            // Sort the list in ascending order -- Used for first fit
void sort_list_descending(struct list **head);           // Sort the list in descending order -- Used for worst fit

#endif