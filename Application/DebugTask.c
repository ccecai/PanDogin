//
// Created by 1 on 2024-06-10.
//
#include "DebugTask.h"

void Debug_Task(void )
{
//    usart_printf("%f,%f,%f,%f\n",Yaw_Data,Roll_Data,Pitch_Data,Distance);
//    usart_printf("%d,%d,%d,%d\n",Desk_Data[22],Desk_Data[23],Desk_Data[24],Desk_Data[25]);
//    usart_printf("%f,%f,%f,%f\n",((float )B_pos[1]*2*pi)/(6.33f*32768),((float )end_pos[1]*2*pi)/(6.33f*32768)
//                 ,((float )B_pos[2]*2*pi)/(6.33f*32768),((float )end_pos[2]*2*pi)/(6.33f*32768));
//    usart_printf("%f,%f,%f,%d,%d\n",visual.offset,visual.distance,Distance,bridge_count,FrontIfLook);
    usart_printf("%f,%f,%f,%f,%f,%f,%d,%d\n",visual.offset,visual.distance,Laser_distance
            ,normal_step_left,normal_step_right,Yaw_Data,Front_Camare_flag,Back_Camare_flag);

    osDelay(10);
}