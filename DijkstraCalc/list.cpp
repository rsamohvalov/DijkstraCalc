#include "list.h"
#include <stdlib.h>
#include <malloc.h>

node* node_new( void* data, bool deletable = false ) {
    node* new_node = (node*)malloc( sizeof( node ) );
    if( !new_node ) {
        return NULL;
    }
    new_node->data = element;
    new_node->delete_on_free = deletable;
    new_node->prev = NULL;
    new_node->next = NULL;
}

list* list_new( void ) {
    list* ret = (list*)malloc( sizeof( list ) );
    if( ret ) {
        ret->begin = ret->end = NULL;
        ret->count = 0;
    }
    return ret;
 }

bool list_is_empty( list* dest ) {
    if( dest ) {
        return !dest->count;
    }
}

int list_add_to_begin( list* dest, void* element, bool deletable = false ) {
    if( !dest || !element) {
        return -1;
    }
    node* new_node = node_new( element, deletable );
    if( !new_node ) {
        return -2;
    }   
    if( dest->begin ) {
        dest->begin->prev = new_node;
        new_node->next = dest->begin;
        dest->begin = new_node;
        dest->count++;
    }
    else {
        dest->begin = dest->end = new_node;
        dest->count++;
    }
    return 0;
}

int list_add_to_end( list* dest, void* element, bool deletable = false ) {
    if( !dest || !element ) {
        return -1;
    }
    node* new_node = node_new( element, deletable );
    if( !new_node ) {
        return -2;
    }
    new_node->data = element;
    new_node->prev = NULL;
    new_node->next = NULL;
    if( dest->end ) {
        dest->end->next = new_node;
        new_node->prev = dest->end;
        dest->end = new_node;
        dest->count++;
    }
    else {
        dest->begin = dest->end = new_node;
        dest->count++;
    }
    return 0;
}

void* list_remove( list* dest, node* what ) {
    if( !dest || !what ) {
        return NULL;
    }
    void* ret = what->data;
    if( what->prev ) {
            if( what->next ) { //inside
                what->prev->next = what->next;
                what->next->prev = what->prev;
            }
            else { //last
                what->prev->next = NULL;
                dest->end = what->prev; 
            }
        }
        else {
            if( what->next ) { //first
                what->next->prev = NULL;
                dest->begin = what->next;
            }
            else {
                dest->begin = dest->end = NULL;
            }
        }
    dest->count--;
    free( what );
    return ret;
}

node* list_find_by_ref( list* dest, void* data ) {
    if( !dest || !data ) {
        return NULL;
    }
    node* cur_node = dest->begin;
    while( cur_node ) {
        if( cur_node->data == data ) {
            return cur_node;
        }
        cur_node = cur_node->next;
    }
    return NULL;
}

node* list_find_with_comparer( list* dest, void* data, comparer_func comparer ) {
    if( !dest || !data ) {
        return NULL;
    }
    node* cur_node = dest->begin;
    while( cur_node ) {
        if( comparer( cur_node->data,data ) ) {
            return cur_node;
        }
        cur_node = cur_node->next;
    }
    return NULL;
}

void list_delete( list* dest ) {
    if( !dest ) {
        return;
    }
    node* cur_node = dest->begin;
    node* temp = NULL;
    while( cur_node ) {
        temp = cur_node->next;
        if( cur_node->delete_on_free ) {
            free( cur_node->data );
        }
        free( cur_node );
        cur_node = temp;
    }
    free( dest );
}