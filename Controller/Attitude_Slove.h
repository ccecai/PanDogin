//
// Created by 1 on 2023-11-07.
//

#ifndef MY_SCUDOG_ATTITUDE_SLOVE_H
#define MY_SCUDOG_ATTITUDE_SLOVE_H

#include "Attitude_Task.h"
#include "main.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"

#define PI 3.1415926535f
//��С�ȳ�
#define L1 11.0f //????100mm
#define L2 22.0f//?��??200mm
//�ȳ���λ
#define LegLenthExtremeMax 33.0f //
#define LegLenthMax 31.0f //
#define LegLenthMin 11.5f //
#define LegStandLenth 17.5f //
#define LegSquatLenth 12.0f //

#define TrunJump_Length 13.7f

#define wooden_bridge_offset 2.0f
//״̬����������
#define StatesMaxNum 20

//#define MidPoint 100.2


typedef struct
{
    float stance_height ;//����������ľ���
    float step_length ;//һ���ľ���
    float up_amp ;//�ϲ����
    float down_amp ;//�²����
    float flight_percent ;//�ڶ���ٷֱ�
    float freq ;//һ����Ƶ��
} GaitParams;
//�����ȵĽṹ�����

typedef struct
{
    uint8_t GaitID;
    GaitParams detached_params_0;
    GaitParams detached_params_1;
    GaitParams detached_params_2;
    GaitParams detached_params_3;

} DetachedParam;

extern DetachedParam state_detached_params[StatesMaxNum];
extern DetachedParam StateDetachedParams_Copy[];
extern float AngleWant_MotorX[9];
extern float x,y,Target_Slope,Target_Xpos,Target_Ypos;
extern float offset_front_0,offset_front_1,offset_back_0,offset_back_1;
extern uint8_t Barrier_flag,Radar_control_flag,FrontJump_flag;
extern float normal_step_left ,normal_step_right ;
extern float MidPoint;
void Get_Target(float theta1,float theta2);
void SetCoupledThetaPositionAll(void);
void SetCoupledThetaPosition(int LegId);
void Output_Angle(void);
void gait_detached(	DetachedParam d_params,
                       float leg0_offset, float leg1_offset,float leg2_offset, float leg3_offset,
                       float leg0_direction, float leg1_direction,float leg2_direction, float leg3_direction);
void Change_SinStateDetachedParams(DetachedParam *State,int8_t id,int8_t legid,float stance_height,float step_length,
                                   float up_amp,float down_amp,float flight_percent,float freq);
void CartesianToTheta(void);
void SinTrajectory (float t,GaitParams params, float gaitOffset,float leg_diretion,float angle,int LegId);
void CoupledMoveLeg(float t, GaitParams params,float gait_offset, float leg_direction, int LegId, float angle);
void YawControl(float yaw_set,DetachedParam *State_Detached_Params,int direction);
void SetPolarPositionAll_Delay(float polar_angle,float polar_diameter,uint16_t delaytime);
void SetCartesianPositionAll_Delay(float x_want,float y_want,uint16_t delaytime);
void SetCoupledCartesianPosition(int LegId,float x_want,float y_want);
void ReverseMoveOpen(void);
void ReverseMoveClose(void);
void SetCartesianPositionFB_Delay(int Leg_FB,float x_want,float y_want,uint16_t delaytime);
void IMU_Slove(uint8_t b_flag,uint8_t f_flag);
void SinTrajectory_Slope (float t,GaitParams params, float gaitOffset,float leg_diretion,float angle,int LegId);
void SetPolarPosition_Delay(int8_t leg_id,float polar_angle,float polar_diameter,uint16_t delaytime);
void SetPolarPositionLeg_Delay(float polar_angle,float polar_diameter,uint16_t delaytime,uint8_t Legid);

#endif //MY_SCUDOG_ATTITUDE_SLOVE_H