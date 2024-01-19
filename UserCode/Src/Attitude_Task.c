//
// Created by 1 on 2023-11-07.
//
#include "Attitude_Task.h"

float TargetAngle1 = 0,TargetAngle2 = 0;
enum GPStates gpstate = STOP;
enum DPStates dpstate = NONE;
float NewHeartbeat = 0;//����ֵ
//ȫ����̬����
int Global_IMU_Control = 0;

void StandUp_Posture(void)
{
    Get_Target(0,PI);
    SetCoupledThetaPositionAll();
}

void LieDown_Posture(void)
{
    for(int i = 1;i < 9;i ++)
    {
        AngleWant_MotorX[i] = 0;
    }
}
//������Trot��̬
void Test_Move(void)
{
    ChangeGainOfPID(60,0,0.6f,0);
    gait_detached(state_detached_params[9],Leg1_Delay,Leg2_Delay,Leg3_Delay,Leg4_Delay,Forward,Forward,Forward,Forward);
}
//ʵ������Trot��̬
void Trot(float direction,int8_t kind)
{
    switch(kind)
    {
        case 0://��Trot
            NewHeartbeat = 6;
            ChangeGainOfPID(8.0f,0,0.6f,0);
            gait_detached(state_detached_params[0],0.0f, 0.5f, 0.5f, 0.0f,
                          direction,direction,direction,direction);
            break;
        case 1://С��Trot
            NewHeartbeat = 5;
            ChangeGainOfPID(5.0f,0,0.6f,0);
            YawControl(yawwant, &state_detached_params[11], direction);
//            Change_SinStateDetachedParams(&state_detached_params[11],11,1,22.0f,28.0f,
//            10.0f,10.0f,0.2f,5.0f);
//            Change_SinStateDetachedParams(&state_detached_params[11],11,2,22.0f,28.0f,
//                                          10.0f,10.0f,0.2f,5.0f)ol
//            YawControl(yawwant, &state_detached_params[11],direction);
            gait_detached(state_detached_params[11],0.0f, 0.5f, 0.5f, 0.0f,
                          direction,direction,direction,direction);
            break;
        default:
            break;
    }
}

//����
void Walk(float direction,uint8_t speed)
{
//    PID_Set_KP_KI_KD(&Yaw_PID_Loop,1.7,0,1.0);
//    YawControl(yawwant,&state_detached_params[1],direction);
//    Yaw_PID_Loop.SumError_limit = 2500;Yaw_PID_Loop.Output_limit = 45;
    NewHeartbeat = 4;
    ChangeGainOfPID(60,0,0.6f,0);
    gait_detached(state_detached_params[1],0.0,0.75,0.5,0.25,direction,direction,direction,direction);
}
//ת�䲽̬
void Turn(int state_flag)
{
    NewHeartbeat = 5;
    ChangeGainOfPID(4.0f,0,0.6f,0);
    switch (state_flag) {
        case 'l':
            state_detached_params[8].detached_params_0.step_length = -20.0f;
            state_detached_params[8].detached_params_1.step_length = -20.0f;
            state_detached_params[8].detached_params_2.step_length = 20.0f;
            state_detached_params[8].detached_params_3.step_length = 20.0f;
            break;
        case 'r':
            state_detached_params[8].detached_params_0.step_length = 20.0f;
            state_detached_params[8].detached_params_1.step_length = 20.0f;
            state_detached_params[8].detached_params_2.step_length = -20.0f;
            state_detached_params[8].detached_params_3.step_length = -20.0f;
            break;
        default:
            break;
    }
    gait_detached(state_detached_params[8],  0.0f, 0.5f, 0.5f, 0.0f,
                  1.0f, 1.0f, 1.0f,1.0f);
}

void EndPosture(void)
{
    motor_pos_controll(0,0,position);                    //3�ŵ����0�ŵ������Գ�
    HAL_Delay(1);
    motor_pos_controll(1,0,position);                    //3�ŵ����0�ŵ������Գ�
    HAL_Delay(1);
    motor_pos_controll(2,0,position);                    //3�ŵ����0�ŵ������Գ�
    HAL_Delay(1);
    motor_pos_controll(3,0,position);                    //3�ŵ����0�ŵ������Գ�
    HAL_Delay(1);
    motor_pos_controll(4,0,position);                    //3�ŵ����0�ŵ������Գ�
    HAL_Delay(1);
    motor_pos_controll(5,0,position);                    //3�ŵ����0�ŵ������Գ�
    HAL_Delay(1);
    motor_pos_controll(6,0,position);                    //3�ŵ����0�ŵ������Գ�
    HAL_Delay(1);
    motor_pos_controll(7,0,position);                    //3�ŵ����0�ŵ������Գ�
    HAL_Delay(1);
    motor_pos_controll(8,0,position);                    //3�ŵ����0�ŵ������Գ�
    HAL_Delay(1);
    motor_pos_controll(9,0,position);                    //3�ŵ����0�ŵ������Գ�
    HAL_Delay(1);
    motor_pos_controll(10,0,position);                    //3�ŵ����0�ŵ������Գ�
    HAL_Delay(1);
    motor_pos_controll(11,0,position);                    //3�ŵ����0�ŵ������Գ�
    HAL_Delay(1);
}

void Dog_Posture(void)
{
    switch (gpstate) {
        case HALT:
            StandUp_Posture();
            break;
        case END:
            LieDown_Posture();
            break;
        case TURN_LEFT:
            Turn('l');
            break;
        case TURN_RIGHT:
            Turn('r');
            break;
        case MARCH:
            Walk(Forward,0);
            break;
        case MARCH_BACK:
            Walk(Backward,0);
            break;
        default:
            break;
    }
}