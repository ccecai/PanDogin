//
// Created by 1 on 2024-06-19.
//

#ifndef PANDOGIN_DOG_BARRIER_TASK_H
#define PANDOGIN_DOG_BARRIER_TASK_H

#include "main.h"

#define Point_of_HighSpeed 7.2f
#define Point_of_Jumpbridge 6.1f //5.98
#define Point_of_JumpDownbridge 3.3f
#define Point_of_stop 0.8f
#define Point_of_Turn 1.62f
#define Point_of_OneJumpStairs 5.4f //5.314
#define Point_of_TwoJumpStairs 5.04f // 5.04
#define Point_of_ThreeJumpStairs 3.85f //4.06
#define Point_of_FourJumpStairs 160.0f
#define Angle_of_outofcontrol 15.0f
#define RightOffsetDistance 110.0f
#define LeftOffsetDistance 90.0f
#define Point_of_JumpHighBar 4.65f //4.53
#define Point_of_HighSpeedBar 5.9f
#define Point_of_HighSpeedStairs 6.9f

extern uint8_t offset_flag;
extern uint8_t angle_pitch_flag;
extern int bridge_count,stairs_count,bar_count,slope_count,automation_flag;
extern uint8_t stairs_flag;

void Barrier_of_Double_wooden_bridge(void );
void Barrier_of_Slope(void );
void Barrier_of_Stairs(void );
void Barrier_of_High_Bar(void );
void Test_Barrier_of_Double_wooden_bridge(void );
void Test_Barrier_of_Slope(void );
void Test_Barrier_of_Stairs(void );
void Test_Barrier_of_High_Bar(void );
void Barrier_Competition(void );

#endif //PANDOGIN_DOG_BARRIER_TASK_H
