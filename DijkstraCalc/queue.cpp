#include "queue.h"
#include "list.h"
#include <stdlib.h>
#include <malloc.h>

queue* queue_new( ) {
    queue* ret = ( queue* ) malloc( sizeof( queue ) );
    if( ret ) {
        ret->internal_list = list_new( );
        if( !ret->internal_list ) {
            free( ret );
            return NULL;
        }
    }
    return ret;
}

int queue_push( queue* dest, void* data, bool deletable ) {
    if( !dest ) {
        return -1;
    }
    return list_add_to_begin( dest->internal_list, data, deletable );
}

void* queue_pop( queue* dest ) {
    if( !dest ) {
        return NULL;
    }
    return list_remove( dest->internal_list, dest->internal_list->end );
}

void* queue_peek( queue* dest ) {
    if( !dest ) {
        return NULL;
    }
    if( !dest->internal_list->count ) {
        return NULL;
    }
    return dest->internal_list->end->data;
}

unsigned int queue_count( queue* dest ) {
    if( !dest ) {
        return 0;
    }
    return dest->internal_list->count;
}

void queue_delete( queue* dest ) {
    if( !dest ) {
        return;
    }
    list_delete( dest->internal_list );
    free( dest );
}