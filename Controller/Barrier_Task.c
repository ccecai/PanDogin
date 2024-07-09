//
// Created by 1 on 2024-06-19.
//
#include "Barrier_Task.h"

int bridge_count = -1;
int stairs_count = -2;
int bar_count = -2;
int slope_count = -1;
int automation_flag = 0;
uint8_t offset_flag = 0;
uint8_t angle_pitch_flag = 0;

void Barrier_Competition(void )
{
    if(automation_flag == 0)
        Barrier_of_Double_wooden_bridge();
    else if(automation_flag == 1)
        Barrier_of_High_Bar();
    else if(automation_flag == 2)
        Barrier_of_Slope();
    else if(automation_flag == 3)
        Barrier_of_Stairs();
}

void Test_Barrier_of_Double_wooden_bridge(void )
{
    if(visual.distance >= Point_of_HighSpeed && bridge_count == -1)
    {
        offset_flag = 1;
        Trot(Forward,4);
    }
    else if(visual.distance < Point_of_HighSpeed && bridge_count == -1)
        bridge_count++;
    if(visual.distance >= Point_of_Jumpbridge && bridge_count == 0)
    {
        offset_flag = 1;
        Trot(Forward,0);
    }

    else if(visual.distance < Point_of_Jumpbridge && bridge_count == 0)
    {
        StandUp_Posture();
        osDelay(200);

        ExecuteJump(Bridge_Jump, 72.0f);

        bridge_count++;
        offset_flag = 0;
    }

    if (visual.distance >= Point_of_JumpDownbridge && bridge_count == 1)
    {
        Trot(Forward,2);
    }

    else if(visual.distance < Point_of_JumpDownbridge && bridge_count == 1)
    {
        StandUp_Posture();
        osDelay(200);

        ExecuteJump(Bridge_Jump, 72.0f);

        bridge_count++;
    }

    if(visual.distance >= Point_of_Turn && bridge_count == 2)
    {
        Trot(Forward,4);
    }
    else if(visual.distance < Point_of_Turn && bridge_count == 2)
    {
        if (Yaw_Data < 90.0f)
        {
            Speed_Competition_Turn();
        }
        else
        {
            StandUp_Posture();
            gpstate = 0;
            automation_flag++;
        }

    }
}

void Barrier_of_Double_wooden_bridge(void )
{
    if(visual.distance >= Point_of_HighSpeed && bridge_count == -1)
    {
        offset_flag = 1;
        Trot(Forward,4);
    }
    else if(visual.distance < Point_of_HighSpeed && bridge_count == -1)
        bridge_count++;
    else if(Laser_distance > Point_of_Jumpbridge && bridge_count == 0)
    {
        offset_flag = 1;
        Trot(Forward,5);
    }

    else if(Laser_distance < Point_of_Jumpbridge && bridge_count == 0)
    {
        StandUp_Posture();
        osDelay(250);

        ExecuteJump(Bridge_Jump, 71.2f);

        bridge_count++;
        offset_flag = 0;
    }

    else if (visual.distance >= Point_of_JumpDownbridge && bridge_count == 1)
    {
        if(Yaw_Data > 0.5f)
            Turn('r','s');
        else if(Yaw_Data < -0.5f)
            Turn('l','s');
        else if(Yaw_Data < 0.5f && Yaw_Data > -0.5f)
            bridge_count++;
    }

    else if (visual.distance >= Point_of_JumpDownbridge && bridge_count == 2)
    {
        Trot(Forward,2);
    }

    else if(visual.distance < Point_of_JumpDownbridge && bridge_count == 2)
    {
        StandUp_Posture();
        osDelay(250);

        ExecuteJump(Bridge_Jump, 72.0f);

        bridge_count++;
    }

    else if(visual.distance >= Point_of_Turn && bridge_count == 3)
    {
        Trot(Forward,4);

        if(Yaw_Data > 80.0f)
        {
            automation_flag = 1;
        }
    }
    else if(visual.distance < Point_of_Turn && Yaw_Data < 90.0f && bridge_count == 3)
    {
        Speed_Competition_Turn();
    }

}


void Test_Barrier_of_Slope(void )
{
    if(slope_count == -1)
    {
        Control_Flag(1,0,0);
        Trot(Forward,3);
    }

    if(Pitch_Data > 15.0f && slope_count == -1)
    {
        Control_Flag(0,1,0);
        slope_count++;
    }

    if(visual.distance >= Point_of_Turn && slope_count == 0)
        Trot(Forward,4);
    else if(visual.distance < Point_of_Turn && slope_count == 0)
    {
        if (Yaw_Data < 90.0f)
        {
            Speed_Competition_Turn();
        }
        else
        {
            StandUp_Posture();
            gpstate = 0;
            automation_flag++;
        }

    }
}


void Barrier_of_Slope(void )
{
    if(slope_count == -1 && Pitch_Data < -12.0f)
    {
        Control_Flag(1,0,0);
        Trot(Forward,3);
    }
    else if(Pitch_Data < 12.0f && slope_count == -1 && Pitch_Data > -12.0f)
    {
        Control_Flag(1,0,0);
        Trot(Forward,3);
    }

    else if(Pitch_Data > 12.0f && slope_count == -1)
    {
        Control_Flag(0,1,0);
        slope_count++;
    }

    else if(visual.distance >= Point_of_Turn - 0.12f && slope_count == 0 )
    {
        Trot(Forward,3);
        if(Yaw_Data > 260.0f)
        {
            automation_flag = 3;
            yawwant = 270.0f;
        }
    }

    else if(visual.distance < Point_of_Turn - 0.12f && slope_count == 0)
    {
        Speed_Competition_Turn();
    }
}

void Test_Barrier_of_Stairs(void )
{
    stairs_flag = 1;

    if(visual.distance >= Point_of_HighSpeedStairs && stairs_count == -1)
        Trot(Forward,4);
    else if(visual.distance < Point_of_HighSpeedStairs && stairs_count == -1)
        stairs_count++;
    if(visual.distance >= Point_of_OneJumpStairs && stairs_count == 0)
        Trot(Forward,0);
    else if(visual.distance < Point_of_OneJumpStairs && stairs_count == 0)
    {
        StandUp_Posture();
        osDelay(200);

        ExecuteJump(StepUp_Jump,76.0f);

        stairs_count++;
    }

    if (stairs_count == 1 && Yaw_Data > 0.5f)
        Turn('r','s');
    else if (stairs_count == 1 && Yaw_Data < -0.5f)
        Turn('l','s');
    else if(stairs_count == 1 && Yaw_Data > -0.5f && Yaw_Data < 0.5f)
    {
        stairs_count++;
    }

    if(stairs_count == 2)
    {
        StandUp_Posture();
        osDelay(200);

        ExecuteJump(StepUp_Jump,64.4f);
        stairs_count++;
    }

    if(stairs_count == 3 && visual.distance < Point_of_ThreeJumpStairs - 2.0f)
        Trot(Backward,0);
    else if(stairs_count == 3 && visual.distance > Point_of_ThreeJumpStairs + 2.0f)
        Trot(Forward,0);
    else if(stairs_count == 3 && visual.distance < Point_of_ThreeJumpStairs + 2.0f && visual.distance > Point_of_ThreeJumpStairs - 2.0f)
    {
        StandUp_Posture();
        osDelay(200);

        ExecuteJump(StepUp_Jump, 71.6f);

        stairs_count++;
    }
    if(stairs_count == 4)
    {
        StandUp_Posture();
        osDelay(200);

        ExecuteJump(Bridge_Jump, 85.0f);

        stairs_count++;
    }

    if(visual.distance >= Point_of_Turn - 0.16f && stairs_count == 5)
        Trot(Forward,4);
    else if(visual.distance < Point_of_Turn - 0.16f && stairs_count == 5)
    {
        StandUp_Posture();
        gpstate = 0;

    }
}

void Barrier_of_Stairs(void )
{
    if(visual.distance < 3.0f && stairs_count == -2)
    {
        MidPoint = 104.5f;
        Trot(Forward,5);
    }

    else if(visual.distance > 3.0f && stairs_count == -2)
    {
        stairs_count = stairs_count + 2;
    }


    else if(stairs_count == 0 && Laser_distance > Point_of_OneJumpStairs)
    {
        MidPoint = 104.5f;
        Trot(Forward,5);
    }

    else if(Laser_distance < Point_of_OneJumpStairs && Laser_distance >= Point_of_OneJumpStairs - 0.3f && stairs_count == 0)
    {
        stairs_count = 1;
    }
    else if(stairs_count == 1)
    {
        if(Yaw_Data > 270.5f)
            Turn('r','s');
        else if(Yaw_Data < 269.5f)
            Turn('l','s');
        else if(Yaw_Data < 270.5f && Yaw_Data > 269.5f)
            stairs_count++;
    }
    else if(stairs_count == 2)
    {
        StandUp_Posture();
        osDelay(200);

        ExecuteJump(StepUp_Jump,71.2f);

        stairs_count++;
    }
    else if(stairs_count == 3)
    {
        if(Yaw_Data > 270.5f)
            Turn('r','s');
        else if(Yaw_Data < 269.5f)
            Turn('l','s');
        else if(Yaw_Data < 270.5f && Yaw_Data > 269.5f)
            stairs_count++;
    }

    else if(stairs_count == 4)
    {
        StandUp_Posture();
        osDelay(200);

        ExecuteJump(StepUp_LowJump,68.0f);

        stairs_count++;
    }
    else if(stairs_count == 5)
    {
        if(Yaw_Data > 270.5f)
            Turn('r','s');
        else if(Yaw_Data < 269.5f)
            Turn('l','s');
        else if(Yaw_Data < 270.5f && Yaw_Data > 269.5f)
            stairs_count++;
    }
    else if(stairs_count == 6)
    {
        StandUp_Posture();
        osDelay(50);

        ExecuteJump(StepUp_LowJump,68.0f);

        stairs_count++;
    }

    else if(stairs_count == 7)
    {
        if(Yaw_Data > 270.5f)
            Turn('r','s');
        else if(Yaw_Data < 269.5f)
            Turn('l','s');
        else if(Yaw_Data < 270.5f && Yaw_Data > 269.5f)
        {
            Control_Flag(1,0,0);
            stairs_count++;
        }
    }
    else if(stairs_count == 8 && visual.distance > Point_of_ThreeJumpStairs)
    {
        Trot(Forward,5);
        IMU_Slove(1,0);//是否开启障碍时腿时刻保持竖直（障碍赛用的）
    }
    else if(stairs_count == 8 && visual.distance <= Point_of_ThreeJumpStairs)
    {
        Control_Flag(0,1,0);
        IMU_Slove(0,0);
        Trot(Forward,4);
    }

    else if(visual.distance < Point_of_stop && stairs_count == 8)
    {
        StandUp_Posture();
        gpstate = 0;
    }
}


void Test_Barrier_of_High_Bar(void )
{
    if(visual.distance >= Point_of_HighSpeedBar && bar_count == -1)
        Trot(Forward,4);
    else if(visual.distance < Point_of_HighSpeedBar && bar_count == -1)
        bar_count++;
    if(visual.distance >= Point_of_JumpHighBar && bar_count == 0)
        Trot(Forward,0);
    else if(visual.distance < Point_of_JumpHighBar && bar_count == 0)
    {
        StandUp_Posture();
        osDelay(200);

        FrontJump();
        bar_count++;
    }

    if(visual.distance >= Point_of_Turn && bar_count == 1)
        Trot(Forward,4);
    else if(visual.distance < Point_of_Turn && bar_count == 1)
    {
        if (Yaw_Data < 90.0f)
        {
            Speed_Competition_Turn();
        }
        else
        {
            StandUp_Posture();
            gpstate = 0;
            automation_flag++;
        }
    }
}

void Barrier_of_High_Bar(void )
{
    if(bar_count == -2 && visual.distance < 6.0f)
        Trot(Forward,4);
    else if(bar_count == -2 && visual.distance > 6.0f)
        bar_count++;
    else if(visual.distance >= Point_of_HighSpeedBar && bar_count == -1)
        Trot(Forward,4);
    else if(visual.distance < Point_of_HighSpeedBar && bar_count == -1)
        bar_count++;
    else if(Laser_distance >= Point_of_JumpHighBar && bar_count == 0)
        Trot(Forward,5);
    else if(Laser_distance < Point_of_JumpHighBar && bar_count == 0)
    {
        if(Yaw_Data > 90.5f)
            Turn('r','s');
        else if(Yaw_Data < 89.5f)
            Turn('l','s');
        else if(Yaw_Data < 90.5f && Yaw_Data > 89.5f)
            bar_count++;
    }
    else if(Laser_distance < Point_of_JumpHighBar && bar_count == 1)
    {
        StandUp_Posture();
        osDelay(250);

        FrontJump();
        bar_count++;
    }

    else if(visual.distance >= Point_of_Turn && bar_count == 2)
    {
        Trot(Forward,4);
    }

    else if(visual.distance < Point_of_Turn && bar_count == 2 && Yaw_Data < 175.0f)
    {

        Speed_Competition_Turn();
    }
    if(Yaw_Data >= 170.0f)
    {
        automation_flag = 2;
        yawwant = 180.0f;
    }
}