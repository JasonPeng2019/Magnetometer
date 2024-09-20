#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "inc/filter.h"



point * init_point(int x, int y){
    point* point_init = malloc(sizeof(point));
    point_init->x = x;
    point_init->y = y;
    point_init->next = NULL;
    point_init->prev = NULL;
    return point_init;
}

void free_point(point * Point){
    free(Point);
    return;
}

static int max(int num1, int num2){
    if (num1 >= num2){
        return num1;
    }
    return num2;
}

point * find_avg(buffer_linked_list * BLL){
    point * avg = init_point(0,0);
    point * curr = BLL->start;
    while (curr != NULL){
        avg->x += curr->x;
        avg->y += curr->y;
        curr = curr->next;
    }
    avg->x = avg->x/BLL->count;
    avg->y = avg->y/BLL->count;
    return avg;

}

static void add_avg(buffer_linked_list * BLL, point * add){
    int * x = &BLL->midpt->x;
    int * y = &BLL->midpt->y;
    int add_x = add->x;
    int add_y = add->y;
    int sum_x = *x * (BLL->count - 1);
    int sum_y = *y * (BLL->count - 1);
    sum_x += add_x;
    sum_y += add_y;
    *x = (int)sum_x/BLL->count;
    *y = (int)sum_y/BLL->count;
}

static int dist(point* Point1, point* Point2){
    int x_dist;
    int y_dist;
    int total_dist;
    if (Point1 != NULL && Point2 != NULL){
        x_dist = Point1->x - Point2->x;
        y_dist = Point1->y - Point2->y;
        total_dist = sqrt(x_dist*x_dist + y_dist*y_dist);
        return total_dist;
    }
    return -1;
}

static void push(buffer_linked_list * BLL){
    if (BLL->count == 0){
        return;
    }
    if (BLL->count == 1){
        free_point(BLL->start);
        BLL->start = BLL->sentinel;
        BLL->start->next = NULL;
        BLL->end = BLL->sentinel;
        BLL->count--;
        return;
    }
    int end_dist = dist(BLL->end, BLL->midpt);
    int start_dist = dist(BLL->start, BLL->midpt);
    int biggest = max(end_dist, start_dist);
    bool end_bigger = (biggest == end_dist);
    if (end_bigger) {
        point * n_end = BLL->end->prev;
        free_point(BLL->end);
        BLL->end = n_end;
        n_end->next = NULL;
    } 

    //else start bigger
    else  
    {
        point * n_start = BLL->start->next;
        free_point(BLL->start);
        BLL->start = n_start;
        BLL->sentinel->next = n_start;
        n_start->prev = BLL->sentinel;
    }
    BLL->count--;
}

static point * search_prior (buffer_linked_list * BLL, point* add){
    printf("Entered search_prior\n");

    if (BLL->count == 0){
        printf("search_prior: count is 0, returning sentinel\n");
        return BLL->sentinel;
    }

    int end_dist = dist(BLL->end, BLL->midpt);
    int start_dist = dist(BLL->start, BLL->midpt);
    int add_dist = dist(add, BLL->midpt);
    int biggest = max(end_dist, start_dist);
    bool end_bigger = (biggest == end_dist);

    printf("search_prior: end_dist=%d, start_dist=%d, add_dist=%d, biggest=%d\n", end_dist, start_dist, add_dist, biggest);

    point * curr = BLL->start;
    if (add_dist >= biggest){
        if (end_bigger){
            printf("search_prior: returning end\n");
            return BLL->end;
        } else {
            printf("search_prior: returning start\n");
            return BLL->start;
        }
    }

    // Iterate through the list to find the prior point
    int curr_dist = dist(curr, BLL->midpt);
    while (curr->next != NULL){
        int next_dist = dist(curr->next, BLL->midpt);
        printf("search_prior: curr_dist=%d, next_dist=%d\n", curr_dist, next_dist);

        if (add_dist >= curr_dist && add_dist < next_dist){
            printf("search_prior: found prior point\n");
            return curr;
        }
        curr_dist = next_dist;
        curr = curr->next;
    }
    
    printf("search_prior: returning last point\n");
    return curr;
}
static point * add_point(buffer_linked_list * BLL, point * add){
    printf("midpt: %d, %d\n", BLL->midpt->x, BLL->midpt->y);
    point* prior;
    point* prior_next;
    // case2: if full, push, then add
    // first check if full. 
    if (BLL->count >= BUFF_MAX){
        int end_dist = dist(BLL->end, BLL->midpt);
        int start_dist = dist(BLL->start, BLL->midpt);
        int biggest = max(end_dist, start_dist);
    //if full AND the adding one is within range, push one. 
        if (dist(add, BLL->midpt) < biggest){
            push(BLL);
        }
        // otherwise, if full and adding pt is out of range, return NULL and keep queue as is.
        return NULL;
    }
    // now at case 1: definitely not full. add away!
    prior = search_prior(BLL, add);
    printf("prior: %p\n", prior);
    if (prior != NULL){
        if (prior == BLL->sentinel){
            BLL->start = add;
            BLL->end = add;
        }
        if (prior->next != NULL){
            prior_next = prior->next;
            prior_next->prev = add;
            add->next = prior_next;
        } else {
            BLL->end = add;
        }
        prior->next = add;
        add->prev = prior;
        BLL->count+=1;
        //update midpt with new point added
        add_avg(BLL, add);
        return add;
    }
    return NULL;
}

buffer_linked_list * init_buffer_LL(void){
    buffer_linked_list* buff_LL = malloc(sizeof(buffer_linked_list));
    buff_LL->count = 0;
    point* start_point;
    start_point = init_point(-10000, -10000);
    buff_LL->sentinel = start_point;
    buff_LL->start = start_point;
    buff_LL->end = start_point;
    buff_LL->midpt = init_point(0,0);
    buff_LL->push = push;
    buff_LL->add = add_point;
    buff_LL->search_prior = search_prior;
    return buff_LL;
}

void free_buffer_LL(buffer_linked_list * BLL){
    point*curr_point;
    point*next_point;
    curr_point = BLL->sentinel;

    while (curr_point != NULL){
        next_point = curr_point->next;
        free_point(curr_point);
        curr_point = next_point;
    }
    free(BLL);
    return;
}