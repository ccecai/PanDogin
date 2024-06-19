//
// Created by 1 on 2024-06-19.
//
#include "Barrier_Task.h"

uint8_t bridge_count = 0;
uint8_t stairs_count = 0;
uint8_t bar_count = 0;
uint8_t automation_flag = 0;

void Barrier_Competition(void )
{
    switch (automation_flag)
    {
        case 0:
            Barrier_of_Double_wooden_bridge();
            break;
        case 1:
            Barrier_of_Slope();
            break;
        case 2:
            Barrier_of_Stairs();
            break;
        case 3:
            Barrier_of_High_Bar();
            break;
        default:
            break;
    }
}

void Barrier_of_Double_wooden_bridge(void )
{
    if(Distance >= Point_of_Jumpbridge && bridge_count == 0)
        Trot(Forward,0);
    else if(Distance < Point_of_Jumpbridge && bridge_count == 0)
    {
        StandUp_Posture();
        osDelay(200);
        MarkingTime();
        osDelay(1000);
        StandUp_Posture();
        osDelay(200);

        ExecuteJump(Bridge_Jump, 72.0f);

        bridge_count++;
    }

    if (Distance >= Point_of_JumpDownbridge && bridge_count == 1)
        Trot(Forward,2);
    else if(Distance < Point_of_JumpDownbridge && bridge_count == 1)
    {
        StandUp_Posture();
        osDelay(200);
        MarkingTime();
        osDelay(1000);
        StandUp_Posture();
        osDelay(200);

        ExecuteJump(Bridge_Jump, 72.0f);

        bridge_count++;
    }

    if(Distance >= Point_of_Turn && bridge_count == 2)
        Trot(Forward,4);
    else if(Distance < Point_of_Turn && bridge_count == 2)
    {
        while (Yaw_Data < 90.0f)
            Turn('l','f');
        automation_flag++;
    }
}

void Barrier_of_Slope(void )
{
    if(Distance >= Point_of_Turn)
        Trot(Forward,4);//可能到时候还要调整电机的角度
    else if(Distance < Point_of_Turn)
    {
        while (Yaw_Data < 178.0f)
            Turn('l','f');
        automation_flag++;

    }
}

void Barrier_of_Stairs(void )
{
    if(Distance >= Point_of_OneJumpStairs && stairs_count == 0)
        Trot(Forward,4);
    else if(Distance < Point_of_OneJumpStairs && stairs_count == 0)
    {
        StandUp_Posture();
        osDelay(200);
        MarkingTime();
        osDelay(1000);
        StandUp_Posture();
        osDelay(200);

        ExecuteJump(StepUp_Jump,75.0f);

        if(Pitch_Data > Angle_of_outofcontrol)
        {
            while (Pitch_Data > Angle_of_outofcontrol)
                Trot(Backward,4);
            StandUp_Posture();
        }

        stairs_count++;
    }

    if(Distance >= Point_of_TwoJumpStairs && stairs_count == 1)
    {
        while (visual.offset > RightOffsetDistance)
            Translate('l');
        while (visual.offset < LeftOffsetDistance)
            Translate('r');

        while (Yaw_Data > 0.5f)
            Turn('r','s');
        while (Yaw_Data < -0.5f)
            Turn('l','s');

        StandUp_Posture();
        osDelay(200);
        MarkingTime();
        osDelay(1000);
        StandUp_Posture();
        osDelay(200);

        ExecuteJump(StepUp_Jump,65.0f);

        while (Yaw_Data > 0.5f)
            Turn('r','s');
        while (Yaw_Data < -0.5f)
            Turn('l','s');

        stairs_count++;
    }

    if(stairs_count == 2)
    {
        while (Distance >= Point_of_ThreeJumpStairs)
            Trot(Forward,4);
        while (Distance < Point_of_ThreeJumpStairs)
            Trot(Backward,4);

        while (visual.offset > RightOffsetDistance)
            Translate('l');
        while (visual.offset < LeftOffsetDistance)
            Translate('r');

        while (Yaw_Data > 0.5f)
            Turn('r','s');
        while (Yaw_Data < -0.5f)
            Turn('l','s');

        StandUp_Posture();
        osDelay(200);
        MarkingTime();
        osDelay(1000);
        StandUp_Posture();
        osDelay(200);

        ExecuteJump(Bridge_Jump, 72.0f);

        while (Yaw_Data > 0.5f)
            Turn('r','s');
        while (Yaw_Data < -0.5f)
            Turn('l','s');

        while (Distance >= Point_of_FourJumpStairs)
            Trot(Forward,4);
        while (Distance < Point_of_FourJumpStairs)
            Trot(Backward,4);

        while (visual.offset > RightOffsetDistance)
            Translate('l');
        while (visual.offset < LeftOffsetDistance)
            Translate('r');

        while (Yaw_Data > 0.5f)
            Turn('r','s');
        while (Yaw_Data < -0.5f)
            Turn('l','s');

        StandUp_Posture();
        osDelay(200);
        MarkingTime();
        osDelay(1000);
        StandUp_Posture();
        osDelay(200);

        ExecuteJump(Bridge_Jump, 85.0f);

        stairs_count++;
    }

    if(Distance >= Point_of_Turn && stairs_count == 3)
        Trot(Forward,4);
    else if(Distance < Point_of_Turn && stairs_count == 3)
    {
        Turn_Jump(45);
        while (Yaw_Data >= -90.0f)
            Turn('l','f');
        automation_flag++;

    }
}

void Barrier_of_High_Bar(void )
{
    if(Distance >= Point_of_JumpHighBar && bar_count == 0)
        Trot(Forward,4);
    else if(Distance < Point_of_JumpHighBar && bar_count == 0)
    {
        FrontJump();
        bar_count++;
    }

    if(Distance >= Point_of_Turn && bar_count == 1)
        Trot(Forward,4);
    else if(Distance < Point_of_Turn && bar_count == 1)
        gpstate = 1;
}