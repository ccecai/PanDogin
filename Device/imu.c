//
// Created by Joerange on 2023/11/18.
//
#include <string.h>
#include "imu.h"
//�������ڽ��ղ�ͬIMU��Ϣ��������

IMU_EulerAngle_u IMU_EulerAngle={0};

//IMU���ڽ���BUF
uint8_t IMU_RX_BUF[IMU_REC_LEN]={0};
//�Ƿ���IMU����
uint8_t IMU_Control_Flag = 0;
//IMU��PID��Kpֵ
//�����Ƕ�
float yawwant = 0.0f,pitchwant = 0.0f,rollwant = 0.0f;
//�IMU������ر���

//�ߵ���̬����PID�ṹ���ʼ��

void IMU_init()
{
    /****IMU��PID��ʼ��****///Pitch��Roll��PID��ʼ����ȫ����̬���ƣ������ֱ�ӿ��Ƶ��ת�ǣ�
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