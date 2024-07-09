//
// Created by 1 on 2024-06-19.
//

#ifndef PANDOGIN_DOG_BARRIER_TASK_H
#define PANDOGIN_DOG_BARRIER_TASK_H

#include "main.h"

#define Point_of_HighSpeed 7.4f
#define Point_of_Jumpbridge 6.05f //5.98
#define Point_of_JumpDownbridge 3.28f

#define Point_of_stop 0.1f
#define Point_of_Turn 1.67f

#define Point_of_HighSpeedStairs 7.69f
#define Point_of_LowSpeedStairs 7.5f
#define Point_of_OneJumpStairs 5.36f //5.26
#define Point_of_ThreeJumpStairs 2.16f //4.063

#define Point_of_JumpHighBar 4.59f //4.53
#define Point_of_HighSpeedBar 6.0f


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
