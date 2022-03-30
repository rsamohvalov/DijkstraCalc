#ifndef __LIST_H_
#define __LIST_H_

typedef struct _node {
    void* data;
    bool delete_on_free;
    struct _node* prev;
    struct _node* next;
} node;

typedef struct {
    node* begin;
    node* end;
    unsigned int count;
} list;

typedef bool (*comparer_func)(void*, void*);


node* node_new( void* data, bool deletable );
list* list_new(void);
void list_delete( list* dest );
bool list_is_empty(list* dest);
int list_add_to_begin(list* dest, void* element, bool deletable );
int list_add_to_end( list* dest, void* element, bool deletable );
void* list_remove(list* dest, node* what);
node* list_find_by_ref( list* dest, void* data );


#endif