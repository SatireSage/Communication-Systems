#ifndef __LIST_H__
#define __LIST_H__

#include <stddef.h>

struct list *create_node(void *chunk);
void add_node(struct list **head, struct list *node);
void remove_node(struct list **head, struct list *node);
void destroy_list(struct list **head);
struct list *list_search(struct list *head, void *node);

#endif