//
// Created by 1 on 2024-06-10.
//

#ifndef PANDOGIN_DOG_GO1_MOTOR_H
#define PANDOGIN_DOG_GO1_MOTOR_H

#include "pid.h"
#include "CanMotor.h"
#include "motor.h"
#include "main.h"

void GO1_Init(void );
void GO1_LeftOutput_Task(void );
void GO1_RightOutput_Task(void );
void PID_CalcTask(void );

#endif //PANDOGIN_DOG_GO1_MOTOR_H
