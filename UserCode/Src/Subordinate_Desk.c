//
// Created by 1 on 2024-05-20.
//
#include "Subordinate_Desk.h"

uint8_t Desk_Data[Length_of_Desk] = {0};

Radar_Final Radar_FinalData;
Radar_int Radar_intdata;

void Process(void )
{
    Radar_intdata.x_data = Desk_Data[1] | Desk_Data[2] << 8 | Desk_Data[3] << 16 | Desk_Data[4] << 24;
    Radar_intdata.y_data = Desk_Data[5] | Desk_Data[6] << 8 | Desk_Data[7] << 16 | Desk_Data[8] << 24;
    Radar_intdata.z_data = Desk_Data[9] | Desk_Data[10] << 8 | Desk_Data[11] << 16 | Desk_Data[12] << 24;
    Radar_intdata.roll_data = Desk_Data[13] | Desk_Data[14] << 8 | Desk_Data[15] << 16 | Desk_Data[16] << 24;
    Radar_intdata.pitch_data = Desk_Data[17] | Desk_Data[18] << 8 | Desk_Data[19] << 16 | Desk_Data[20] << 24;
    Radar_intdata.yaw_data = Desk_Data[21] | Desk_Data[22] << 8 | Desk_Data[23] << 16 | Desk_Data[24] << 24;

    Radar_FinalData.x_pos =(float ) Radar_intdata.x_data / 10000;
    Radar_FinalData.y_pos = (float ) Radar_intdata.y_data / 10000;
    Radar_FinalData.z_pos =(float )  Radar_intdata.z_data / 10000;
    Radar_FinalData.roll = (float ) Radar_intdata.yaw_data / 100;
    Radar_FinalData.pitch = (float ) Radar_intdata.roll_data / 100;
    Radar_FinalData.yaw = (float ) Radar_intdata.pitch_data / 100;
}