#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "find_edges.h"
#include "find_edges.c"

int main(){
    printf("code started");
    point * p1 = init_point(1, 1);
    point * p2 = init_point(2, 2);
    point * p3 = init_point(3,3);
    point * p4 = init_point(40,40);
    point * p5 = init_point(50,50);
    point * p6 = init_point(60,60);
    point * p7 = init_point(7,7);
    point * p8 = init_point(8,8);
    point * p9 = init_point(30,30);
    point * p10 = init_point(9,9);
    point * p11 = init_point(10, 10);
    point * p12 = init_point(11, 11);
    point * p13 = init_point(6,6);

    point * array[] = {p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13};

    buffer_linked_list * BLL = init_buffer_LL(X_coor, Max_list);
    printf("code reached stage ");
    for (int i; i<12; i++){
        BLL->add(BLL, array[i]);
    }
    print_BLL(BLL);
    int max_edge = find_border(BLL);
    printf("\nmax border (should be 11): %d", max_edge);
    return 0;
}