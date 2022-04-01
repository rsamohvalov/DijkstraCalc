
//#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "stack.h"

//typedef int (*operation)( int first, int second );
typedef float (*operation)(float first, float second);

float my_add( float first, float second ) {
    return first + second;
}

float my_sub( float first, float second) {
    return first - second;
}

float my_div( float first, float second ) {
    return first/second;
}

float my_mul( float first, float second ) {
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
        float number;
        operation func;
    } data;
} element;

int push_arg_element( stack* count_stack, float number ) {
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

void pop_operations( char operation, char* priority, stack* my_stack, char* output_buffer, int* output_buffer_index, stack* count_stack, char to_out_buffer ) {
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

            if( to_out_buffer ) {
                output_buffer[*output_buffer_index] = *data;
                (*output_buffer_index)++;
            }
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
        new_element->data.number = (float)atof( parse_buffer );
        if( !new_element->data.number ) {
            free( new_element );
            return -1;
        }
        stack_push( count_stack, new_element );
    }
    memset( parse_buffer, 0, *parse_buffer_index + 1 );
    *parse_buffer_index = 0;
    return 0;
}



int count( stack* count_stack ) {
    int ret = 0;
    float result = 0;
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
                    result = data->data.func( second->data.number, first->data.number );
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
                    result = data->data.func( second->data.number, first->data.number );
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
                    result = data->data.func( second->data.number, first->data.number );
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
                    result = data->data.func( second->data.number, first->data.number );
                    free( second );                            
                }
                free( first );
            }
            if( push_arg_element( count_stack, result ) ) {
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

    char closing_bracket = 0;
    char mul = '*';

    while( 1 ) {

        closing_bracket = 0;

        stack* my_stack = stack_new( );
        stack* count_stack = stack_new( );
        memset( output_buffer, 0, output_buffer_index );
        output_buffer_index = 0;
        memset( parse_buffer, 0, parse_buffer_index );
        parse_buffer_index = 0;
        input_buffer_index = 0;

        printf( "Type your expression, \"quit\" for exit\n" );
        fgets( input_buffer, 4095, stdin );
#if defined(WIN32)
        if( !_strnicmp( input_buffer, "quit", 4095 ) ) {
#else
        if( !strncasecmp( input_buffer, "quit", 4095 ) ) {
#endif
            break;
        }

        int input_size = strlen( input_buffer );
        while( input_buffer_index < input_size ) {
            if( input_buffer[input_buffer_index] == ' ' ) {
                input_buffer_index++;
                continue;
            }
            if( input_buffer[input_buffer_index] == 0x0A || input_buffer[input_buffer_index] == 0x0D ) {
                break;
            }
            if( (input_buffer[input_buffer_index] >= 0x30 && input_buffer[input_buffer_index] <= 0x39) || input_buffer[input_buffer_index] == ',' || input_buffer[input_buffer_index] == '.' ) {
                parse_buffer[parse_buffer_index] = input_buffer[input_buffer_index];
                parse_buffer_index++;
                output_buffer[output_buffer_index] = input_buffer[input_buffer_index];
                input_buffer_index++;
                output_buffer_index++;

                closing_bracket = 0;

                continue;
            }
            switch( input_buffer[input_buffer_index] ) {
            case '-': {
                if( !parse_buffer_index && !closing_bracket ) {
                    input_buffer_index++;
                    
#if defined(WIN32)                    
                    strcpy_s( parse_buffer, 255, "-1" );
#else
                    strcpy( parse_buffer, "-1" );
#endif

                    parse_buffer_index = 1;
                    if( take_data_from_parse_buffer( parse_buffer, &parse_buffer_index, count_stack ) ) {
                        printf( "parsing error in %s. Exiting. Press enter...\n", parse_buffer );
                        fgetc( stdin );
                        return -1;
                    }
                  
#if defined(WIN32)                  
                    strcpy_s( output_buffer + output_buffer_index, 4096, "(-1)" );
#else
                    strcpy( output_buffer + output_buffer_index, "(-1)" );
#endif

                    output_buffer_index += strlen( "(-1)" );                    
                    stack_push( my_stack, &(mul) );                    
                    continue;
                }
                else {
                    if( take_data_from_parse_buffer( parse_buffer, &parse_buffer_index, count_stack ) ) {
                        printf( "parsing error in %s. Exiting. Press enter...\n", parse_buffer );
                        fgetc( stdin );
                        return -1;
                    }
                    pop_operations( input_buffer[input_buffer_index], priority, my_stack, output_buffer, &output_buffer_index, count_stack, 1 );
                    stack_push( my_stack, &(input_buffer[input_buffer_index]) );
                    closing_bracket = 0;
                    break;
                }
            }
            case '+': {
                if( !parse_buffer_index && !closing_bracket ) {
                    input_buffer_index++;
                    continue;
                }
            }
            case '*':
            case ':':
            case '/': {
                if( take_data_from_parse_buffer( parse_buffer, &parse_buffer_index, count_stack ) ) {
                    printf( "parsing error in %s. Exiting. Press enter...\n", parse_buffer );
                    fgetc( stdin );
                    return -1;
                }
                pop_operations( input_buffer[input_buffer_index], priority, my_stack, output_buffer, &output_buffer_index, count_stack, 1 );
                stack_push( my_stack, &(input_buffer[input_buffer_index]) );
                closing_bracket = 0;
                break;
            }
            case '(': {
                if( take_data_from_parse_buffer( parse_buffer, &parse_buffer_index, count_stack ) ) {
                    printf( "parsing error in %s. Exiting. Press enter...\n", parse_buffer );
                    fgetc( stdin );
                    return -1;
                }
                stack_push( my_stack, &(input_buffer[input_buffer_index]) );
                closing_bracket = 0;
                break;
            }
            case ')': {
                closing_bracket = 1;
                if( take_data_from_parse_buffer( parse_buffer, &parse_buffer_index, count_stack ) ) {
                    printf( "parsing error in %s. Exiting. Press enter...\n", parse_buffer );
                    fgetc( stdin );
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
                printf( "Error in expression, symbol %c not suitable. Press enter...\n", input_buffer[input_buffer_index] );
                fgetc(stdin );
                return -1;
            }
            }
            input_buffer_index++;
        }

        if( take_data_from_parse_buffer( parse_buffer, &parse_buffer_index, count_stack ) ) {
            printf( "parsing error in %s. Exiting. Press enter...\n", parse_buffer );
            fgetc( stdin );
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
        output_buffer[output_buffer_index] = 0;
        printf( "Reverse polish notaion: %s", output_buffer );

        if( count( count_stack ) ) {
            printf( "Error!\n" );
        }
        else {
            if( stack_count( count_stack ) == 1 ) {
                element* result = ( element* ) stack_pop( count_stack );
                printf( " = %.2f\n\n", result->data.number );
                free( result );
            }
        }

        stack_delete( count_stack );
    }
    return 0;
}
