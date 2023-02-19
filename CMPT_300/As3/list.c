#include "list.h"

struct list
{
    void *data_chunk;
    struct list *next_chunk;
    struct list *prev_chunk;
};

struct list *create_node(void *chunk)
{
    struct list *new_node = malloc(sizeof(struct list));
    new_node->data_chunk = chunk;
    new_node->next_chunk = NULL;
    new_node->prev_chunk = NULL;
    return new_node;
}

void add_node(struct list **head, struct list *node)
{
    if (*head == NULL)
        *head = node;
    else
    {
        struct list *temp = *head;
        while (temp->next_chunk != NULL)
            temp = temp->next_chunk;
        temp->next_chunk = node;
        node->prev_chunk = temp;
    }
}

void remove_node(struct list **head, struct list *node)
{
    if (*head == node)
    {
        if (node->next_chunk == NULL)
            *head = NULL;
        else
        {
            *head = node->next_chunk;
            node->next_chunk->prev_chunk = NULL;
        }
    }
    else
    {
        struct list *temp = *head;
        while (temp->next_chunk != node)
            temp = temp->next_chunk;
        temp->next_chunk = node->next_chunk;
        node->next_chunk->prev_chunk = temp;
    }
    free(node);
}

void destroy_list(struct list **head)
{
    struct list *temp = *head;
    while (temp != NULL)
    {
        struct list *temp2 = temp;
        temp = temp->next_chunk;
        free(temp2);
    }
    *head = NULL;
}

struct list *list_search(struct list *head, void *chunk)
{
    struct list *temp = head;
    while (temp != NULL)
    {
        if (temp->data_chunk == chunk)
            return temp;
        temp = temp->next_chunk;
    }
    return NULL;
}