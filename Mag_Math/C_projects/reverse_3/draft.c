#ifdef DRAFT_C
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "inc/filter.h"
#include matrix.h

int draft(void){
    //start cal
    if enum = 0:
    printf("Calibrating magnetometer. Please follow the ACI Guide on Custom Magnetometer Calibration. 
    Hit enter when ready to start and move TDC to position 1.")
    buffer_linked_list * BLL_1 = init_buffer_LL();
    buffer_linked_list * BLL_2 = init_buffer_LL();

    if enum = 1:
    printf("Move the TDC to position 2 for 5 seconds. Hit enter when ready to start.")

    //

    //calibrating
    if enum = 0:
    BLL_1->add(BLL_1, init_point(mag_x_val, mag_z_val));
    if enum = 1:
    BLL_2->add(BLL_2, init_point(mag_x_val, mag_z_val));
    

    //end_calibration
    point cal_point_1 = find_avg(BLL_1);
    point cal_point_2 = find_avg(BLL_2);
    device->MagCal_X_Offset = (cal_point_1->x + cal_point_2->x)/2.0;
    device->MagCal_Y_Offset = (cal_point_1->y + cal_point_2->y)/2.0;

}
#endif