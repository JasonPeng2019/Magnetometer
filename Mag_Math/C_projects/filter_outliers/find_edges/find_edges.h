#include <stdio.h>
#include <stdlib.h>

#ifndef BUFF_MAX

#define BUFF_MAX                11
#define POINTS_ALLOW            4
#define POINT_TO_POINT_DIST     5


typedef struct point {
    int x;
    int y;
    struct point * next;
    struct point * prev;
} point;

typedef struct buffer_linked_list {
    int count;
    point*terminator;
    point*start;
    point*end;
    point*midpt;
    point* grp_start;
    point* grp_start_GAP;
    point* (*push)(struct buffer_linked_list*);
    point* (*add)(struct buffer_linked_list*, point*);
    point* (*search_position_prior)(struct buffer_linked_list*, point*);
    bool (*compare)(point*, point*);
    int (*coor_Value)(struct buffer_linked_list*, point*);
} buffer_linked_list;

static void print_BLL(buffer_linked_list * BLL);
static int find_border(buffer_linked_list* BLL);
static void free_buffer_LL(buffer_linked_list * BLL);
static buffer_linked_list * init_buffer_LL(void);
static int find_coor_value(buffer_linked_list * BLL, point * coor);
static point * add_point(buffer_linked_list * input_buffer, point * point_add);
static point * search_prior(buffer_linked_list * BLL, point * point_add);
static point * push_point(buffer_linked_list * input_buffer);
static bool compare(point * Point_1, point * Point_2);
static void free_point(point * Point);
static point * init_point(int x, int y);
static int compare_dist(point * Point_1, point* Point_2)

#endif

