//
// Created by Joerange on 2024/1/18.
//

#ifndef ROBOMASTER_C_DEMO_JUMP_TASK_H
#define ROBOMASTER_C_DEMO_JUMP_TASK_H
#define SpeedMode_JUMPPEDAL 8900 //go1�ļ����ٶ�
enum JumpTypes
{
    Standard_Jump = 0,
    High_Jump =     1,
    Far_Jump =      2,
    Step_Jump =     3,
    Leap_Jump =     4,
    Test_Jump =     5,
};

void ExecuteJump(uint8_t JumpType,float JumpAngle);
#endif //ROBOMASTER_C_DEMO_JUMP_TASK_H