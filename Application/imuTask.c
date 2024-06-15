//
// Created by 1 on 2024-06-10.
//
#include "imuTask.h"

int32_t Yaw_iData,Roll_iData,Pitch_iData;
float Yaw_Data,Roll_Data,Pitch_Data;

void IMU_Task(void )
{
    Yaw_iData   = Desk_Data[20] | Desk_Data[21] << 8 | Desk_Data[22] << 16 | Desk_Data[23] << 24;
    Roll_iData  = Desk_Data[24] | Desk_Data[25] << 8 | Desk_Data[26] << 16 | Desk_Data[27] << 24;
    Pitch_iData = Desk_Data[28] | Desk_Data[29] << 8 | Desk_Data[30] << 16 | Desk_Data[31] << 24;

    Yaw_Data = Yaw_iData / 1000.0f;
    Roll_Data = Roll_iData / 1000.0f;
    Pitch_Data = Pitch_iData / 1000.0f;

    osDelay(1);
}