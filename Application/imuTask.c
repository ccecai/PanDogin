//
// Created by 1 on 2024-06-10.
//
#include "imuTask.h"

int32_t Yaw_iData,Roll_iData,Pitch_iData;
float Yaw_Data,Roll_Data,Pitch_Data;

void IMU_Task(void )
{
    Yaw_iData   = Desk_Data[22] | Desk_Data[23] << 8 | Desk_Data[24] << 16 | Desk_Data[25] << 24;
    Roll_iData  = Desk_Data[26] | Desk_Data[27] << 8 | Desk_Data[28] << 16 | Desk_Data[29] << 24;
    Pitch_iData = Desk_Data[30] | Desk_Data[31] << 8 | Desk_Data[32] << 16 | Desk_Data[33] << 24;

    Yaw_Data = Yaw_iData / 1000.0f;
    Roll_Data = Roll_iData / 1000.0f;
    Pitch_Data = Pitch_iData / 1000.0f;

    osDelay(1);
}