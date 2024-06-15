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
    static uint8_t SystematicErrorFlag = 0;
    static float Yaw_ErrorCorrection = 0;
    static float Pitch_ErrorCorrection = 0;
    static float Roll_ErrorCorrection = 0;
    //���͵�ŷ���������У�ǰ0-5���ֽ���header�����ǿ��Բ��ܣ�6-8�ֽ�ΪPID����������жϡ�
    if(IMU_RX_BUF[6]==0x01 && IMU_RX_BUF[7]==0xb0 && IMU_RX_BUF[8]==0x10)//����ŷ���ǣ�PID��0xB001��������LSB���䣬��ʵ�����յ�01�����յ�B0��PID֮����PL�����غɴ�С��0x10��ʾ16�ֽڵ�����
    {
        //���������ݷŵ�ŷ������������
        for(uint8_t i=9;i<21;i++) IMU_EulerAngle.raw_data[i-9] = IMU_RX_BUF[i];//�Ƕ����ݲ�����0 1 2λ�õ����ݣ���Ϊ����֡ͷ����������3-14��4��Ԫ�أ�*3���飩���ݡ�
        //����ʼ�⵽��ֵ��Ϊ�������ֵ
        switch(SystematicErrorFlag)//����switcg-case����ʵ�ַǳ�����Ч�˷�������ʱ
        {
            case 10:
            {
                //ϵͳ�������ֵ����
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
        //���������Ƕ�ֵ
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