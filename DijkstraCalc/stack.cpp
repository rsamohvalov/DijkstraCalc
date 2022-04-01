#include "stack.h"
#include "list.h"
#include <stdlib.h>
#include <malloc.h>

stack* stack_new( ) {
    stack* ret = (stack*)malloc(sizeof(stack) );
    if( ret ) {
        ret->internal_list = list_new();
        if( !ret->internal_list ) {
            free( ret );
            return NULL;
        }
    }
    return ret;
}

int stack_push( stack* dest, void* data, unsigned char deletable ) {
    if( !dest ) {
        return -1;
    }
    return list_add_to_begin( dest->internal_list, data, deletable );
}

void* stack_pop( stack* dest ) {
    if( !dest ) {
        return NULL;
    }
    return list_remove( dest->internal_list, dest->internal_list->begin );
}

void* stack_peek( stack* dest ) {
    if( !dest ) {
        return NULL;
    }
    if( !dest->internal_list->count ) {
        return NULL;
    }
    return dest->internal_list->begin->data;
}

unsigned int stack_count( stack* dest ) {
    if( !dest ) {
        return 0;
    }
    return dest->internal_list->count;
}

void stack_delete( stack* dest ) {
    if( !dest ) {
        return;
    }
    list_delete( dest->internal_list );
    free( dest );
}