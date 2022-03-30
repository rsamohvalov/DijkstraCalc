#ifndef __STACK_H_
#define __STACK_H_

#include "list.h"

typedef struct {
    list* internal_list;
} stack;

stack* stack_new();
int stack_push(stack* dest, void* data, bool deletable = false );
void* stack_pop( stack* dest );
void* stack_peek( stack* dest );
unsigned int stack_count( stack* dest );
void stack_delete( stack* dest );

#endif