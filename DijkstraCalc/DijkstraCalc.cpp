
#include <iostream>
#include "list.h"

int main()
{
    list* my_list = list_new( );
    if( !my_list ) {
        return -1;
    }

    int intarray[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

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

}
