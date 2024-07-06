//
// Created by 1 on 2024-06-10.
//
#include "imuTask.h"

int32_t Yaw_iData,Roll_iData,Pitch_iData,Laser_idistance;
float Yaw_Data,Roll_Data,Pitch_Data,Laser_distance;

void IMU_Task(void )
{
    visual_process();

    if(Desk_Data[28] == 0x1a && Desk_Data[41] == 0xb1)
    {
        Yaw_iData   = Desk_Data[29] | Desk_Data[30] << 8 | Desk_Data[31] << 16 | Desk_Data[32] << 24;
        Roll_iData  = Desk_Data[33] | Desk_Data[34] << 8 | Desk_Data[35] << 16 | Desk_Data[36] << 24;
        Pitch_iData = Desk_Data[37] | Desk_Data[38] << 8 | Desk_Data[39] << 16 | Desk_Data[40] << 24;

        Yaw_Data = Yaw_iData / 1000.0f;
        Roll_Data = Roll_iData / 1000.0f;
        Pitch_Data = Pitch_iData / 1000.0f;

        if ((yawwant == 180.0f || yawwant == 270.0f) && Yaw_Data < 0)
        {
            Yaw_Data = Yaw_Data + 360.0f;
        }

    }

    if(Desk_Data[42] == 0x13 && Desk_Data[47] == 0xc1)
    {
        Laser_idistance = Desk_Data[43] | Desk_Data[44] << 8 | Desk_Data[45] << 16 | Desk_Data[46] << 24;
        Laser_distance = Laser_idistance / 1000.0f;
    }

    osDelay(1);
}