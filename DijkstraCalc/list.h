#ifndef __LIST_H_
#define __LIST_H

typedef struct _node {
    void* data;
    struct _node* prev;
    struct _node* next;
} node;

typedef struct {
    node* begin;
    node* end;
    unsigned int count;
} list;


list* new_list(void);
unsigned char is_empty_list(list* dest);
int add_to_list(list* dest, void* element);
void* remove_from_list(list* dest, node* what);
node* find_in_list(list* dest, void* data);

#endif
