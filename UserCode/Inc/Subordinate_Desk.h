//
// Created by 1 on 2024-05-20.
//

#ifndef PANDOGIN_DOG_SUBORDINATE_DESK_H
#define PANDOGIN_DOG_SUBORDINATE_DESK_H

#include "main.h"
#include "cmsis_os.h"

#define Length_of_Desk 30

typedef struct {
    float x_pos;
    float y_pos;
    float z_pos;
    float yaw;
    float roll;
    float pitch;

    float Last_x_pos;
    float Last_y_pos;
    float Last_z_pos;
}Radar_Final;

typedef struct {
    int x_data;
    int y_data;
    int z_data;
    int yaw_data;
    int roll_data;
    int pitch_data;
}Radar_int;

extern Radar_Final Radar_FinalData;
extern Radar_int Radar_intdata;
extern uint8_t Desk_Data[Length_of_Desk];

void Process(void );

#endif //PANDOGIN_DOG_SUBORDINATE_DESK_H
