//
// Created by 1 on 2024-06-19.
//
#include "Race_Task.h"

void Race_Competition(void)
{
    if(visual.distance >= FrontLength_of_Jump && Race_count == 0)
    {
        MidPoint = 96.8f;
        Trot(Forward,1);
    }

    else if(visual.distance < FrontLength_of_Jump && Race_count == 0)
    {
        if(Yaw_Data < 45.0f)
            Turn('l','f');
        else if(Yaw_Data >= 27.0f)
        {
            Front_Camare_flag = 0;
            Back_Camare_flag = 1;
            Race_count++;
            yawwant = 45.0f;

            StandUp_Posture();
            osDelay(300);
        }

    }

    else if(visual.distance >= BackLength_of_Jump && Race_count == 1)
    {
        MidPoint = 101.5f;
        Trot(Backward,7);
    }
    else if(visual.distance < BackLength_of_Jump && Race_count == 1)
    {
        if(Yaw_Data < 90.0f)
            Turn('l','f');
        else if(Yaw_Data >= 74.0f)
        {
            Race_count++;
            Front_Camare_flag = 1;
            Back_Camare_flag = 0;
            yawwant = 90.0f;

            StandUp_Posture();
            osDelay(300);
        }
    }

    else if(visual.distance >= FrontLength_of_Jump && Race_count == 2)
    {
        MidPoint = 96.8f;
        Trot(Forward,1);
    }
    else if(visual.distance < FrontLength_of_Jump && Race_count == 2)
    {
        if(Yaw_Data > 45.0f)
            Turn('r','f');
        else if(Yaw_Data < 64.8f)
        {
            Race_count++;
            Front_Camare_flag = 0;
            Back_Camare_flag = 1;
            yawwant = 45.0f;

            StandUp_Posture();
            osDelay(300);
        }
    }

    else if(Race_count == 3)
    {
        MidPoint = 100.5f;
        Trot(Backward,7);
    }

//    else if(visual.distance <= BackLength_of_Jump && Race_count == 3)
//    {
//        gpstate= 1;
//    }
}
