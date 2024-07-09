//
// Created by 1 on 2024-01-19.
//
#include "visual.h"

uint8_t VISUAL_REC[Length_of_visual] = {0};
Visial_data visual;
uint8_t visual_control_flag = 0;
float distance[Length_of_distance];
float Distance = 0;
float Last_Distance[2] = {0},Last_offset = 0;
uint8_t distance_flag = 0;
uint8_t FrontIfLook = 0,BackIfLook = 0;

void visual_process(void)
{
    if(BarrierMode_flag == 1 && Race_flag == 0)
    {
        if(Back_Camare_flag == 1 && Desk_Data[0] == 48 && Desk_Data[1] == 52 && Desk_Data[10] == 51 && Desk_Data[11] == 53)
        {
            visual.distance = (Desk_Data[2]-48) * 10.0f + (Desk_Data[3]-48) * 1.0f + (Desk_Data[4]-48) * 0.1f + (Desk_Data[5]-48) * 0.01f ;
            visual.offset = (Desk_Data[6]-48) * 100.0f + (Desk_Data[7]-48) * 10.0f + (Desk_Data[8]-48) * 1.0f ;

            BackIfLook = Desk_Data[9]-48;

        }

        else if(Front_Camare_flag == 1 && Desk_Data[12] == 49 && Desk_Data[13] == 53 && Desk_Data[22] == 52 && Desk_Data[23] == 54)
        {
            visual.distance = (Desk_Data[14]-48) * 10.0f + (Desk_Data[15]-48) * 1.0f + (Desk_Data[16]-48) * 0.1f + (Desk_Data[17]-48) * 0.01f ;
            visual.offset = (Desk_Data[18]-48) * 100.0f + (Desk_Data[19]-48) * 10.0f + (Desk_Data[20]-48) * 1.0f;

            FrontIfLook = Desk_Data[21]-48;

        }
    }
    else if(BarrierMode_flag == 0 && Race_flag == 1)
    {
        if(Front_Camare_flag == 1 && Desk_Data[0] == 48 && Desk_Data[1] == 52 && Desk_Data[10] == 51 && Desk_Data[11] == 53)
        {
            visual.distance = (Desk_Data[2]-48) * 10.0f + (Desk_Data[3]-48) * 1.0f + (Desk_Data[4]-48) * 0.1f + (Desk_Data[5]-48) * 0.01f ;
            visual.offset = (Desk_Data[6]-48) * 100.0f + (Desk_Data[7]-48) * 10.0f + (Desk_Data[8]-48) * 1.0f ;

            BackIfLook = Desk_Data[9]-48;

        }

        else if(Back_Camare_flag == 1 && Desk_Data[12] == 49 && Desk_Data[13] == 53 && Desk_Data[22] == 52 && Desk_Data[23] == 54)
        {
            visual.distance = (Desk_Data[14]-48) * 10.0f + (Desk_Data[15]-48) * 1.0f + (Desk_Data[16]-48) * 0.1f + (Desk_Data[17]-48) * 0.01f ;
            visual.offset = (Desk_Data[18]-48) * 100.0f + (Desk_Data[19]-48) * 10.0f + (Desk_Data[20]-48) * 1.0f;

            FrontIfLook = Desk_Data[21]-48;

        }
    }

}