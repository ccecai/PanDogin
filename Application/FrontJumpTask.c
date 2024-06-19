//
// Created by 1 on 2024-06-10.
//
#include "FrontJumpTask.h"

void FrontJump_Task(void )
{
    if(wait_flag == 1)
    {
        AngleLoop[3].Output_limit = 30;
        AngleLoop[4].Output_limit = 30;
        AngleLoop[7].Output_limit = 30;
        AngleLoop[8].Output_limit = 30;
        PID_Set_KP_KI_KD(&AngleLoop[3],50.0f,0,2.0f);
        PID_Set_KP_KI_KD(&AngleLoop[4],50.0f,0,2.0f);
        PID_Set_KP_KI_KD(&AngleLoop[7],50.0f,0,2.0f);
        PID_Set_KP_KI_KD(&AngleLoop[8],50.0f,0,2.0f);

        SetPolarPositionLeg_Delay(Leg_Back_Angle - 20, 13.2f, 0,1);
        SetPolarPositionLeg_Delay(Leg_Back_Angle - 20, 13.2f, 0,3);
    }
    osDelay(1);
}