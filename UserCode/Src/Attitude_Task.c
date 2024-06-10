//
// Created by 1 on 2023-11-07.
//
#include "Attitude_Task.h"

float TargetAngle1 = 0,TargetAngle2 = 0;
enum GPStates gpstate = STOP;
enum DPStates dpstate = NONE;
float NewHeartbeat = 0;//心跳值
//全局姿态控制
float TargetAngle = 0;
int Race_count = 0;
uint8_t IMU_Stand_flag = 0;
uint8_t Solpe_flag = 0;

uint8_t x_Rectification = 0,y_Rectification = 1, slope1_Rectification = 0, slope2_Rectification = 0;

void StandUp_Posture(void)
{
    ChangeGainOfPID(5.0f,0.8f,0.03f,0.05f);//初始化pid
    AllLegsSpeedLimit(SpeedMode_VERYFAST);
    Get_Target(0,PI);
    SetCoupledThetaPositionAll();
}

void LieDown_Posture(void)
{
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
//实际运行Trot步态
void Trot(float direction,int8_t kind)
{
    switch(kind)
    {
        case 0://小步Trot
            AllLegsSpeedLimit(SpeedMode_EXTREME);
            NewHeartbeat = 6;
            ChangeGainOfPID(12.0f,0.2f,0.6f,0);
            ChangeYawOfPID(200.0f,2.0f,3000.0f,10.0f);
            YawControl(yawwant, &state_detached_params[4], direction);
            gait_detached(state_detached_params[4],0.0f, 0.5f, 0.5f, 0.0f,
                          direction,direction,direction,direction);
            break;
        case 1://大步Trot
            AllLegsSpeedLimit(30.0f);
            NewHeartbeat = 5;
            ChangeGainOfPID(35.0f,2.0f,0.6f,0);
            YawControl(yawwant, &state_detached_params[1], direction);
            gait_detached(state_detached_params[1],0.0f, 0.5f, 0.5f, 0.0f,
                          direction,direction,direction,direction);
            break;
        case 2://双木桥
            AllLegsSpeedLimit(SpeedMode_EARLYEX);
            NewHeartbeat = 5;
            ChangeGainOfPID(17.0f,1.0f,0.6f,0);
            ChangeYawOfPID(0.35f,0.035f,3000.0f,10.0f);
            YawControl(yawwant, &state_detached_params[4], direction);
            gait_detached(state_detached_params[4],0.0f, 0.5f, 0.5f, 0.0f,
                          direction,direction,direction,direction);
            break;
        case 3://斜坡
            AllLegsSpeedLimit(30.0f);
            NewHeartbeat = 5;
            ChangeGainOfPID(20.0f,2.0f,0.6f,0);
            ChangeYawOfPID(0.3f,0.03f,3000.0f,10.0f);
            YawControl(yawwant, &state_detached_params[8], direction);
            gait_detached(state_detached_params[8],0.0f, 0.5f, 0.5f, 0.0f,
                          direction,direction,direction,direction);
            break;
        default:
            break;
    }
}

//慢步
void Walk(float direction,uint8_t speed)
{
    AllLegsSpeedLimit(30.0f);
    NewHeartbeat = 5;
    ChangeGainOfPID(20.0f,0.8f,0.6f,0);
//    ChangeYawOfPID(100.0f,0.5f,2500.0f,10.0f);
//    YawControl(yawwant, &state_detached_params[3], direction);
    gait_detached(state_detached_params[8],0.0,0.75,0.5,0.25,direction,direction,direction,direction);
}
//转弯步态
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
            break;
        case 'r':
            state_detached_params[0].detached_params_0.step_length = length;
            state_detached_params[0].detached_params_1.step_length = length;
            state_detached_params[0].detached_params_2.step_length = -length;
            state_detached_params[0].detached_params_3.step_length = -length;
            break;
        default:
            break;
    }
    gait_detached(state_detached_params[0],  0.0f, 0.5f, 0.5f, 0.0f,
                  1.0f, 1.0f, 1.0f,1.0f);
}

void EndPosture(void)
{
    motor_pos_controll(0,0,position);                    //3号电机与0号电机镜像对称
    HAL_Delay(1);
    motor_pos_controll(1,0,position);                    //3号电机与0号电机镜像对称
    HAL_Delay(1);
    motor_pos_controll(2,0,position);                    //3号电机与0号电机镜像对称
    HAL_Delay(1);
    motor_pos_controll(3,0,position);                    //3号电机与0号电机镜像对称
    HAL_Delay(1);
    motor_pos_controll(4,0,position);                    //3号电机与0号电机镜像对称
    HAL_Delay(1);
    motor_pos_controll(5,0,position);                    //3号电机与0号电机镜像对称
    HAL_Delay(1);
    motor_pos_controll(6,0,position);                    //3号电机与0号电机镜像对称
    HAL_Delay(1);
    motor_pos_controll(7,0,position);                    //3号电机与0号电机镜像对称
    HAL_Delay(1);
    motor_pos_controll(8,0,position);                    //3号电机与0号电机镜像对称
    HAL_Delay(1);
    motor_pos_controll(9,0,position);                    //3号电机与0号电机镜像对称
    HAL_Delay(1);
    motor_pos_controll(10,0,position);                    //3号电机与0号电机镜像对称
    HAL_Delay(1);
    motor_pos_controll(11,0,position);                    //3号电机与0号电机镜像对称
    HAL_Delay(1);
}

void Handshake(void)
{
    //整体限制
    AllLegsSpeedLimit(SpeedMode_SLOW);
    ChangeGainOfPID(7.0f,0.0f,0.8f,0);
    //左前腿
    x =  LegLenthMax*cos((30)*PI/180);
    y = -LegLenthMax*sin((30)*PI/180);
    CartesianToTheta();
    SetCoupledThetaPosition(0);
    //其它腿
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
    //控制
    AllLegsSpeedLimit(SpeedMode_SLOW);
    CartesianToTheta();//进行坐标转换
    SetCoupledThetaPositionAll();
    gpstate = STOP;
    osDelay(400);
}
void FBwAaLitAir(void)
{
    /*自动转换*/
    x=0;
    y=-LegSquatLenth;
    //使用低刚度和大量的阻尼来处理下降
    ChangeGainOfPID(8.0,0.1,0.25,100);
    AllLegsSpeedLimit(SpeedMode_SLOW);
    //后腿反方向（向后）转动
    ReverseMoveOpen();//方向反向控制
    CartesianToTheta();
    SetCoupledThetaPosition(1);
    SetCoupledThetaPosition(3);
    //前腿正常（向前）转动
    ReverseMoveClose();//恢复（其实不写也没事儿）
    CartesianToTheta();
    SetCoupledThetaPosition(0);
    SetCoupledThetaPosition(2);
    gpstate = STOP;//回到停止态
    osDelay(2000);
}

void Translate(int direction)
{
    switch (direction) {
        case 'r':
            AllLegsSpeedLimit(15.0f);
            NewHeartbeat = 5;
            ChangeGainOfPID(15.0f,2.0f,0.6f,0);
            gait_detached(state_detached_params[6],0.5f, 0.5f, 0.0f, 0.0f,
                          direction,direction,direction,direction);
            break;
        case 'l':
            AllLegsSpeedLimit(15.0f);
            NewHeartbeat = 5;
            ChangeGainOfPID(15.0f,2.0f,0.6f,0);
            gait_detached(state_detached_params[7],0.5f, 0.5f, 0.0f, 0.0f,
                          direction,direction,direction,direction);
            break;
        default:
            break;
    }
}

void WarnPosture(void)//警戒
{
    AllLegsSpeedLimit(SpeedMode_EXTREME);
    //使用刚度小，阻尼大的增益
    ChangeGainOfPID(8.0f,0.25f,70,0.22);
    SetPolarPositionAll_Delay(-60,LegLenthMin,700);
    gpstate=STOP;
}

void KneelPosture(void)//跪下
{
    ChangeGainOfPID(10.0f,0.1f,80,0.22);//恢复正常PD
    AllLegsSpeedLimit(SpeedMode_SLOW);
    TargetAngle1=-(3.48874f+offset_front_0-PI);//-(199.89+offset_front_1-180)
    TargetAngle2=PI-(1.549853f-offset_front_1);//180-(88.8-offset_front_1)
    SetCoupledThetaPositionAll();
    gpstate = STOP;
    osDelay(700);
}

void Race_Competition(void)
{
    /**
     * 从起点出发，前往第一个必达区的代码
     */
    if(Radar_FinalData.x_pos <= Half_of_hypotenuse && Race_count == 0) //略小于8000mm场地边长的距离大小
        Trot(Forward,1);

    /**
     * 进行斜着跳跃45度，再进行后退到达第二个必达区
     */
    if(Radar_FinalData.x_pos > Half_of_hypotenuse && Race_count == 0)
    {
        x_Rectification = 0;
        y_Rectification = 0;
        slope1_Rectification = 1;
        slope2_Rectification = 0;

        yawwant = 45.0f;
        Race_count++;
        Turn_Jump(-45);
    }


    /**
     * 后退到达第二个必达区
     */
    if (Radar_FinalData.x_pos >= End_of_hypotenuse && Race_count == 1)
        Trot(Backward,1);


    /**
     * 到达第二个必达区后，跳跃准备进入第三个必达区
     */
    if (Radar_FinalData.x_pos < End_of_hypotenuse && Race_count == 1)
    {
        x_Rectification = 1;
        y_Rectification = 0;
        slope1_Rectification = 0;
        slope2_Rectification = 0;

        yawwant = 90.0f;
        Race_count++;
        Turn_Jump(-45);
    }
    /**
     * 前往第三个必达区
     */
    if (Radar_FinalData.y_pos <= Half_of_hypotenuse && Race_count == 2)
        Trot(Forward,1);

    /**
     * 跳跃准备进入第四个必达区
     */
    if (Radar_FinalData.y_pos > Half_of_hypotenuse && Race_count == 2)
    {
        x_Rectification = 0;
        y_Rectification = 0;
        slope1_Rectification = 0;
        slope2_Rectification = 1;

        yawwant = 45.0f;
        Race_count++;
        Turn_Jump(45);
    }

    /**
     * 后退前往第四个必达区
     */
    if (Radar_FinalData.y_pos >= End_of_hypotenuse && Race_count == 3)
        Trot(Backward,1);

    /**
     * 达到第四个必达区后停下来
     */
    if(Radar_FinalData.y_pos < End_of_hypotenuse && Race_count == 3)
        gpstate = 1;

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

    ExecuteJump(StepUp_Jump,68);
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

    ExecuteJump(StepUp_Jump,68);
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

    ExecuteJump(StepUp_Jump,68);
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

    ExecuteJump(StepUp_Jump,68);
    osDelay(700);

    while(visual.data_8[9] > 13)
        Trot(Backward,2);

    StandUp_Posture();
    osDelay(200);

    for (int i = 0; i < 500; ++i) {
        MarkingTime();
    }

    ExecuteJump(StepDown_Jump,76);
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

    ExecuteJump(StepDown_Jump,76);
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

    ExecuteJump(Bridge_Jump,62);


    return 0;

}