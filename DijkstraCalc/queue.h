#ifndef __QUEUE_H_
#define __QUEUE_H_

#include "list.h"

typedef struct {
    list* internal_list;
} queue;

queue* queue_new( );
int queue_push( queue* dest, void* data, bool deletable = false );
void* queue_pop( queue* dest );
void* queue_peek( queue* dest );
unsigned int queue_count( queue* dest );
void queue_delete( queue* dest );

#endif
