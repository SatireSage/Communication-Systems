#ifndef BBUFF_H
#define BBUFF_H

// Source for _Bool definition: https://e2e.ti.com/support/tools/code-composer-studio-group/ccs/f/code-composer-studio-forum/438557/_bool-is-undefined
#if defined(__cplusplus)
typedef bool _Bool;
#endif

#define BUFFER_SIZE 10

void bbuff_init(void);
void bbuff_blocking_insert(void *item);
void *bbuff_blocking_extract(void);
_Bool bbuff_is_empty(void);

#endif