//
// Created by 1 on 2024-06-10.
//
#include "GO1_Motor.h"

void GO1_Init(void )
{
    MOTOR_Send_Init(); //初始化电机发送帧头
    Eight_PID_Init();//八个电机PID结构体初始化
    ChangeGainOfPID(6.0f,1.0f,0.03f,0.05f);//初始化pid

    Get_motor_began_pos();       //获得各个电机的初始位
    EndPosture();                //锁住电机

    PID_Init(&Yaw_PID_Loop);
    ChangeYawOfPID(0.2f,0.1f,4000.0f,15.0f);//陀螺仪PID初始化

    PID_Init(&VisualLoop);
    ChangePID(&VisualLoop,0.12f,0.04f,4000.0f,15.0f);

    PID_Init(&RadarController);
    ChangePID(&RadarController,0.2f,0.1f,4000.0f,15.0f);

    PID_Init(&M2006_Speed);
    PID_Init(&M2006_Position);

    PID_Set_KP_KI_KD(&M2006_Speed,6.5f,0.07f,0.0f);//2006电机速度环初始化
    PID_Set_KP_KI_KD(&M2006_Position,0.95f,0.0f,1.3f);//2006电机位置环初始化


    M2006_Speed.Output_limit = 4000;
    M2006_Position.Output_limit = 10000;

    printf("GO1 Init Ready\n");
    osDelay(3);
}

void GO1_LeftOutput_Task(void )
{
    leg_pos_controll02();
    osDelay(2);
}

void GO1_RightOutput_Task(void )
{
    leg_pos_controll();
    osDelay(2);
}

void PID_CalcTask(void )
{
    for (int i = 1; i < 9; ++i)
    {
        SetPoint(&AngleLoop[i], AngleWant_MotorX[i], i);
        PID_PosLocCalc(&AngleLoop[i], end_pos[i]);
    }

    osDelay(5);
}