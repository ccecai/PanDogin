//
// Created by Joerange on 2023/11/18.
//
#include "Attitude_Slove.h"
#include "Attitude_Task.h"
#include "remote.h"
#include "imu.h"
#include "stm32g474xx.h"

//�ض���fputc����
//int fputc(int ch, FILE *f)
//{
//    while((USART1->SR&0X40)==0);//ѭ������,ֱ���������
//    USART1->DR = (uint8_t) ch;
//    return ch;
//}

//����1�жϷ������
uint8_t USART_RX_BUF[USART_REC_LEN];//���ջ���,���USART_REC_LEN���ֽ�
//����״̬
//bit15��	������ɱ�־��0x0a��
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
uint16_t USART_RX_STA=0;//����״̬���

/*ң����������ؿ��Ʊ���*/
uint8_t REMOTE_RX_BUF[REMOTE_REC_LEN];
uint8_t TestFlag = 0;
uint8_t RestartFlag = 0;
uint8_t DebugData = 0;
uint8_t once_flag = 0;
//ң�ؿ��ٵ��ڲ�̬������ʵ����Ѳ�̬��ȷ��
float Leg1Delay = 0;
float Leg2Delay = 0.5;
float Leg3Delay = 0.5;
float Leg4Delay = 0;

//�˶��ٶȿ���
uint32_t MoveSpeed = SpeedMode_EXTREME;
//�˶�ǿ�ȿ���
uint16_t MoveIntensity_PosKp = 70;
float MoveIntensity_SpdKi = 0.26;
//��Ծ�Ƕȿ���
uint8_t Jump_Angle_Remote = 60;
//ң������������
int16_t RemoteHeartBeat = 0;
uint8_t flag = 0;

//�ر�ң�ش��ڽ����ж�
void CloseRemote(void)
{
    // �رմ���5�ж�
    HAL_NVIC_DisableIRQ(UART5_IRQn);
}
//����ң�ش��ڽ����ж�
void OpenRemote(void)
{
    HAL_NVIC_EnableIRQ(UART5_IRQn);
}

void Remote_Controller(void)
{
    switch (gpstate) {
        case 1:
            StandUp_Posture();
            flag = 1;
            break;
        case 2://����
            KneelPosture();
            break;
        case 3:
            LieDown_Posture();
            break;
        case 6:
            if (dpstate == 53)
            {
                Translate('l');
            }
            else
            {
                Speed_Competition_Turn();
            }

            break;
        case 7:
            if (dpstate == 53)
            {
                Translate('r');
            }
            else
            {
                Turn('r', 's');
            }
            break;
        case 10:
            if (dpstate == 53)
                Trot(Forward, 2);
            else
                Trot(Forward, 3);
            break;
        case 11:
            if (dpstate == 53)
                Trot(Backward, 2);
            else
                Trot(Backward, 3);
            break;
        case 20:
            ExecuteJump(Bridge_Jump, 71.2f);
//            ExecuteJump(StepDown_Jump,73.5f);
            break;
        case 21:
//            ExecuteJump(StepUp_LowJump,72.0f);
//            ExecuteJump(StepUp_Jump,76.0f);
            ExecuteJump(StepUp_Jump,71.2f);
            break;
        case 22:
//            ExecuteJump(StepUp_LowJump,72.0f);
//            ExecuteJump(StepUp_Jump,66.0f);
//            Turn_Jump(-45);
            ExecuteJump(StepUp_LowJump,69.0f);
            break;
        case 24://����
//            ExecuteJump(Bridge_Jump, 85.0f);
            FrontJump();
            break;
        case 30:
            SquatPosture();//����
            break;
        case 31:
            Handshake();//����
            break;
        case 32:
            StretchPosture();//��չ
            break;
        case 33:
            MarkingTime();//̤��
            break;
        case 36:
            Barrier_Competition();
//            Barrier_of_Stairs();
//            Test_Barrier_of_Slope();
//            Barrier_Competition();
            break;
        case 37:
            Race_Competition();
            break;
        case 41://����
            WarnPosture();
            break;
        case 100:
            Barrier_of_Slope();
        default:
            break;
    }
    if (Desk_Data[25] == 2)
        gpstate = 37;
    else if (Desk_Data[25] == 3)
        gpstate = 36;
    else if(Desk_Data[25] == 1)
        gpstate = 1;
    else if(Desk_Data[25] == 4 && once_flag == 0)
    {
        once_flag = 1;
        gpstate = 36;
    }
    else if(Desk_Data[25] == 5 && once_flag == 0)
    {
        once_flag = 2;
        gpstate = 36;
    }
    else if(Desk_Data[25] == 6 && once_flag == 0)
    {
        once_flag = 3;
        gpstate = 36;
    }
    else if(Desk_Data[25] == 7 && once_flag == 0)
    {
        once_flag = 4;
        gpstate = 36;
    }

    if(once_flag == 1)
    {
        automation_flag = 0;
        bridge_count = -1;
        once_flag = 10;
    }
    else if(once_flag == 2)
    {
        automation_flag = 1;
        bar_count = -2;
        once_flag = 10;
    }
    else if(once_flag == 3)
    {
        automation_flag = 2;
        slope_count = -1;
        yawwant = 180.0f;
        once_flag = 10;
    }
    else if(once_flag == 4)
    {
        automation_flag = 3;
        stairs_count = -2;
        yawwant = 270.0f;
        once_flag = 10;
    }


}

//���ƺ���
void RemoteCtrl(uint16_t rx_len)
{
    if(TestFlag!=0 && rx_len ==1) //DMA��������·�һ���Խ���ʱ����ȡ�ĳ���ʽ
    {
        //��IMU���ƺ�û�д���ͬ����Ϊʵ������IMU����ʱ����̬�Ĳ�����׼��������copy���������޷�ͨ������ԭ�����ı�ʵ�ʲ�̬��
        if(IMU_Control_Flag==0)
        {
            switch(TestFlag)
            {
                //��̬��������
                case 1:
                    state_detached_params[9].detached_params_0.stance_height =
                    state_detached_params[9].detached_params_1.stance_height =
                    state_detached_params[9].detached_params_2.stance_height =
                    state_detached_params[9].detached_params_3.stance_height = REMOTE_RX_BUF[0] / 10.0f;
                    break;
                case 2:
                    state_detached_params[9].detached_params_0.step_length =
                    state_detached_params[9].detached_params_1.step_length =state_detached_params[9].detached_params_2.step_length =
                    state_detached_params[9].detached_params_3.step_length = REMOTE_RX_BUF[0] / 5.6f;
                    break;
                case 3:
                    state_detached_params[9].detached_params_0.up_amp =
                    state_detached_params[9].detached_params_1.up_amp =
                    state_detached_params[9].detached_params_2.up_amp =
                    state_detached_params[9].detached_params_3.up_amp = REMOTE_RX_BUF[0] / 10.0f;
                    break;
                case 4:
                    state_detached_params[9].detached_params_0.down_amp =
                    state_detached_params[9].detached_params_1.down_amp =
                    state_detached_params[9].detached_params_2.down_amp =
                    state_detached_params[9].detached_params_3.down_amp = REMOTE_RX_BUF[0] / 10.0f;
                    break;
                case 5:
                    state_detached_params[9].detached_params_0.flight_percent =
                    state_detached_params[9].detached_params_1.flight_percent =
                    state_detached_params[9].detached_params_2.flight_percent =
                    state_detached_params[9].detached_params_3.flight_percent = REMOTE_RX_BUF[0] / 100.0f;
                    break;
                case 6:
                    state_detached_params[9].detached_params_0.freq =
                    state_detached_params[9].detached_params_1.freq =
                    state_detached_params[9].detached_params_2.freq =
                    state_detached_params[9].detached_params_3.freq = REMOTE_RX_BUF[0] / 10.0f;
                    break;
                    //��λ��ʱ����
                case 7:
                    Leg1Delay = (float)REMOTE_RX_BUF[0] / 100;
                    break;
                case 8:
                    Leg2Delay = (float)REMOTE_RX_BUF[0] / 100;
                    break;
                case 9:
                    Leg3Delay = (float)REMOTE_RX_BUF[0] / 100;
                    break;
                case 10:
                    Leg4Delay = (float)REMOTE_RX_BUF[0] / 100;
                    break;
                    //���ٵ���
                case 11:
                    MoveSpeed = (int)(REMOTE_RX_BUF[0] * 34.9);
                    break;
                    //IMUǿ�ȵ���
                case 12:;
                    break;
                    //�µ�����
                case 13:
                    //NewHeartbeat = REMOTE_RX_BUF[0];//�ڶ�ʱ�����в���������
                    break;
                    //��Ծ�Ƕȵ���
                case 14:
                    Jump_Angle_Remote = REMOTE_RX_BUF[0];
                    break;
                    //��̬ǿ�ȵ���
                case 15:
                    MoveIntensity_PosKp = REMOTE_RX_BUF[0];
                    break;
                default:
                    break;
            }
        }
        else
        {
            switch(TestFlag)
            {
                //��̬��������
                case 1:
                    StateDetachedParams_Copy[9].detached_params_0.stance_height=
                    StateDetachedParams_Copy[9].detached_params_1.stance_height=
                    StateDetachedParams_Copy[9].detached_params_2.stance_height=
                    StateDetachedParams_Copy[9].detached_params_3.stance_height=REMOTE_RX_BUF[0]/10.0f;
                    break;
                case 2:
                    StateDetachedParams_Copy[9].detached_params_0.step_length=
                    StateDetachedParams_Copy[9].detached_params_1.step_length=
                    StateDetachedParams_Copy[9].detached_params_2.step_length=
                    StateDetachedParams_Copy[9].detached_params_3.step_length= REMOTE_RX_BUF[0]/5.6f;
                    break;
                case 3:
                    StateDetachedParams_Copy[9].detached_params_0.up_amp=
                    StateDetachedParams_Copy[9].detached_params_1.up_amp=
                    StateDetachedParams_Copy[9].detached_params_2.up_amp=
                    StateDetachedParams_Copy[9].detached_params_3.up_amp=REMOTE_RX_BUF[0]/10.0f;
                    break;
                case 4:
                    StateDetachedParams_Copy[9].detached_params_0.down_amp=
                    StateDetachedParams_Copy[9].detached_params_1.down_amp=
                    StateDetachedParams_Copy[9].detached_params_2.down_amp=
                    StateDetachedParams_Copy[9].detached_params_3.down_amp=REMOTE_RX_BUF[0]/10.0f;
                    break;
                case 5:
                    StateDetachedParams_Copy[9].detached_params_0.flight_percent=
                    StateDetachedParams_Copy[9].detached_params_1.flight_percent=
                    StateDetachedParams_Copy[9].detached_params_2.flight_percent=
                    StateDetachedParams_Copy[9].detached_params_3.flight_percent=REMOTE_RX_BUF[0]/100.0f;
                    break;
                case 6:
                    StateDetachedParams_Copy[9].detached_params_0.freq=
                    StateDetachedParams_Copy[9].detached_params_1.freq=
                    StateDetachedParams_Copy[9].detached_params_2.freq=
                    StateDetachedParams_Copy[9].detached_params_3.freq=REMOTE_RX_BUF[0]/10.0f;
                    break;
                    //��λ��ʱ����
                case 7:
                    Leg1Delay = (float)REMOTE_RX_BUF[0]/100;
                    break;
                case 8:
                    Leg2Delay = (float)REMOTE_RX_BUF[0]/100;
                    break;
                case 9:
                    Leg3Delay = (float)REMOTE_RX_BUF[0]/100;
                    break;
                case 10:
                    Leg4Delay = (float)REMOTE_RX_BUF[0]/100;
                    break;
                    //���ٵ���
                case 11:
                    MoveSpeed = (int)(REMOTE_RX_BUF[0]*34.9);
                    break;
                    //IMUǿ�ȵ���
                case 12:;
                    break;
                    //�µ�����
                case 13:
                    //NewHeartbeat = REMOTE_RX_BUF[0];
                    break;
                    //��Ծ�Ƕȵ���
                case 14:
                    Jump_Angle_Remote = REMOTE_RX_BUF[0];
                    break;
                    //��̬ǿ�ȵ���
                case 15:
                    MoveIntensity_PosKp = REMOTE_RX_BUF[0];
                    break;
                case 16:
                    MoveIntensity_SpdKi = (float)REMOTE_RX_BUF[0]/50;
                    break;
                default:
                    break;
            }
        }
        TestFlag = 0;
    }
    else if(rx_len ==3 && REMOTE_RX_BUF[rx_len-1]=='C' && REMOTE_RX_BUF[rx_len-2]=='R')//��λ����ָ��
    {
        //ң����Э����������Ӧ��ϵת��
        switch(REMOTE_RX_BUF[0])
        {
            /*��д��ĸ���ͨ�����*/
            case 'W':gpstate = MARCH;break;
            case 'S':gpstate = MARCH_BACK;break;
            case 'A':gpstate = ROTAT_LEFT;break;
            case 'D':gpstate = ROTAT_RIGHT;break;
            case 'Q':gpstate = SQUAT;break;
            case 'Z':gpstate = END;break;
            case 'X':gpstate = HALT;break;
            case 'E':gpstate = KNEEL;break;
            case 'C':gpstate = SHAKEHAND;break;
            case 'R':gpstate = STRETCH;break;
            case 'V':gpstate = PLANE;break;
            case 'T':gpstate = Jump_Leap;break;
            case 'G':gpstate = Jump_Standard;break;
            case 'Y':gpstate = Jump_High;break;
            case 'F':gpstate = Jump_Far;break;
            case 'H':gpstate = WARN;break;
            case 'N':gpstate = SWAY;break;
            case 'U':gpstate = MARKINGTIME;break;
            case 'J':gpstate = HOMOLATERAL;break;
            case 'B':gpstate = NIGHTVISION;break;
            case 'I':yawwant=IMU_EulerAngle.EulerAngle[Yaw];break;//����IMUĿ��ֵΪ��ǰֵ
            case 'O':gpstate = REPOSITION;break;//�Զ���λ����
                /*�����ַ����ר�����*/
            case '>':dpstate = RACING;break;	        //����
            case '~':dpstate = HIGHHURDLES;break;      //����
            case '#':dpstate = PEBBLEROAD;break;       //��ʯ·
            case '^':dpstate = SEESAW;break;	        //���ΰ�
            case '|':dpstate = CALANDRIA;break;	    //�Ź�
            case '=':dpstate = BRIDGE;break;	        //˫ľ��
            case '-':dpstate = STAIR;break;		    //����
            case '*':dpstate = TEST;break;             //����
            case '?':dpstate = NONE;break;             //�˳�ר������
                /*Сд��ĸ��������������������Ρ��Զ���������̬�л��ȣ�*/
                //ϵͳ������������
            case 'l':yawwant+=2;break;//Ŀ��Ƕ�������ζ��Ҫ������е���
            case 'r':yawwant-=2;break;//Ŀ��Ƕȼ�С��ζ��Ҫ����
            case 'o':yawwant=0;break; //��ƫ��Ŀ��ǶȻص�0
            case 'p':RestartFlag=1;break;//ͨ������һ��������0��1���Ӷ�ʹϵͳ������whileѭ���У���ѭ����ִ�С��Ե�̬�����Ӷ��޷�ι����ͬʱ˫��ֹͣ����1s�󴥷�ϵͳ������
            case 'i':IMU_Control_Flag=!IMU_Control_Flag;break;//IMU��̬���ڵĿ��ƿ�������
            case 'w':break;
                /******���Խ���ʹ��******/
                //��̬�������������߲�̬�л�����
            case 'h':if(dpstate==TEST)TestFlag=1;else dpstate=TROT;break;
            case 's':if(dpstate==TEST)TestFlag=2;else dpstate=WALK;break;
            case 'a':if(dpstate==TEST)TestFlag=3;else dpstate=PACE;break;
            case 'd':if(dpstate==TEST)TestFlag=4;else dpstate=CANTER;break;
            case 'b':if(dpstate==TEST)TestFlag=5;else dpstate=GALLOP;break;
            case 'f':if(dpstate==TEST)TestFlag=6;else dpstate=SMALLSTEPS;break;
            case 'm':TestFlag=11;break;
            case 'k':TestFlag=12;break;
            case 'u':TestFlag=15;break;
            case 'n':TestFlag=16;break;
                //��̬��λ��ʱ����
            case '1':TestFlag=7;break;
            case '2':TestFlag=8;break;
            case '3':TestFlag=9;break;
            case '4':TestFlag=10;break;
                //�µ�����
            case 'x':TestFlag=13;break;
                //��Ծ�Ƕȿ���
            case 'g':if(dpstate==TEST) TestFlag=14;else dpstate=CHECK;break;
        }
    }

}




