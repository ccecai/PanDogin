//
// Created by 1 on 2023-11-06.
//
#include "Myinit.h"
#include "string.h"

int8_t BlueTeeth_flag = 0,NRFStart_flag = 0;

void Myinit(void)
{
    MyFDCan1_config();
    MyFDCan2_config();
    RetargetInit(&huart3);
    HAL_TIM_Base_Start_IT(&htim2);

    visual.offset = 100;//视觉纠偏的值进行初始化

    memcpy(StateDetachedParams_Copy,state_detached_params,100*StatesMaxNum);//state_detached_params每个元素（DetachedParam型,即每种步态，外加一个u8的ID号）。
    //实际占据的字节数为4*6*4+4=96+4=100（＋4而不是加1是因为要4字节对齐）。
    //设定StatesMaxNum，则拷贝的上限为100*StateMaxNum，不要少拷贝，可以多拷贝，但多拷贝的不要用。
    //该复制操作不要在任务中进行，而要在操作系统初始化之前进行，否则将给操作系统的运行造成奇怪的问题。

    RemoteControl_Init(1,0); //选择要使用的远程控制模式
    Control_Flag(0,0,1);//选择是否开启陀螺仪与视觉纠偏开关(竞速赛用的）
    IMU_Slove(0,0);//是否开启障碍时腿时刻保持竖直（障碍赛用的）

    printf("Init_Ready\n");
    osDelay(3);

    joint_motor_init(&motor,1,MIT_MODE);

    osDelay(1000); //等待云深处电机驱动开机

    for(int i=0;i<6;i++)
    {
        enable_motor_mode(&hfdcan2, motor.para.id, MIT_MODE);//使能电机
        osDelay(20);
    }

    osDelay(1000); //在调试的时候延迟3秒用来打开急停开关

}

void RemoteControl_Init(int8_t Blue_flag,int8_t NRF_flag)
{
    if(NRF_flag == 1)
    {
        while (NRF24L01_Check()) {
            NRF24L01_RX_Mode();
            usart_printf("NRF READY!\r\n");
        }
    }

    BlueTeeth_flag = Blue_flag;
}
