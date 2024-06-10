//
// Created by 1 on 2024-06-10.
//
#include "FrontJumpTask.h"

void FrontJump_Task(void )
{
    if(wait_flag == 1)
    {
        AngleLoop[1].Output_limit = 30;
        AngleLoop[2].Output_limit = 30;
        AngleLoop[5].Output_limit = 30;
        AngleLoop[6].Output_limit = 30;
        PID_Set_KP_KI_KD(&AngleLoop[1],40.0f,0,2.0f);
        PID_Set_KP_KI_KD(&AngleLoop[2],40.0f,0,2.0f);
        PID_Set_KP_KI_KD(&AngleLoop[5],40.0f,0,2.0f);
        PID_Set_KP_KI_KD(&AngleLoop[6],40.0f,0,2.0f);

        SetPolarPositionLeg_Delay(77.0f, LegLenthMin, 0,0);
        SetPolarPositionLeg_Delay(77.0f, LegLenthMin, 0,2);
    }

    if(Yaw_Data < -89.5f)
    {
        osDelay(50);
        pitch = -90.0f + Pitch_Data;
    }
    else
    {
        pitch = Pitch_Data;
    }


    osDelay(1);
}