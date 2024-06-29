//
// Created by 1 on 2024-06-10.
//
#include "TripodHeadTask.h"

void TripodHead_Task(void )
{
    if (angle_pitch_flag == 1)
        TargetAngle = Pitch_Data;

    SetPoint_IMU(&M2006_Position, AngleChange(TargetAngle));
    PID_PosLocM2006(&M2006_Position,struct_debug1[0].total_angle);

    SetPoint_IMU(&M2006_Speed, M2006_Position.Out_put);
    PID_PosLocM2006(&M2006_Speed,struct_debug1[0].speed);

    set_current(&hfdcan2,0x200,M2006_Speed.Out_put,0,0,0);

    osDelay(5);
}