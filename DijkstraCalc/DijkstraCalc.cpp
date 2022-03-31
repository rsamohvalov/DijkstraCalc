
#include <iostream>
#include <stdlib.h>
#include "list.h"
#include "stack.h"

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

typedef struct  {
    data_type d_type;
    union {
        int number;
        operation func;
    } data;
} element;

int push_int_element( stack* count_stack, int number ) {
    int ret = 0;
    element* new_element = ( element* ) malloc( sizeof( element ) );
    if( !new_element ) {
        return -1;
    }
    new_element->d_type = arg;
    new_element->data.number = number;
    ret = stack_push( count_stack, new_element );
    if( ret ) {
        free( new_element );
    }
    return ret;
}

int push_oper_element( stack* count_stack, char data ) {
    element* new_element = ( element* ) malloc( sizeof( element ) );
    if( !new_element ) {
        printf( "Not enough memory to complete...\n" );
        return -1;
    }
    new_element->d_type = oper;
    switch( data ) {
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
        default: {
            printf( "wrong operation %d\n", data );
            free( new_element );
            return -1;
        }
    }
    return stack_push( count_stack, new_element );
}

void pop_operations( char operation, char* priority, stack* my_stack, char* output_buffer, int* output_buffer_index, stack* count_stack ) {
    char* data = (char*)stack_peek( my_stack );
    while( data ) {
        if( priority[(unsigned char)*data] >= priority[( unsigned char )operation] ) {
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
    if( !data ) {
        return -1;
    }
    if( data->d_type == oper ) {
        element* first = (element*)stack_peek( count_stack );
        if( first ) {
            if( first->d_type == arg ) {
                first = (element*)stack_pop( count_stack );
                if( !first ) {
                    return -1;
                }
                element* second = (element*)stack_peek( count_stack );
                if( second->d_type == arg ) {
                    second = (element*)stack_pop( count_stack );
                    if( !second ) {
                        return -1;
                    }
                    ret = data->data.func( second->data.number, first->data.number );
                    free( second );
                }
                else {
                    ret = count( count_stack );
                    if( ret ) {
                        return ret;
                    }
                    second = ( element* ) stack_pop( count_stack );
                    if( !second ) {
                        return -1;
                    }
                    ret = data->data.func( second->data.number, first->data.number );
                    free( second );
                }
                free( first );
            }
            else {
                ret = count( count_stack );
                if( ret ) {
                    return -1;
                }
                first = ( element* ) stack_pop( count_stack );
                if( !first ) {
                    return -1;
                }
                element* second = ( element* ) stack_peek( count_stack );
                if( second->d_type == arg ) {
                    second = ( element* ) stack_pop( count_stack );
                    if( !second ) {
                        return -1;
                    }
                    ret = data->data.func( second->data.number, first->data.number );
                    free( second );
                }
                else {
                    ret = count( count_stack );
                    if( ret ) {
                        return -1;
                    }
                    second = ( element* ) stack_pop( count_stack );
                    if( !second ) {
                        return -1;
                    }
                    ret = data->data.func( second->data.number, first->data.number );
                    free( second );                            
                }
                free( first );
            }

            if( push_int_element( count_stack, ret ) ) {
                return -1;
            }

        } 
    }
    free( data );
    return 0;
}

int main()
{
    char input_buffer[4096] = {0};
    int input_buffer_index = 0;

    char output_buffer[4096] = {0};
    int  output_buffer_index = 0;
        
    stack* my_stack = stack_new( );
    stack* count_stack = stack_new();

    char parse_buffer[256] = {0};
    int parse_buffer_index = 0;

    char priority[256] = { 0 };
    priority['('] = 1;
    priority[')'] = 2;
    priority['+'] = 3;
    priority['-'] = 4;
    priority['*'] = 5;
    priority['/'] = 5;
    priority[':'] = 5;

    printf( "Type your expression\n" );
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
                pop_operations( input_buffer[input_buffer_index], priority, my_stack, output_buffer, &output_buffer_index, count_stack );
                stack_push( my_stack, &(input_buffer[input_buffer_index]) );
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
                    push_oper_element( count_stack, *data );
                    output_buffer[output_buffer_index] = *data;
                    output_buffer_index++;
                    data = ( char* ) stack_pop( my_stack );
                }
                break;
            }
            default: {
                printf( "error in writing, symbol %c\n", input_buffer[input_buffer_index] );
                return -1;
            }
        }
        input_buffer_index++;
    }

    if( take_data_from_parse_buffer( parse_buffer, &parse_buffer_index, count_stack ) ) {
        printf( "error\n" );
        return -1;
    }

    if( stack_count( my_stack ) ) {
        char* data = ( char* ) stack_pop( my_stack );
        while( data ) {
            if( *data == '(' ) {
                printf( "error in symbol %c\n", input_buffer[input_buffer_index] );
                return -1;
            }
            push_oper_element( count_stack, *data );
            output_buffer[output_buffer_index] = *data;
            output_buffer_index++;
            data = ( char* ) stack_pop( my_stack );
        }
    }

    stack_delete( my_stack );

    printf( "\nReverse polish notaion: %s", output_buffer );

    if( count( count_stack ) ) {
        printf( "Error during counting expression result!\n" );
    }
    else {
        if( stack_count( count_stack ) == 1 ) {
            element* result = (element*)stack_pop(count_stack );
            printf( " = %d", result->data.number );
            free( result );
        }
    }

    stack_delete( count_stack );

    getchar( );
    return 0;
}
