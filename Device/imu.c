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
    static uint8_t SystematicErrorFlag = 0;
    static float Yaw_ErrorCorrection = 0;
    static float Pitch_ErrorCorrection = 0;
    static float Roll_ErrorCorrection = 0;
    //发送的欧拉角数据中，前0-5个字节是header，我们可以不管；6-8字节为PID，用其进行判断。
    if(IMU_RX_BUF[6]==0x01 && IMU_RX_BUF[7]==0xb0 && IMU_RX_BUF[8]==0x10)//对于欧拉角，PID是0xB001，由于是LSB传输，故实际先收到01，后收到B0。PID之后是PL，即载荷大小，0x10表示16字节的数据
    {
        //将串口数据放到欧拉角联合体中
        for(uint8_t i=9;i<21;i++) IMU_EulerAngle.raw_data[i-9] = IMU_RX_BUF[i];//角度数据不包括0 1 2位置的内容（作为三个帧头），而包括3-14共4（元素）*3（组）数据。
        //将初始测到的值作为误差修正值
        switch(SystematicErrorFlag)//利用switcg-case可以实现非常简洁高效了非阻塞延时
        {
            case 10:
            {
                //系统误差修正值计算
                Yaw_ErrorCorrection = IMU_EulerAngle.EulerAngle[Yaw];
                Pitch_ErrorCorrection = IMU_EulerAngle.EulerAngle[Pitch];
                Roll_ErrorCorrection = IMU_EulerAngle.EulerAngle[Roll];
                SystematicErrorFlag++;
            }
            case 11:break;
            default:
                SystematicErrorFlag++;
                break;
        }
        //误差修正后角度值
        IMU_EulerAngle.EulerAngle[Yaw]  -= Yaw_ErrorCorrection;
        IMU_EulerAngle.EulerAngle[Pitch]-= Pitch_ErrorCorrection;
        IMU_EulerAngle.EulerAngle[Roll] -=Roll_ErrorCorrection;
    }
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