//
// Created by 1 on 2024-01-19.
//
#include "visual.h"

uint8_t VISUAL_REC[Length_of_visual] = {0};
Visial_data visual;
uint8_t visual_control_flag = 0;
float distance[Length_of_distance];
float Distance = 0;
uint8_t FrontIfLook = 0,BackIfLook = 0;

void visual_process(void)
{
    static uint8_t count = 0;
    float sum_distance = 0;

    if(visual_control_flag == 1 && Back_Camare_flag == 1)
    {
        visual.distance = (Desk_Data[1]-48) * 10.0f + (Desk_Data[2]-48) * 1.0f + (Desk_Data[3]-48) * 0.1f + (Desk_Data[4]-48) * 0.01f ;
        visual.offset = (Desk_Data[5]-48) * 100.0f + (Desk_Data[6]-48) * 10.0f + (Desk_Data[7]-48) * 1.0f ;

        BackIfLook = Desk_Data[8]-48;

        distance[count++] = visual.distance;
        if(count == Length_of_distance)
        {
            for (int i = 0; i < Length_of_distance; ++i) {
                sum_distance += distance[i];
            }
            Distance = sum_distance / Length_of_distance;
            count = 0;
        }

//        if(visual.distance < -3000 || visual.distance > 3000)
//            visual.distance = 200;
//        if(visual.offset < -300 || visual.offset > 300)
//            visual.offset = 100;
    }

    else if(visual_control_flag == 1 && Front_Camare_flag == 1)
    {
        visual.distance = (Desk_Data[10]-48) * 10.0f + (Desk_Data[11]-48) * 1.0f + (Desk_Data[12]-48) * 0.1f + (Desk_Data[13]-48) * 0.01f ;
        visual.offset = (Desk_Data[14]-48) * 100.0f + (Desk_Data[15]-48) * 10.0f + (Desk_Data[16]-48) * 1.0f;

        FrontIfLook = Desk_Data[17]-48;

        distance[count++] = visual.distance;
        if(count == Length_of_distance)
        {
            for (int i = 0; i < Length_of_distance; ++i) {
                sum_distance += distance[i];
            }
            Distance = sum_distance / Length_of_distance;
            count = 0;
        }

//        if(visual.distance < -3000 || visual.distance > 3000)
//            visual.distance = 200;
//        if(visual.offset < -300 || visual.offset > 300)
//            visual.offset = 100;
    }
}