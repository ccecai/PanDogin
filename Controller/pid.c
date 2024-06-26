//
// Created by 19734wtf on 2023/3/27.
//
#include <stdint-gcc.h>
#include "pid.h"
#include "motor.h"
#include "usart.h"
#include "Attitude_Slove.h"
#include "Attitude_Task.h"
#include "crc16.h"

dog_leg_parameter DogLegsMotorXPID[8]={0};//核心存储数组之一
PIDTypeDef AngleLoop[9];
PIDTypeDef SpeedLoop[9];
PIDTypeDef VisualLoop;
PIDTypeDef RadarController;
uint8_t OnlyPosLoop = 1;
PIDTypeDef Yaw_PID_Loop={0},Roll_PID_Loop = {0},Pitch_PID_Loop={0};
PIDTypeDef M2006_Speed,M2006_Position;
void PID_Init(PIDTypeDef *pid)
{
    pid->Setpoint=0.0f;
    pid->SumError=0.0f;

    pid->P=0.0f;
    pid->I=0.0f;
    pid->D=0.0f;

    pid->Last_error=0.0f;
    pid->LLast_error=0.0f;
}

//PID基础配置
void PID_Set_KP_KI_KD(PIDTypeDef *pid,float kp,float ki,float kd)
{
    pid->P=kp;pid->I=ki;pid->D=kd;
}

//PID位置环目标确定
void SetPoint(PIDTypeDef *pid,float want,uint8_t id)
{
    pid->Setpoint= want + (B_pos[id]*2*pi)/(6.33f*32768);    //在初始零点的基础上进行位置控制
}
void SetPoint_IMU(PIDTypeDef *pid,float want)
{
    pid->Setpoint=want;
}
//PID偏差
void SetPoint_Visual(PIDTypeDef *pid,float want)
{
    pid->Setpoint=want;
}

//位置式PID计算(用于位置（角度）环，其输出作为速度环的目标值)
void PID_PosLocCalc(PIDTypeDef *pid, float feedbackpos)//位置式
{
    float Now_Point,Now_Error,d_Error;
    Now_Point = ((float )feedbackpos*2*pi)/(6.33f*32768);
    Now_Error=pid->Setpoint-Now_Point;
    pid->SumError+=Now_Error;//这部分进行了累加，从而导致积分饱和现象。
    //积分限幅（而增量式PID不需要积分限幅）积分限幅有两种思路，一种是限制sum（相对限制），另一种是限制I*sum（绝对限制），后者我认为更加合理，但考虑到新老代码的兼容性，仍然采用前者。
    if(pid->SumError     >  pid->SumError_limit) pid->SumError= pid->SumError_limit;
    else if(pid->SumError< -pid->SumError_limit) pid->SumError=-pid->SumError_limit;

    d_Error = Now_Error - pid->Last_error;//误差之差，表示微分。
    pid->Last_error=Now_Error;

    pid->Out_put = pid->P * Now_Error +
                   pid->I * pid->SumError +
                   pid->D * d_Error;
    //限幅输出
    if(pid->Out_put     > pid->Output_limit) pid->Out_put= pid->Output_limit;
    else if(pid->Out_put<-pid->Output_limit) pid->Out_put=-pid->Output_limit;

    pid->Last_Out_put = pid->Out_put;
}

void PID_PosLocM2006(PIDTypeDef *pid, int32_t feedbackpos)//位置式
{
    float Now_Point,Now_Error,d_Error;
    Now_Point = feedbackpos;
    Now_Error=pid->Setpoint-Now_Point;
    pid->SumError+=Now_Error;//这部分进行了累加，从而导致积分饱和现象。
    //积分限幅（而增量式PID不需要积分限幅）积分限幅有两种思路，一种是限制sum（相对限制），另一种是限制I*sum（绝对限制），后者我认为更加合理，但考虑到新老代码的兼容性，仍然采用前者。
    if(pid->SumError     >  pid->SumError_limit) pid->SumError= pid->SumError_limit;
    else if(pid->SumError< -pid->SumError_limit) pid->SumError=-pid->SumError_limit;

    d_Error = Now_Error - pid->Last_error;//误差之差，表示微分。
    pid->Last_error=Now_Error;

    pid->Out_put = pid->P * Now_Error +
                   pid->I * pid->SumError +
                   pid->D * d_Error;

    //限幅输出
    if(pid->Out_put     > pid->Output_limit) pid->Out_put= pid->Output_limit;
    else if(pid->Out_put<-pid->Output_limit) pid->Out_put=-pid->Output_limit;

    pid->Last_Out_put = pid->Out_put;
}

void PID_Pos(PIDTypeDef *pid, float feedbackpos)//位置式
{
    float Now_Point,Now_Error,d_Error;
    Now_Point = feedbackpos;
    Now_Error=pid->Setpoint-Now_Point;
    pid->SumError+=Now_Error;//这部分进行了累加，从而导致积分饱和现象。
    //积分限幅（而增量式PID不需要积分限幅）积分限幅有两种思路，一种是限制sum（相对限制），另一种是限制I*sum（绝对限制），后者我认为更加合理，但考虑到新老代码的兼容性，仍然采用前者。
    if(pid->SumError     >  pid->SumError_limit) pid->SumError= pid->SumError_limit;
    else if(pid->SumError< -pid->SumError_limit) pid->SumError=-pid->SumError_limit;

    d_Error = Now_Error - pid->Last_error;//误差之差，表示微分。
    pid->Last_error=Now_Error;

    pid->Out_put = pid->P * Now_Error +
                   pid->I * pid->SumError +
                   pid->D * d_Error;

    //限幅输出
    if(pid->Out_put     > pid->Output_limit) pid->Out_put= pid->Output_limit;
    else if(pid->Out_put<-pid->Output_limit) pid->Out_put=-pid->Output_limit;

    pid->Last_Out_put = pid->Out_put;
}


//增量式PID计算（用于速度环，其输出作为电调的控制电流值）
void PID_IncCalc(PIDTypeDef *pid,int16_t feedbackspeed)//增量式
{
    float p_Error,Now_Error,d_Error,i_Error,Now_Point;
    Now_Point = ((float )feedbackspeed*2*pi)/(256*6.33f);
    Now_Error=pid->Setpoint-Now_Point;
    p_Error=Now_Error - pid->Last_error;//p分量
    i_Error=Now_Error;//I分量
    d_Error=Now_Error - 2*pid->Last_error + pid->LLast_error;//D分量(LLast_error是增量式PID的标志)
    pid->Last_error=Now_Error;
    pid->LLast_error=pid->Last_error;
    pid->Out_put += (pid->P*p_Error+  //与位置式的最大区别在于这里用的是“+=”而非“=”。
                              pid->I*i_Error+  //积分被累加到了输出上，因此只需输出限幅，而无需积分限幅。
                              pid->D*d_Error);
    //限幅输出
    if(pid->Out_put>pid->Output_limit) pid->Out_put=pid->Output_limit;
    else if (pid->Out_put<-pid->Output_limit) pid->Out_put=-pid->Output_limit;
/*
	为什么速度环采用增量式PID？
		因为速度环往往是要求保持一定的速度，当我们需要维持一定的速度时，就需要维持一定的电流，
		一旦当前速度与目标速度产生偏差，增量式PID就会对应产生一个增量输出，从而对电流值进行调节。
		因而，可以说，如果说位置式pid的输出对应着当前值与目标值的偏差量，那么增量式pid则对应着随时
		发生的变化量。即，位置式只要有误差就有输出，而增量式只有误差发生变化才有输出。

*/
}

/*!
 * 修改伪8自由度的8个电机的PID参数
 * @param pos_kp
 * @param pos_kd
 */
void ChangeGainOfPID(float pos_kp,float pos_kd,float sp_kp,float sp_ki)
{
    for (uint8_t i=1;i<9;i++)
    {
        PID_Set_KP_KI_KD(&AngleLoop[i],pos_kp,0,pos_kd);
        PID_Set_KP_KI_KD(&SpeedLoop[i],sp_kp,sp_ki,0);
    }
}
//相比上个函数，提供了调节速度环ki的api，调整更加灵活（位置环的ki仍然始终为0，因为作为外环没必要这么多参数，反而会增加太调节的难度）。
void ChangeAllGainOfPID(float sp_kp,float sp_kd,float sp_ki,float pos_kp,float pos_kd)
{
    for(uint8_t i=0; i<8; i++)
    {
        PID_Set_KP_KI_KD(&SpeedLoop[i],sp_kp,sp_ki,sp_kd);//速度环刚度调整
        PID_Set_KP_KI_KD(&AngleLoop[i],pos_kp,0,pos_kd);//位置环刚度调整
    }
}
//每个腿的PID的单独配置
void LegPID_Set(uint8_t LegId,float sp_kp,float sp_kd,float sp_ki,float pos_kp,float pos_kd)
{
    switch (LegId) {
        case 0:
            PID_Set_KP_KI_KD(&SpeedLoop[1],sp_kp,sp_ki,sp_kd);
            PID_Set_KP_KI_KD(&AngleLoop[1],pos_kp,0,pos_kd);
            PID_Set_KP_KI_KD(&SpeedLoop[2],sp_kp,sp_ki,sp_kd);
            PID_Set_KP_KI_KD(&AngleLoop[2],pos_kp,0,pos_kd);
            break;
        case 1:
            PID_Set_KP_KI_KD(&SpeedLoop[3],sp_kp,sp_ki,sp_kd);
            PID_Set_KP_KI_KD(&AngleLoop[3],pos_kp,0,pos_kd);
            PID_Set_KP_KI_KD(&SpeedLoop[4],sp_kp,sp_ki,sp_kd);
            PID_Set_KP_KI_KD(&AngleLoop[4],pos_kp,0,pos_kd);
            break;
        case 2:
            PID_Set_KP_KI_KD(&SpeedLoop[5],sp_kp,sp_ki,sp_kd);
            PID_Set_KP_KI_KD(&AngleLoop[5],pos_kp,0,pos_kd);
            PID_Set_KP_KI_KD(&SpeedLoop[6],sp_kp,sp_ki,sp_kd);
            PID_Set_KP_KI_KD(&AngleLoop[6],pos_kp,0,pos_kd);
            break;
        case 3:
            PID_Set_KP_KI_KD(&SpeedLoop[7],sp_kp,sp_ki,sp_kd);
            PID_Set_KP_KI_KD(&AngleLoop[7],pos_kp,0,pos_kd);
            PID_Set_KP_KI_KD(&SpeedLoop[8],sp_kp,sp_ki,sp_kd);
            PID_Set_KP_KI_KD(&AngleLoop[8],pos_kp,0,pos_kd);
            break;
        default:
            break;
    }
}
//前、后腿的PID配置
void FBLegsPID_Set(uint8_t Leg_FB,float sp_kp,float sp_kd,float sp_ki,float pos_kp,float pos_kd)
{
    if(Leg_FB==Leg_Front)
    {
        LegPID_Set(0,sp_kp,sp_kd,sp_ki,pos_kp,pos_kd);
        LegPID_Set(2,sp_kp,sp_kd,sp_ki,pos_kp,pos_kd);
    }
    else
    {
        LegPID_Set(1,sp_kp,sp_kd,sp_ki,pos_kp,pos_kd);
        LegPID_Set(3,sp_kp,sp_kd,sp_ki,pos_kp,pos_kd);
    }
}
/*!
 * 修改陀螺仪PID参数
 * @param pos_kp
 * @param pos_kd
 */
void ChangeYawOfPID(float Yaw_Kp,float Yaw_Kd,float sum_error,float output_limit)
{
    Yaw_PID_Loop.P = Yaw_Kp;
    Yaw_PID_Loop.D = Yaw_Kd;
    Yaw_PID_Loop.SumError_limit = sum_error;
    Yaw_PID_Loop.Output_limit = output_limit;
}

void ChangePID(PIDTypeDef *PID,float Kp,float Kd,float sum_error,float output_limit)
{
    PID->P = Kp;
    PID->D = Kd;
    PID->SumError = sum_error;
    PID->Output_limit = output_limit;
}

/*!
 * 初始化伪8自由度8个电机的PID参数
 */
void Eight_PID_Init(void )
{
    for(uint8_t i=1;i<9;i++)
    {
        PID_Init(&AngleLoop[i]);
        PID_Init(&SpeedLoop[i]);
    }

    AllLegsSpeedLimit(SpeedMode_VERYFAST);
    AllLegsTorqueLimit(10);
}

/**
 * 修改speed_kp的值，其实就是修改宇树电机速度模式时K_W的值
 * @param K_W  期待的speed_kp的值
 */
void Change_speed_kp(float K_W)
{
    speed_kp = K_W;
}