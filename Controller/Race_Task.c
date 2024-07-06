//
// Created by 1 on 2024-06-19.
//
#include "Race_Task.h"

void Race_Competition(void)
{
    if(visual.distance >= FrontLength_of_Jump && Race_count == 0)
    {
        Trot(Forward,1);
    }

    else if(visual.distance < FrontLength_of_Jump && Race_count == 0)
    {
        Turn_Jump(45);

        Front_Camare_flag = 0;
        Back_Camare_flag = 1;

        yawwant = -45.0f;

        Race_count++;
    }
    else if(visual.distance >= BackLength_of_Jump && Race_count == 1)
    {
        Trot(Backward,1);
    }
    else if(visual.distance < BackLength_of_Jump && Race_count == 1)
    {
        Turn_Jump(45);

        Front_Camare_flag = 1;
        Back_Camare_flag = 0;

        yawwant = -90.0f;

        Race_count++;
    }
    else if(Race_count == 2 && visual.distance >= FrontLength_of_Jump + 0.08)
    {
        Trot(Forward,1);
    }
    else if(Race_count == 2 && visual.distance < FrontLength_of_Jump + 0.08)
    {
        Turn_Jump(-45);

        Front_Camare_flag = 0;
        Back_Camare_flag = 1;

        yawwant = -45.0f;

        Race_count++;

    }
    else if(Race_count == 3 && visual.distance >= FrontLength_of_Jump)
    {
        Trot(Backward,1);
    }
    else if(Race_count == 3 && visual.distance < FrontLength_of_Jump)
    {
        StandUp_Posture();
    }
}
