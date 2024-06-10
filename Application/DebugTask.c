//
// Created by 1 on 2024-06-10.
//
#include "DebugTask.h"

void Debug_Task(void )
{
    printf("%f,%f,%f\n",Radar_FinalData.x_pos,Radar_FinalData.y_pos,Radar_FinalData.yaw);

    osDelay(10);
}