//
// Created by Joerange on 2023/11/18.
//
#include <string.h>
#include "imu.h"
//定义用于接收不同IMU信息的联合体

IMU_EulerAngle_u IMU_EulerAngle={0};

//IMU串口接收BUF
uint8_t IMU_RX_BUF[IMU_REC_LEN]={0};
//是否开启IMU控制
uint8_t IMU_Control_Flag = 0;
//IMU的PID的Kp值
//期望角度
float yawwant = 0.0f,pitchwant = 0.0f,rollwant = 0.0f;
//差动IMU调节相关变量

//惯导姿态控制PID结构体初始化

void IMU_init()
{
    /****IMU的PID初始化****///Pitch和Roll的PID初始化（全局姿态控制，其输出直接控制电机转角）
    IMU_AUTO_PID_SET(0.5,0.01,2.0,3600);
}

void IMU_AUTO_PID_SET(float kp,float ki,float kd,float SumError_limit)
{
    PID_Set_KP_KI_KD(&Pitch_PID_Loop,kp,ki,kd);
    Pitch_PID_Loop.Output_limit = 180;
    Pitch_PID_Loop.SumError_limit = SumError_limit;
    memcpy(&Roll_PID_Loop,&Pitch_PID_Loop,sizeof(PIDTypeDef));
}


void IMU_Data_Process(uint16_t rx_len)
{

        if((yawwant == 180.0f || yawwant == 270.0f) && Yaw_Data < 0)
            Yaw_Data = Yaw_Data + 360.0f;

}

void Control_Flag(uint8_t IMU_Flag,uint8_t Visual_flag,uint8_t Radar_flag)
{
    if(IMU_Flag == 1)
        IMU_Control_Flag = 1;
    else if(IMU_Flag == 0)
        IMU_Control_Flag = 0;

    if(Visual_flag == 1)
        visual_control_flag = 1;
    else if(Visual_flag == 0)
        visual_control_flag = 0;

    if(Radar_flag == 1)
        Radar_control_flag = 1;
    else if(Radar_flag == 0)
        Radar_control_flag = 0;
}