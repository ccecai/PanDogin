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

    visual.offset = 100;//�Ӿ���ƫ��ֵ���г�ʼ��

    memcpy(StateDetachedParams_Copy,state_detached_params,100*StatesMaxNum);//state_detached_paramsÿ��Ԫ�أ�DetachedParam��,��ÿ�ֲ�̬�����һ��u8��ID�ţ���
    //ʵ��ռ�ݵ��ֽ���Ϊ4*6*4+4=96+4=100����4�����Ǽ�1����ΪҪ4�ֽڶ��룩��
    //�趨StatesMaxNum���򿽱�������Ϊ100*StateMaxNum����Ҫ�ٿ��������Զ࿽�������࿽���Ĳ�Ҫ�á�
    //�ø��Ʋ�����Ҫ�������н��У���Ҫ�ڲ���ϵͳ��ʼ��֮ǰ���У����򽫸�����ϵͳ�����������ֵ����⡣

    RemoteControl_Init(1,0); //ѡ��Ҫʹ�õ�Զ�̿���ģʽ
    Control_Flag(0,0,1);//ѡ���Ƿ������������Ӿ���ƫ����(�������õģ�
    IMU_Slove(0,0);//�Ƿ����ϰ�ʱ��ʱ�̱�����ֱ���ϰ����õģ�

    printf("Init_Ready\n");
    osDelay(3);

    joint_motor_init(&motor,1,MIT_MODE);

    osDelay(1000); //�ȴ���������������

    for(int i=0;i<6;i++)
    {
        enable_motor_mode(&hfdcan2, motor.para.id, MIT_MODE);//ʹ�ܵ��
        osDelay(20);
    }

    osDelay(1000); //�ڵ��Ե�ʱ���ӳ�3�������򿪼�ͣ����

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
