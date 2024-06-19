//
// Created by 1 on 2024-06-10.
//
#include "DebugTask.h"

void Debug_Task(void )
{
    usart_printf("%f,%f,%f,%f\n",Yaw_Data,Roll_Data,Pitch_Data,Distance);

    osDelay(10);
}