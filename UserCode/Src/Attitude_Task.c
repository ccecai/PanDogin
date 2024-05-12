//
// Created by 1 on 2023-11-07.
//
#include "Attitude_Task.h"
uint8_t delay_flag = 0;
float TargetAngle1 = 0,TargetAngle2 = 0;
enum GPStates gpstate = STOP;
enum DPStates dpstate = NONE;
float NewHeartbeat = 0;//����ֵ
//ȫ����̬����
int Global_IMU_Control = 0;
float TargetAngle = 0;
int Race_count = 0;
uint8_t IMU_Stand_flag = 0;
uint8_t Solpe_flag = 0;
uint8_t Lie_Down = 1;
void StandUp_Posture(void)
{
    Lie_Down = 0;
    ChangeGainOfPID(5.0f,0.8f,0.03f,0.05f);//��ʼ��pid
    AllLegsSpeedLimit(SpeedMode_VERYFAST);
    Get_Target(0,PI);
    SetCoupledThetaPositionAll();
}

void LieDown_Posture(void)
{
    Lie_Down = 1;
    AllLegsSpeedLimit(SpeedMode_VERYSLOW);
    for(int i = 1;i < 9;i ++)
    {
        AngleWant_MotorX[i] = 0;
    }
}
void MarkingTime(void)
{
    NewHeartbeat = 5;
    AllLegsSpeedLimit(SpeedMode_EXTREME);
    ChangeGainOfPID(8.0f,0.8f,0.6f,0);
    gait_detached(state_detached_params[2],0.0f, 0.5f, 0.5f, 0.0f,
                  1.0f,1.0f,1.0f,1.0f);
}
//ʵ������Trot��̬
void Trot(float direction,int8_t kind)
{
    switch(kind)
    {
        case 0://С��Trot
            AllLegsSpeedLimit(SpeedMode_EXTREME);
            NewHeartbeat = 6;
            ChangeGainOfPID(12.0f,0.2f,0.6f,0);
            ChangeYawOfPID(200.0f,2.0f,3000.0f,10.0f);
            YawControl(yawwant, &state_detached_params[4], direction);
            gait_detached(state_detached_params[4],0.0f, 0.5f, 0.5f, 0.0f,
                          direction,direction,direction,direction);
            break;
        case 1://��Trot
            Solpe_flag = 0;
            AllLegsSpeedLimit(30.0f);
            NewHeartbeat = 5;
            ChangeGainOfPID(20.0f,0.8f,0.6f,0);
            ChangeYawOfPID(0.1f,0.01f,3000.0f,10.0f);
            YawControl(yawwant, &state_detached_params[1], direction);
            gait_detached(state_detached_params[1],0.0f, 0.5f, 0.5f, 0.0f,
                          direction,direction,direction,direction);
            break;
        case 2://˫ľ��
            Solpe_flag = 0;
            AllLegsSpeedLimit(SpeedMode_EARLYEX);
            NewHeartbeat = 5;
            ChangeGainOfPID(17.0f,0.8f,0.6f,0);
            ChangeYawOfPID(0.35f,0.035f,3000.0f,10.0f);
            YawControl(yawwant, &state_detached_params[4], direction);
            gait_detached(state_detached_params[4],0.0f, 0.5f, 0.5f, 0.0f,
                          direction,direction,direction,direction);
            break;
        case 3://б��
//            Solpe_flag = 1;
            AllLegsSpeedLimit(18.0f);
            NewHeartbeat = 4;
            ChangeGainOfPID(18.0f,0.2f,0.6f,0);
            ChangeYawOfPID(0.1f,0.01f,3000.0f,10.0f);
            YawControl(yawwant, &state_detached_params[8], direction);
            gait_detached(state_detached_params[8],0.0f, 0.5f, 0.5f, 0.0f,
                          direction,direction,direction,direction);
            break;
        default:
            break;
    }
}

//����
void Walk(float direction,uint8_t speed)
{
    AllLegsSpeedLimit(30.0f);
    NewHeartbeat = 5;
    ChangeGainOfPID(20.0f,0.8f,0.6f,0);
//    ChangeYawOfPID(100.0f,0.5f,2500.0f,10.0f);
//    YawControl(yawwant, &state_detached_params[3], direction);
    gait_detached(state_detached_params[8],0.0,0.75,0.5,0.25,direction,direction,direction,direction);
}
//ת�䲽̬
void Turn(int state_flag,int speed_flag)
{
    float length;

    if(speed_flag == 'f')
    {
        length = 16.0f;
        state_detached_params[0].detached_params_0.freq = 5.5f;
        state_detached_params[0].detached_params_1.freq = 5.5f;
        state_detached_params[0].detached_params_2.freq = 5.5f;
        state_detached_params[0].detached_params_3.freq = 5.5f;
    }
    else if(speed_flag == 's')
    {
        length = 3.0f;
        state_detached_params[0].detached_params_0.freq = 2.5f;
        state_detached_params[0].detached_params_1.freq = 2.5f;
        state_detached_params[0].detached_params_2.freq = 2.5f;
        state_detached_params[0].detached_params_3.freq = 2.5f;
    }

    NewHeartbeat = 5;
    AllLegsSpeedLimit(SpeedMode_EXTREME);
    ChangeGainOfPID(17.0f,0.8f,0.6f,0);

    switch (state_flag) {
        case 'l':
            state_detached_params[0].detached_params_0.step_length = -length;
            state_detached_params[0].detached_params_1.step_length = -length;
            state_detached_params[0].detached_params_2.step_length = length;
            state_detached_params[0].detached_params_3.step_length = length;
//            state_detached_params[0].detached_params_0.step_length = -23.0f;
//            state_detached_params[0].detached_params_1.step_length = -23.0f;
//            state_detached_params[0].detached_params_2.step_length = -0.8f;
//            state_detached_params[0].detached_params_3.step_length = -0.8f;
            break;
        case 'r':
            state_detached_params[0].detached_params_0.step_length = length;
            state_detached_params[0].detached_params_1.step_length = length;
            state_detached_params[0].detached_params_2.step_length = -length;
            state_detached_params[0].detached_params_3.step_length = -length;
//            state_detached_params[0].detached_params_0.step_length = -0.8f;
//            state_detached_params[0].detached_params_1.step_length = -0.8f;
//            state_detached_params[0].detached_params_2.step_length = -23.0f;
//            state_detached_params[0].detached_params_3.step_length = -23.0f;
            break;
        default:
            break;
    }
    gait_detached(state_detached_params[0],  0.0f, 0.5f, 0.5f, 0.0f,
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

void Handshake(void)
{
    //��������
    AllLegsSpeedLimit(SpeedMode_SLOW);
    ChangeGainOfPID(7.0f,0.0f,0.8f,0);
    //��ǰ��
    x =  LegLenthMax*cos((30)*PI/180);
    y = -LegLenthMax*sin((30)*PI/180);
    CartesianToTheta();
    SetCoupledThetaPosition(0);
    //������
    x =  LegLenthMin*cos(60*PI/180);
    y =  LegLenthMin*sin(60*PI/180);
    CartesianToTheta();
    SetCoupledThetaPosition(1);
    SetCoupledThetaPosition(2);
    SetCoupledThetaPosition(3);
    gpstate = STOP;

}

void StretchPosture(void)
{
    AllLegsSpeedLimit(SpeedMode_FAST);
    x = -(LegStandLenth + 5) * cos(60 * PI / 180);
    y = (LegStandLenth + 5) * sin(60 * PI / 180);
    CartesianToTheta();
    SetCoupledThetaPosition(1);
    SetCoupledThetaPosition(3);
    osDelay(800);
    x = LegLenthMax * cos(30 * PI / 180);
    y = LegLenthMax * sin(30 * PI / 180);
    CartesianToTheta();
    SetCoupledThetaPosition(0);
    osDelay(1500);
    SetCoupledThetaPosition(2);
    gpstate = STOP;
}

void SquatPosture(void)
{
    x=0;
    y=LegSquatLenth;
    //����
    AllLegsSpeedLimit(SpeedMode_SLOW);
    CartesianToTheta();//��������ת��
    SetCoupledThetaPositionAll();
    gpstate = STOP;
    osDelay(400);
}
void FBwAaLitAir(void)
{
    /*�Զ�ת��*/
    x=0;
    y=-LegSquatLenth;
    //ʹ�õ͸նȺʹ����������������½�
    ChangeGainOfPID(8.0,0.1,0.25,100);
    AllLegsSpeedLimit(SpeedMode_SLOW);
    //���ȷ��������ת��
    ReverseMoveOpen();//���������
    CartesianToTheta();
    SetCoupledThetaPosition(1);
    SetCoupledThetaPosition(3);
    //ǰ����������ǰ��ת��
    ReverseMoveClose();//�ָ�����ʵ��дҲû�¶���
    CartesianToTheta();
    SetCoupledThetaPosition(0);
    SetCoupledThetaPosition(2);
    gpstate = STOP;//�ص�ֹ̬ͣ
    osDelay(2000);
}

void Translate(int direction)
{
    switch (direction) {
        case 'r':
            AllLegsSpeedLimit(30.0f);
            NewHeartbeat = 5;
            ChangeGainOfPID(23.0f,2.3f,0.6f,0);
            gait_detached(state_detached_params[6],0.5f, 0.5f, 0.0f, 0.0f,
                          direction,direction,direction,direction);
            break;
        case 'l':
            AllLegsSpeedLimit(30.0f);
            NewHeartbeat = 5;
            ChangeGainOfPID(23.0f,2.3f,0.6f,0);
            gait_detached(state_detached_params[7],0.5f, 0.5f, 0.0f, 0.0f,
                          direction,direction,direction,direction);
            break;
        default:
            break;
    }
}

void WarnPosture(void)//����
{
    AllLegsSpeedLimit(SpeedMode_EXTREME);
    //ʹ�øն�С������������
    ChangeGainOfPID(8.0f,0.25f,70,0.22);
    SetPolarPositionAll_Delay(-60,LegLenthMin,700);
    gpstate=STOP;
}

void KneelPosture(void)//����
{
    ChangeGainOfPID(8,0.1f,80,0.22);//�ָ�����PD
    AllLegsSpeedLimit(SpeedMode_SLOW);
    TargetAngle1=-(199.89f+offset_front_0-180.0f);//-(199.89+offset_front_1-180)
    TargetAngle2=180.0f-(88.8f-offset_front_1);//180-(88.8-offset_front_1)
    SetCoupledThetaPositionAll();
    gpstate = STOP;
    osDelay(700);
}

void Race_Competition(void)
{
    if(Race_count == 0 && Distance >= 160.0f)
    {
        Trot(Backward,1);
    }

    if(Distance < 160.0f && Race_count == 0)
    {
        while (IMU_EulerAngle.EulerAngle[Yaw] < 135.0f)
        {
            Turn('l','s');
        }
        for (int i = 0; i < 5; ++i) {
            distance[i] = 300;
        }
        Distance = 300.0f;
        visual.offset = 100;
        yawwant = 135.0f;
        Race_count++;
        StandUp_Posture();
        osDelay(200);
    }
    if(Race_count == 1 && Distance >= 95.0f)
    {
        Trot(Backward,1);
    }
    if(Distance < 95.0f && Race_count == 1)
    {
        while (IMU_EulerAngle.EulerAngle[Yaw] < -91.0f || IMU_EulerAngle.EulerAngle[Yaw] > -89.0f)
        {
            Turn('l','s');
        }
        for (int i = 0; i < 5; ++i) {
            distance[i] = 300;
        }
        Distance = 300.0f;
        visual.offset = 100;
        yawwant = -90.0f;
        Race_count++;
        StandUp_Posture();
        osDelay(200);

    }
    if(Race_count == 2 && Distance >= 160.0f)
    {
        Trot(Backward,1);
    }
    if(Race_count == 2 && Distance < 160.0f)
    {

        while (IMU_EulerAngle.EulerAngle[Yaw] > 136.0f || IMU_EulerAngle.EulerAngle[Yaw] < 134.0f)
        {
            Turn('r','s');
        }

        for (int i = 0; i < 5; ++i) {
            distance[i] = 300;
        }
        Distance = 300.0f;
        visual.offset = 100;
        yawwant = -45.0f;
        Race_count++;
        StandUp_Posture();
        osDelay(200);



    }
    if(Race_count == 3 && Distance >= 70.0f)
    {
        Trot(Backward,1);
    }
    if(Race_count == 3 && Distance < 70.0f)
    {
        StandUp_Posture();
    }
}

int ll(void)
{
    while(visual.data_8[1] > 82)
    {
        Trot(Backward,2);
    }

    StandUp_Posture();
    osDelay(200);

    for (int i = 0; i < 500; ++i) {
        MarkingTime();
    }

    StandUp_Posture();
    osDelay(200);

    ExecuteJump(High_Jump,68);
    osDelay(600);
    while(visual.data_8[5] < 12 || visual.data_8[5] > 17)
    {
        if(visual.data_8[5] < 12)
            Trot(Forward,2);
        if(visual.data_8[5] > 17)
            Trot(Backward,2);
    }
    while(IMU_EulerAngle.EulerAngle[Yaw] > 0.5f || IMU_EulerAngle.EulerAngle[Yaw] < -0.5f)
    {
        if(IMU_EulerAngle.EulerAngle[Yaw] > 0.5f)
            Turn('r','s');
        if(IMU_EulerAngle.EulerAngle[Yaw] < -0.5f)
            Turn('l','s');
    }



    StandUp_Posture();
    osDelay(200);

    for (int i = 0; i < 500; ++i) {
        MarkingTime();
    }

    StandUp_Posture();
    osDelay(200);

    ExecuteJump(High_Jump,68);
    osDelay(700);

    while(visual.data_8[5] < 12 || visual.data_8[5] > 17)
    {
        if(visual.data_8[5] < 12)
            Trot(Forward,2);
        if(visual.data_8[5] > 17)
            Trot(Backward,2);
    }

    while(IMU_EulerAngle.EulerAngle[Yaw] > 0.5f || IMU_EulerAngle.EulerAngle[Yaw] < -0.5f)
    {
        if(IMU_EulerAngle.EulerAngle[Yaw] > 0.5f)
            Turn('r','s');
        if(IMU_EulerAngle.EulerAngle[Yaw] < -0.5f)
            Turn('l','s');
    }



    StandUp_Posture();
    osDelay(200);

    for (int i = 0; i < 500; ++i) {
        MarkingTime();
    }

    StandUp_Posture();
    osDelay(200);

    ExecuteJump(High_Jump,68);
    osDelay(700);

    while(IMU_EulerAngle.EulerAngle[Yaw] > 0.5f || IMU_EulerAngle.EulerAngle[Yaw] < -0.5f)
    {
        if(IMU_EulerAngle.EulerAngle[Yaw] > 0.5f)
            Turn('r','s');
        if(IMU_EulerAngle.EulerAngle[Yaw] < -0.5f)
            Turn('l','s');
    }

    StandUp_Posture();
    osDelay(200);

    for (int i = 0; i < 500; ++i) {
        MarkingTime();
    }

    StandUp_Posture();
    osDelay(200);

    ExecuteJump(High_Jump,68);
    osDelay(700);

    while(visual.data_8[9] > 13)
        Trot(Backward,2);

    StandUp_Posture();
    osDelay(200);

    for (int i = 0; i < 500; ++i) {
        MarkingTime();
    }

    ExecuteJump(Leap_Jump,76);
    osDelay(700);

    while(visual.data_8[10] < 20 || visual.data_8[10] > 28)
    {
        if(visual.data_8[10] < 20)
            Trot(Backward,2);
        if(visual.data_8[10] > 28)
            Trot(Forward,2);
    }
    while(IMU_EulerAngle.EulerAngle[Yaw] > 0.5f || IMU_EulerAngle.EulerAngle[Yaw] < -0.5f)
    {
        if(IMU_EulerAngle.EulerAngle[Yaw] > 0.5f)
            Turn('r','s');
        if(IMU_EulerAngle.EulerAngle[Yaw] < -0.5f)
            Turn('l','s');
    }

    for (int i = 0; i < 500; ++i) {
        MarkingTime();
    }

    ExecuteJump(Leap_Jump,76);
    osDelay(700);

    while(visual.data_8[10] < 20 || visual.data_8[10] > 28)
    {
        if(visual.data_8[10] < 20)
            Trot(Backward,2);
        if(visual.data_8[10] > 28)
            Trot(Forward,2);
    }
    while(IMU_EulerAngle.EulerAngle[Yaw] > 0.5f || IMU_EulerAngle.EulerAngle[Yaw] < -0.5f)
    {
        if(IMU_EulerAngle.EulerAngle[Yaw] > 0.5f)
            Turn('r','s');
        if(IMU_EulerAngle.EulerAngle[Yaw] < -0.5f)
            Turn('l','s');
    }
    for (int i = 0; i < 500; ++i) {
        MarkingTime();
    }

    ExecuteJump(High_Jump,62);


    return 0;

}