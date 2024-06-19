//
// Created by 1 on 2024-06-19.
//
#include "Race_Task.h"

void Race_Competition(void)
{
    if(Distance >= FrontLength_of_Jump && Race_count == 0)
    {
        Trot(Forward,1);
    }

    if(Distance < FrontLength_of_Jump && Race_count == 0)
    {
        Turn_Jump(45);

        for (int i = 0; i < 5; ++i)
        {
            distance[i] = 300;
        }
        Distance = 300.0f;
        visual.offset = 100;
        yawwant = 45.0f;

        Front_Camare_flag = 0;
        Back_Camare_flag = 1;

        Race_count++;
    }
    if(Distance >= BackLength_of_Jump && Race_count == 1)
    {
        Trot(Backward,1);
    }
    if(Distance < BackLength_of_Jump && Race_count == 1)
    {
        Turn_Jump(45);

        for (int i = 0; i < 5; ++i) {
            distance[i] = 300;
        }

        Distance = 300.0f;
        visual.offset = 100;
        yawwant = -90.0f;

        Front_Camare_flag = 1;
        Back_Camare_flag = 0;

        Race_count++;
    }
    if(Race_count == 2 && Distance >= FrontLength_of_Jump)
    {
        Trot(Forward,1);
    }
    if(Race_count == 2 && Distance < FrontLength_of_Jump)
    {
        Turn_Jump(-45);

        for (int i = 0; i < 5; ++i) {
            distance[i] = 300;
        }
        Distance = 300.0f;
        visual.offset = 100;
        yawwant = 45.0f;

        Front_Camare_flag = 0;
        Back_Camare_flag = 1;

        Race_count++;

    }
    if(Race_count == 3 && Distance >= BackLength_of_Jump)
    {
        Trot(Backward,1);
    }
    if(Race_count == 3 && Distance < BackLength_of_Jump)
    {
        StandUp_Posture();
    }
}
