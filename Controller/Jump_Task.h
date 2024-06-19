//
// Created by Joerange on 2024/1/18.
//

#ifndef ROBOMASTER_C_DEMO_JUMP_TASK_H
#define ROBOMASTER_C_DEMO_JUMP_TASK_H

#include "cmsis_os.h"

#define Leg_Back_Angle 85.0f //go1的极限速度
enum JumpTypes
{
    Bridge_Jump = 0,
    StepUp_Jump = 1,
    StepDown_Jump = 2,

};

extern uint8_t Jump_flag;
extern float pitch ;
extern uint8_t wait_flag,JumpOver_flag;

int ExecuteJump(uint8_t JumpType,float JumpAngle);
int FrontJump(void );
void Turn_Jump(int16_t Jump_angle);

#endif //ROBOMASTER_C_DEMO_JUMP_TASK_H
