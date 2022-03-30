
#include <iostream>
#include <stdlib.h>
#include "list.h"
#include "stack.h"
#include "queue.h"

char priority[256] = { 0 };

enum {
    Unknown,
    ReadingFirstOperand,
    ReadingOperation,
    ReadingSecondOperand
};

typedef int (*operation)( int first, int second );

int my_add( int first, int second ) {
    return first + second;
}

int my_sub( int first, int second) {
    return first - second;
}

int my_div( int first, int second ) {
    return first/second;
}

int my_mul( int first, int second ) {
    return first * second;
}

typedef enum {
    none,
    arg,
    oper
} data_type;

typedef struct _element {
    data_type d_type;
    union {
        int number;
        operation func;
    } data;
} element;

void pop_operations( char operation, stack* my_stack, char* output_buffer, int* output_buffer_index, stack* count_stack ) {
    char* data = (char*)stack_peek( my_stack );
    while( data ) {
        if( priority[*data] >= priority[operation] ) {
            data = ( char* ) stack_pop( my_stack );

            element* new_element = ( element* ) malloc( sizeof( element ) );
            if( !new_element ) {
                printf( "Not enough memory to complete...\n" );
                return;
            }
            new_element->d_type = oper;
            switch( *data ) {
                case '+': {
                    new_element->data.func = my_add;
                    break;
                }
                case '-': {
                    new_element->data.func = my_sub;
                    break;
                }
                case ':':
                case '/': {
                    new_element->data.func = my_div;
                    break;
                }
                case '*': {
                    new_element->data.func = my_mul;
                    break;
                }
            }            
            stack_push( count_stack, new_element );

            output_buffer[*output_buffer_index] = *data;
            (*output_buffer_index)++;
        }
        else {
            break;
        }
        data = (char*)stack_peek( my_stack );
    }
}

int take_data_from_parse_buffer( char* parse_buffer, int* parse_buffer_index, stack* count_stack ) {
    if( *parse_buffer_index ) {
        element* new_element = ( element* ) malloc( sizeof( element ) );
        if( !new_element ) {
            printf( "Not enough memory to complete...\n" );
            return -1;
        }
        new_element->d_type = arg;
        new_element->data.number = atoi( parse_buffer );
        stack_push( count_stack, new_element );
    }
    memset( parse_buffer, 0, *parse_buffer_index + 1 );
    *parse_buffer_index = 0;
    return 0;
}

int count( stack* count_stack ) {
    int ret = 0;
    if( stack_count( count_stack ) == 1 ) {
        return 0;
    }
    element* data = (element*)stack_pop( count_stack );
    if( data->d_type == oper ) {
        element* first = (element*)stack_peek( count_stack );
        if( first ) {
            if( first->d_type == arg ) {
                first = (element*)stack_pop( count_stack );
                element* second = (element*)stack_peek( count_stack );
                if( second->d_type == arg ) {
                    second = (element*)stack_pop( count_stack );
                    ret = data->data.func( second->data.number, first->data.number );
                    free( second );

                    element* new_element = ( element* ) malloc( sizeof( element ) );
                    if( !new_element ) {
                        printf( "Not enough memory to complete...\n" );
                        return -1;
                    }
                    new_element->d_type = arg;
                    new_element->data.number = ret;
                    stack_push( count_stack, new_element );

                    return 0;
                }
                else {
                    ret = count( count_stack );
                    second = ( element* ) stack_pop( count_stack );
                    ret = data->data.func( second->data.number, first->data.number );
                    free( second );
                    element* new_element = ( element* ) malloc( sizeof( element ) );
                    if( !new_element ) {
                        printf( "Not enough memory to complete...\n" );
                        return -1;
                    }
                    new_element->d_type = arg;
                    new_element->data.number = ret;
                    stack_push( count_stack, new_element );
                }
                free( first );
            }
            else {
                ret = count( count_stack );
                first = ( element* ) stack_pop( count_stack );
                element* second = ( element* ) stack_peek( count_stack );
                if( second->d_type == arg ) {
                    second = ( element* ) stack_pop( count_stack );
                    ret = data->data.func( second->data.number, first->data.number );
                    free( second );

                    element* new_element = ( element* ) malloc( sizeof( element ) );
                    if( !new_element ) {
                        printf( "Not enough memory to complete...\n" );
                        return -1;
                    }
                    new_element->d_type = arg;
                    new_element->data.number = ret;
                    stack_push( count_stack, new_element );

                }
                else {
                    ret = count( count_stack );
                    second = ( element* ) stack_pop( count_stack );
                    ret = data->data.func( second->data.number, first->data.number );
                    free( second );
                    element* new_element = ( element* ) malloc( sizeof( element ) );
                    if( !new_element ) {
                        printf( "Not enough memory to complete...\n" );
                        return -1;
                    }
                    new_element->d_type = arg;
                    new_element->data.number = ret;
                    stack_push( count_stack, new_element );
                }
                free( first );
            }
        }
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

    char mul = '*';

    priority['('] = 1;
    priority[')'] = 2;
    priority['+'] = 3;
    priority['-'] = 4;
    priority['*'] = 5;
    priority['/'] = 5;
    priority[':'] = 5;

    int input_state = Unknown;
    
    stack* my_stack = stack_new( );
    stack* count_stack = stack_new();

    char parse_buffer[256] = {0};
    int parse_buffer_index = 0;


    gets_s( input_buffer, 4095 );
    int input_size = strlen( input_buffer );
    while( input_buffer_index < input_size ) {
        if( input_buffer[input_buffer_index] == ' ' ) {
            if( take_data_from_parse_buffer( parse_buffer, &parse_buffer_index, count_stack ) ) {
                return -1;
            }
            input_buffer_index++;
            continue;
        }
        if( input_buffer[input_buffer_index] >= 0x30 && input_buffer[input_buffer_index] <= 0x39 ) {
            if( input_state == ReadingOperation ) {
                input_state = ReadingSecondOperand;
            }
            else {
                input_state - ReadingFirstOperand;
            }
            parse_buffer[parse_buffer_index] = input_buffer[input_buffer_index];
            parse_buffer_index++;
            output_buffer[output_buffer_index] = input_buffer[input_buffer_index];
            input_buffer_index++;
            output_buffer_index++;
            continue;
        }
        switch( input_buffer[input_buffer_index] ) {
        case '-': 
        case '+':
        case '*':
        case ':':
        case '/': {
            if( take_data_from_parse_buffer( parse_buffer, &parse_buffer_index, count_stack ) ) {
                return -1;
            }
            pop_operations( input_buffer[input_buffer_index], my_stack, output_buffer, &output_buffer_index, count_stack );
            stack_push( my_stack, &(input_buffer[input_buffer_index]) );
            input_state == ReadingOperation;
            //output_buffer[output_buffer_index] = ' ';
            //output_buffer_index++;
            break;
        }
        case '(': {
            stack_push( my_stack, &(input_buffer[input_buffer_index]) );
            break;
        }
        case ')': {
            if( take_data_from_parse_buffer( parse_buffer, &parse_buffer_index, count_stack ) ) {
                return -1;
            }

            char* data = ( char* ) stack_pop( my_stack );
            while( data ) {
                if( *data == '(' ) {
                    break;
                }

                element* new_element = ( element* ) malloc( sizeof( element ) );
                if( !new_element ) {
                    printf( "Not enough memory to complete...\n" );
                    return -1;
                }
                new_element->d_type = oper;
                switch( *data ) {
                case '+': {
                    new_element->data.func = my_add;
                    break;
                }
                case '-': {
                    new_element->data.func = my_sub;
                    break;
                }
                case ':':
                case '/': {
                    new_element->data.func = my_div;
                    break;
                }
                case '*': {
                    new_element->data.func = my_mul;
                    break;
                }
                }
                stack_push( count_stack, new_element );
                
                output_buffer[output_buffer_index] = *data;
                output_buffer_index++;
                data = ( char* ) stack_pop( my_stack );
            }
            break;
        }
        default: {
            printf( "error in writing in symbol %c\n", input_buffer[input_buffer_index] );
            getchar( );
            return -1;
        }
        }
        input_buffer_index++;
    }

    if( take_data_from_parse_buffer( parse_buffer, &parse_buffer_index, count_stack ) ) {
        return -1;
    }


    if( stack_count( my_stack ) ) {
        char* data = ( char* ) stack_pop( my_stack );
        while( data ) {
            if( *data == '(' ) {
                printf( "error in writing in symbol %c\n", input_buffer[input_buffer_index] );
                getchar( );
                return -1;
            }

            element* new_element = ( element* ) malloc( sizeof( element ) );
            if( !new_element ) {
                printf( "Not enough memory to complete...\n" );
                return -1;
            }
            new_element->d_type = oper;
            switch( *data ) {
            case '+': {
                new_element->data.func = my_add;
                break;
            }
            case '-': {
                new_element->data.func = my_sub;
                break;
            }
            case ':':
            case '/': {
                new_element->data.func = my_div;
                break;
            }
            case '*': {
                new_element->data.func = my_mul;
                break;
            }
            }
            stack_push( count_stack, new_element );

            output_buffer[output_buffer_index] = *data;
            output_buffer_index++;
            data = ( char* ) stack_pop( my_stack );
        }
    }

    printf( "\n%s", output_buffer );

    count( count_stack );
    if( stack_count( count_stack ) == 1 ) {
        element* result = (element*)stack_pop(count_stack );
        printf( " = %d", result->data.number );
    }


    getchar( );
    return 0;







    gets_s(input_buffer, 4095 );
    //int input_size = strlen( input_buffer );
    while( input_buffer_index < input_size) {
        if( input_buffer[input_buffer_index] == ' ' ) {
            input_buffer_index++;
            continue;
        }
        if( input_buffer[input_buffer_index] >= 0x30 && input_buffer[input_buffer_index] <= 0x39 ) {
            if( input_state == ReadingOperation ) {
                input_state = ReadingSecondOperand;
            }
            else {
                input_state - ReadingFirstOperand;
            }
            output_buffer[output_buffer_index] = input_buffer[input_buffer_index];
            input_buffer_index++;
            output_buffer_index++;
            continue;
        }
        switch( input_buffer[input_buffer_index] ) {
            case '-': {
                if( input_state == Unknown || input_state == ReadingOperation || input_state == ReadingSecondOperand) {
                    output_buffer[output_buffer_index] = '-';                    
                    output_buffer_index++;
                    output_buffer[output_buffer_index] = '1';
                    output_buffer_index++;
                    stack_push( my_stack, &mul );
                    input_buffer_index++;
                    continue;
                }
                else {
                    pop_operations( '-', my_stack, output_buffer, &output_buffer_index, count_stack );
                    stack_push( my_stack, &(input_buffer[input_buffer_index]) );
                    break;
                }
            }
            case '+':
            case '*':
            case ':':
            case '/': {
                pop_operations( '*', my_stack, output_buffer, &output_buffer_index, count_stack );
                stack_push( my_stack, &(input_buffer[input_buffer_index]) );
                input_state == ReadingOperation;
                output_buffer[output_buffer_index] = ' ';
                output_buffer_index++;
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
    if( stack_count(my_stack) ) {
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
