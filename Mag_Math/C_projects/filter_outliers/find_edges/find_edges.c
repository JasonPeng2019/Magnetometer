#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "find_edges.h"

static point * init_point(int x, int y){
    point* point_init = malloc(sizeof(point));
    point_init->x = x;
    point_init->y = y;
    point_init->next = NULL;
    point_init->prev = NULL;
    return point_init;
}

static void free_point(point * Point){
    free(Point);
    return;
}

static bool compare(point * Point_1, point * Point_2){
    if (Point_1->x >= Point_2->x){
        return true;
    }
    return false;
}

/*
static point * push_point(buffer_linked_list * input_buffer){
    point * to_push;
    point * terminator;
    point * new_point = init_point(0, 0);
    to_push = input_buffer->start;
    terminator = input_buffer->terminator;
    if (to_push!=terminator){
        point * new_start = to_push->next;
        terminator->next = new_start;
        new_start->prev = terminator;
        input_buffer->start = new_start;
    }
    new_point->x = to_push->x;
    new_point->y = to_push->y;
    free_point(to_push);
    input_buffer->count -= 1;
    return new_point;
}
*/

static point * search_prior(buffer_linked_list * BLL, point * point_add){
    int point_value;
    point* prior;
    prior = BLL->terminator;

    if (prior->next == NULL){
        return prior;
    } else {
        if (compare(point_add, BLL->end)){
            return BLL->end;
        }
        while (!(compare(point_add, prior) && compare(prior->next, point_add)))
        {
            if (prior->next == NULL){
                return prior;
            }
            prior = prior->next;
        }
        return prior;
    }
}


static point * add_point(buffer_linked_list * input_buffer, point * point_add){
    point* prior;
    point* prior_next;
    if (input_buffer->count >= BUFF_MAX){
        // if at max:
        // if point_add < 

        input_buffer->push(input_buffer);
    }
    prior = search_prior(input_buffer, point_add);
    if (prior == input_buffer->terminator){
        input_buffer->start = point_add;
    }
    if (prior->next == NULL){
        input_buffer->end = point_add;
        prior->next = point_add;
        point_add->prev = prior;
        input_buffer->count+=1;
        return point_add;
    }
    prior_next = prior->next;
    prior->next = point_add;
    point_add->prev = prior;
    point_add->next = prior_next;
    prior_next->prev = point_add;
    input_buffer->count+=1;
    return point_add;
}

static int find_coor_value(buffer_linked_list * BLL, point * coor){
    if (BLL->dimension = X_coor){
        return coor->x;
    }
    if (BLL->dimension = Y_coor){
        return coor->y;
    }
    printf("Mag Cal error: dimension not defined correctly");
    return 0;
}

buffer_linked_list * init_buffer_LL(void){
    buffer_linked_list* buff_LL = malloc(sizeof(buffer_linked_list));
    buff_LL->count = 0;
    point* start_point;
    start_point = init_point(-10000, -10000);
    buff_LL->terminator = start_point;
    buff_LL->start = start_point;
    buff_LL->end = start_point;
    buff_LL-> midpt = start_point;
    buff_LL->grp_start = start_point;
    buff_LL->grp_start_GAP = NULL;
    buff_LL->push = push_point;
    buff_LL->add = add_point;
    buff_LL->search_position_prior = search_prior;
    buff_LL->coor_Value = find_coor_value;
    buff_LL->compare = compare;
    return buff_LL;
}

void free_buffer_LL(buffer_linked_list * BLL){
    point*curr_point;
    point*next_point;
    curr_point = BLL->terminator;

    while (curr_point != NULL){
        next_point = curr_point->next;
        free_point(curr_point);
        curr_point = next_point;
    
    }
    free_point(curr_point);
    free(BLL);
    return;
}
/*
static int find_border(buffer_linked_list* BLL){
    if (BLL->count <= POINTS_ALLOW+1){
        printf("Mag Cal: not enough points in buffer to properly find borders");
    }
    dimension_choice dimension = BLL->dimension;
    BLL->grp_start_GAP = BLL->end;
    BLL->grp_start = BLL->end;
    for (int i; i<POINTS_ALLOW; i++){
        BLL->grp_start_GAP = BLL->grp_start_GAP->prev;
    }
    while (abs(BLL->coor_Value(BLL,BLL->grp_start_GAP) - BLL->coor_Value(BLL,BLL->grp_start)) > POINT_TO_POINT_DIST){
  //      printf("search: grp_start_GAP: %d, grp_start: %d, terminator: %d\n", BLL->grp_start_GAP->x, BLL->grp_start->x, BLL->terminator->x);
        if (BLL->grp_start_GAP == BLL->terminator){
            BLL->grp_start = NULL;
            printf("Mag Cal: too many outliers to calibrate. Increase buffer size.");
            break;
        }
        BLL->grp_start_GAP = BLL->grp_start_GAP->prev;
        BLL->grp_start = BLL->grp_start->prev;
    }
    if (dimension == X_coor){
        return BLL->grp_start->x;
    }
    if (dimension == Y_coor){
        return BLL->grp_start->y;
    }
}
*/

static void print_BLL(buffer_linked_list * BLL){
    point * current = BLL->terminator;
    while (current != NULL){
        printf("Point current: X: %d Y: %d\n", current->x, current->y);
        current = current->next;
        }
}
