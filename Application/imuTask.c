//
// Created by 1 on 2024-06-10.
//
#include "imuTask.h"

int32_t Yaw_iData = 0,Roll_iData = 0,Pitch_iData = 0,Laser_idistance = 0;
float Yaw_Data = 0,Roll_Data = 0,Pitch_Data = 0,Laser_distance = 0;
int32_t Last_Yaw_Data = 0,Last_Roll_Data = 0,Last_Pitch_Data = 0;
int32_t error_Yaw = 0,error_Roll = 0,error_Pitch = 0;

void IMU_Task(void )
{
    if(Desk_Data[28] == 0x1a && Desk_Data[41] == 0xb1)
    {
        Yaw_iData   = Desk_Data[29] | Desk_Data[30] << 8 | Desk_Data[31] << 16 | Desk_Data[32] << 24;
        Roll_iData  = Desk_Data[33] | Desk_Data[34] << 8 | Desk_Data[35] << 16 | Desk_Data[36] << 24;
        Pitch_iData = Desk_Data[37] | Desk_Data[38] << 8 | Desk_Data[39] << 16 | Desk_Data[40] << 24;

//        if(Yaw_iData < -20000)
//        {
//            Yaw_Data = Yaw_iData / 1000.0f + 360.0f;
//            Roll_Data = Roll_iData / 1000.0f;
//            Pitch_Data = Pitch_iData / 1000.0f;
//        }
//        else
//        {
            Yaw_Data = Yaw_iData / 1000.0f;
            Roll_Data = Roll_iData / 1000.0f;
            Pitch_Data = Pitch_iData / 1000.0f;
//        }
            if(Yaw_Data < -20.0f)
                Yaw_Data = Yaw_Data + 360.0f;

    }

    if(Desk_Data[42] == 0x13 && Desk_Data[47] == 0xc1)
    {
        Laser_idistance = Desk_Data[43] | Desk_Data[44] << 8 | Desk_Data[45] << 16 | Desk_Data[46] << 24;
        Laser_distance = Laser_idistance / 1000.0f;
    }

    osDelay(1);
}