//
// Created by 1 on 2024-01-19.
//
#include "visual.h"

uint8_t VISUAL_REC[Length_of_visual] = {0};
Visial_data visual;
uint8_t visual_control_flag = 0;
float distance[5] = {200,200,200,200,200};
float Distance = 0;

void visual_process(void)
{
    static uint8_t count = 0;

    if(visual_control_flag == 1 && Front_Camare_flag == 1)
    {
        visual.distance = (Desk_Data[1]-48) * 1000.0f + (Desk_Data[2]-48) * 100.0f + (Desk_Data[3]-48) * 10.0f + (Desk_Data[4]-48) * 1.0f ;
        visual.offset = (Desk_Data[5]-48) * 100.0f + (Desk_Data[6]-48) * 10.0f + (Desk_Data[7]-48) * 1.0f ;

        distance[count] = visual.distance;
        if(count++ == 2)
            count = 0;

        Distance = (distance[0] + distance[1] + distance[2]) / 3;

        if(visual.distance < -3000 || visual.distance > 3000)
            visual.distance = 200;
        if(visual.offset < -300 || visual.offset > 300)
            visual.offset = 100;
    }

    else if(visual_control_flag == 1 && Back_Camare_flag == 1)
    {
        visual.distance = (Desk_Data[9]-48) * 1000.0f + (Desk_Data[10]-48) * 100.0f + (Desk_Data[11]-48) * 10.0f + (Desk_Data[12]-48) * 1.0f ;
        visual.offset = (Desk_Data[13]-48) * 100.0f + (Desk_Data[14]-48) * 10.0f + (Desk_Data[15]-48) * 1.0f ;

        distance[count] = visual.distance;
        if(count++ == 2)
            count = 0;

        Distance = (distance[0] + distance[1] + distance[2]) / 3;

        if(visual.distance < -3000 || visual.distance > 3000)
            visual.distance = 200;
        if(visual.offset < -300 || visual.offset > 300)
            visual.offset = 100;
    }
}