//
// Created by 1 on 2024-01-19.
//

#ifndef PANDOGIN_DOG_VISUAL_H
#define PANDOGIN_DOG_VISUAL_H

#include "main.h"
#include "queue.h"
#include "cmsis_os.h"

#define Length_of_visual 7
#define Length_of_distance 20

typedef struct
{
    uint8_t data_8[Length_of_visual];
    float distance;
    float offset;
}Visial_data;

extern osMessageQId VisialHandle;
extern uint8_t VISUAL_REC[Length_of_visual];
extern Visial_data visual;
extern uint8_t visual_control_flag;
extern float Distance;
extern float distance[Length_of_distance];
extern uint8_t FrontIfLook,BackIfLook;

void visual_process(void);

#endif //PANDOGIN_DOG_VISUAL_H
