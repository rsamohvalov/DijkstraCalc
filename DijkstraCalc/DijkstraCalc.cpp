
#include <iostream>
#include "list.h"
#include "stack.h"

char priority[256] = { 0 };

void pop_operations( char operation, stack* my_stack, char* output_buffer, int* output_buffer_index ) {
    char* data = (char*)stack_peek( my_stack );
    while( data ) {
        if( priority[*data] >= priority[operation] ) {
            output_buffer[*output_buffer_index] = *(( char* ) stack_pop( my_stack ));
            (*output_buffer_index)++;
        }
        else {
            break;
        }
        data = (char*)stack_peek( my_stack );
    }
}

int main()
{
    list* my_list = list_new( );
    if( !my_list ) {
        return -1;
    }

    int intarray[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    char input_buffer[4096] = {0};
    int input_buffer_index = 0;

    char output_buffer[4096] = {0};
    int  output_buffer_index = 0;

    
    priority['('] = 1;
    priority[')'] = 2;
    priority['+'] = 3;
    priority['-'] = 4;
    priority['*'] = 5;
    priority['/'] = 5;
    priority[':'] = 5;
    
    stack* my_stack = stack_new( );

    gets_s(input_buffer, 4095 );
    int input_size = strlen( input_buffer );
    while( input_buffer_index < input_size) {
        if( input_buffer[input_buffer_index] == ' ' ) {
            input_buffer_index++;
            continue;
        }
        if( input_buffer[input_buffer_index] >= 0x30 && input_buffer[input_buffer_index] <= 0x39 ) {
            output_buffer[output_buffer_index] = input_buffer[input_buffer_index];
            input_buffer_index++;
            output_buffer_index++;
            continue;
        }
        switch( input_buffer[input_buffer_index] ) {
            case '+':
            case '-': {
                pop_operations( '-', my_stack, output_buffer, &output_buffer_index );
                stack_push( my_stack, &(input_buffer[input_buffer_index]) );
                break;
            }
            case '*':
            case ':':
            case '/': {
                pop_operations( '*', my_stack, output_buffer, &output_buffer_index );
                stack_push( my_stack, &(input_buffer[input_buffer_index]) );
                break;
            }
            case '(': {
                stack_push( my_stack, &(input_buffer[input_buffer_index]) );
                break;
            }           
            case ')': {
                char* data = (char*)stack_pop( my_stack );
                while( data ) {
                    if( *data == '(' ) {
                        break;
                    }
                    output_buffer[output_buffer_index] = *data;
                    output_buffer_index++;
                    data = (char*)stack_pop( my_stack );
                }
                break;
            }
            default: {
                printf( "error in writing in symbol %c\n", input_buffer[input_buffer_index] );
                getchar();
                return -1;
            }
        }
        input_buffer_index++;
    }
    if( stack_count ) {
        char* data = ( char* ) stack_pop( my_stack );
        while( data ) {
            if( *data == '(' ) {
                printf( "error in writing in symbol %c\n", input_buffer[input_buffer_index] );
                getchar( );
                return -1;
            }
            output_buffer[output_buffer_index] = *data;
            output_buffer_index++;
            data = ( char* ) stack_pop( my_stack );
        }
    }

    printf( "\n%s\n", output_buffer );
    getchar( );
    return 0;





    stack_push( my_stack, &intarray[0], false );
    stack_push( my_stack, &intarray[1], false );
    stack_push( my_stack, &intarray[2], false );
    stack_push( my_stack, &intarray[3], false );
    stack_push( my_stack, &intarray[4], false );
    stack_push( my_stack, &intarray[5], false );

    while( stack_count(my_stack) ) {
        void* data = stack_pop( my_stack );
        printf( "%d, ", *((int*)data) );
    }
    printf( "\n" );
        
    
    list_add_to_begin( my_list, &intarray[0], false);
    list_add_to_begin( my_list, &intarray[1], false );
    list_add_to_begin( my_list, &intarray[2], false );
    list_add_to_end( my_list, &intarray[8], false );
    list_add_to_end( my_list, &intarray[9], false );

    node* cur = my_list->begin;
    while( cur ) {
        printf( "%d, ", *((int*)cur->data) );
        cur = cur->next;
    }
    printf( "\n" );

    list_remove( my_list, my_list->begin );
    cur = my_list->begin;
    while( cur ) {
        printf( "%d, ", *((int*)cur->data) );
        cur = cur->next;
    }
    printf( "\n" );

    list_remove( my_list, my_list->end );
    cur = my_list->begin;
    while( cur ) {
        printf( "%d, ", *((int*)cur->data) );
        cur = cur->next;
    }

    list_delete( my_list );
    printf( "\n" );

    return 0;
}
