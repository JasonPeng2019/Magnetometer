#include <stdio.h>
#include <stdlib.h>

#ifndef BUFF_MAX

#define BUFF_MAX                9


typedef struct point {
    int x;
    int y;
    struct point * next;
    struct point * prev;
} point;

typedef struct buffer_linked_list {
    int count;
    point*sentinel;
    point*start;
    point*end;
    point*midpt;
    void (*push)(struct buffer_linked_list*);
    point* (*add)(struct buffer_linked_list*, point*);
    point* (*search_prior)(struct buffer_linked_list*, point*);
} buffer_linked_list;

point * init_point(int x, int y);
void free_point(point * Point);
buffer_linked_list * init_buffer_LL(void);
void free_buffer_LL(buffer_linked_list * BLL);
point * find_avg(buffer_linked_list * BLL);


#endif