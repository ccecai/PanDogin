//
// Created by Joerange on 2024/1/18.
//
#include "main.h"
#include "Jump_Task.h"

extern float times;
extern uint8_t reverse_move_flag;
uint8_t Jump_flag = 0;
uint8_t wait_flag = 0,JumpOver_flag = 0;
float pitch = 0;

int ExecuteJump(uint8_t JumpType,float JumpAngle)
{
    if (JumpType == Bridge_Jump)//标准跳（对绝大多数地面都有较好的适应性，兼具高度和远度）
    {
        /***
         * 双木桥用的跳
         */
        speed_kp = 0.24f;
        /*跳跃过程的时间把控（以实测为主设置何时的时间，保证运动过程分段的合理性）*/
        const uint16_t prep_time = 800;       //准备时间，即收缩退准备起跳的时间  [s]  0.4
        const uint16_t launch_time = 260;    //伸展腿的持续时间                  [s]  0.2
        const uint16_t fall_time = 100;      //在空中飞翔的时间                 [s]  0.25（这个时间最好设置的小点）
        const uint16_t strech_time = 700;  //落地并用力支撑的时间              [s]  0.3（这个时间结束后就会立刻进入站立态了）
        /*跳跃的姿态把控（调节时，可按0.1的整数倍进行加减调整，如（LegSquatLenth-0.4））*/
        const float stance_height = LegLenthMin;  //跳跃之前腿的高度  [cm]，理论上应等于LegSquatLenth 11.2f，这里测试跳跃时可以使用LegLenthMin 10.7f
        const float jump_extension = LegLenthExtremeMax; //伸展跳跃的最大伸腿长度      [cm]，理论上应等于LegLenthMax 28
        const float jump_flylegheight = LegStandLenth; //飞翔时腿长度   [cm]，经验上应介于LegLenthMax 28与LegStandLenth 18.0f之间，是一个适中的值。
        const float jump_landlegheight = LegStandLenth; //落地时腿长度  [cm]，理论上应等于LegStandLenth
        //下蹲，准备起跳，持续时间为prep_time
        AllLegsSpeedLimit(SpeedMode_EARLYEX);
        ChangeGainOfPID(7.3f, 1.0f, 0, 0);//使用刚度小，阻尼大的增益
        SetPolarPositionAll_Delay(JumpAngle, stance_height, prep_time);
        //芜湖起飞（核心），持续时间为launch_time
        AllLegsSpeedLimit(30.0f);//速度拉满
//        AngleLoop[5].Output_limit = 28.0f;
//        AngleLoop[6].Output_limit = 28.0f;
        ChangeGainOfPID(50.0f,0.8f,0, 0);//使用刚度小，阻尼大的增益0
        SetPolarPositionAll_Delay(JumpAngle, jump_extension, launch_time);

        //飞翔过程（也即降落过程）中的姿态（核心），持续时间为fall_time
        AllLegsSpeedLimit(30.0f);
        ChangeGainOfPID(20.0f, 3.0f, 0, 0);//使用刚度小，阻尼大的增益
        SetPolarPositionAll_Delay(-34.0f, stance_height, fall_time);
        speed_kp = 0.06f;

        //脚用力准备站起来
        ChangeGainOfPID(15.0f,2.0f, 0, 0);//使用刚度小，阻尼大的增益
        SetPolarPositionAll_Delay(-80, jump_landlegheight, strech_time);
        //差不多站好了，执行
        IMU_Slove(1,0);
        gpstate = 1;
        speed_kp = 0.14f;
    }
    else if(JumpType == StepUp_Jump)
    {
        /**
         * 上台阶用的跳
         */
        speed_kp = 0.24f;
//        Control_Flag(0,0);//选择是否开启陀螺仪与视觉纠偏开关

        IMU_Slove(1,0);//是否开启障碍时腿时刻保持竖直
        /*跳跃过程的时间把控（以实测为主设置何时的时间，保证运动过程分段的合理性）*/
        const uint16_t prep_time = 800;       //准备时间，即收缩退准备起跳的时间  [s]  0.4
        const uint16_t launch_time = 190;    //伸展腿的持续时间                  [s]  0.2
        const uint16_t fall_time = 200;      //在空中飞翔的时间                 [s]  0.25（这个时间最好设置的小点）
        const uint16_t strech_time = 400;  //落地并用力支撑的时间              [s]  0.3（这个时间结束后就会立刻进入站立态了）
        /*跳跃的姿态把控（调节时，可按0.1的整数倍进行加减调整，如（LegSquatLenth-0.4））*/
        const float stance_height = LegLenthMin;  //跳跃之前腿的高度  [cm]，理论上应等于LegSquatLenth 11.2f，这里测试跳跃时可以使用LegLenthMin 10.7f
        const float jump_extension = LegLenthExtremeMax; //伸展跳跃的最大伸腿长度      [cm]，理论上应等于LegLenthMax 28
        const float jump_flylegheight = LegStandLenth; //飞翔时腿长度   [cm]，经验上应介于LegLenthMax 28与LegStandLenth 18.0f之间，是一个适中的值。
        const float jump_landlegheight = LegStandLenth; //落地时腿长度  [cm]，理论上应等于LegStandLenth
        //下蹲，准备起跳，持续时间为prep_time
        AllLegsSpeedLimit(SpeedMode_EARLYEX);
        ChangeGainOfPID(7.3f, 1.0f, 0, 0);//使用刚度小，阻尼大的增益
        SetPolarPositionAll_Delay(JumpAngle, stance_height, prep_time);


        //芜湖起飞（核心），持续时间为launch_time
        AllLegsSpeedLimit(26.7f);//速度拉

        ChangeGainOfPID(40.0f,0.8f,0, 0);//使用刚度小，阻尼大的增益0
        SetPolarPositionAll_Delay(JumpAngle, jump_extension, launch_time);
        speed_kp = 0.14f;

        //飞翔过程（也即降落过程）中的姿态（核心），持续时间为fall_time
        AllLegsSpeedLimit(20.0f);
        ChangeGainOfPID(15.0f, 3.0f, 0, 0);//使用刚度小，阻尼大的增益
        SetPolarPositionAll_Delay(-25, jump_flylegheight, fall_time);

        //脚用力准备站起来
        speed_kp = 0.1f;
        ChangeGainOfPID(8.0f,0.1f, 0, 0);//使用刚度小，阻尼大的增益
        SetPolarPositionAll_Delay(73, jump_landlegheight, strech_time);

        speed_kp = 0.14f;
        //差不多站好了，执行
        gpstate = 1;
    }
    else if(JumpType == StepDown_Jump)//简单跳个远（要求地面摩擦较大）
    {
        /**
         * 下台阶用的跳
         */
        speed_kp = 0.24f;
//        Control_Flag(0,0);//选择是否开启陀螺仪与视觉纠偏开关

        IMU_Slove(1,0);//是否开启障碍时腿时刻保持竖直
        /*跳跃过程的时间把控（以实测为主设置何时的时间，保证运动过程分段的合理性）*/
        const uint16_t prep_time = 800;       //准备时间，即收缩退准备起跳的时间  [s]  0.4
        const uint16_t launch_time = 190;    //伸展腿的持续时间                  [s]  0.2
        const uint16_t fall_time = 200;      //在空中飞翔的时间                 [s]  0.25（这个时间最好设置的小点）
        const uint16_t strech_time = 300;  //落地并用力支撑的时间              [s]  0.3（这个时间结束后就会立刻进入站立态了）
        /*跳跃的姿态把控（调节时，可按0.1的整数倍进行加减调整，如（LegSquatLenth-0.4））*/
        const float stance_height = LegLenthMin;  //跳跃之前腿的高度  [cm]，理论上应等于LegSquatLenth 11.2f，这里测试跳跃时可以使用LegLenthMin 10.7f
        const float jump_extension = LegLenthMax; //伸展跳跃的最大伸腿长度      [cm]，理论上应等于LegLenthMax 28
        const float jump_flylegheight = LegStandLenth; //飞翔时腿长度   [cm]，经验上应介于LegLenthMax 28与LegStandLenth 18.0f之间，是一个适中的值。
        const float jump_landlegheight = LegStandLenth; //落地时腿长度  [cm]，理论上应等于LegStandLenth
        //下蹲，准备起跳，持续时间为prep_time
        AllLegsSpeedLimit(SpeedMode_EARLYEX);
        ChangeGainOfPID(7.3f, 1.0f, 0, 0);//使用刚度小，阻尼大的增益
        SetPolarPositionAll_Delay(JumpAngle, stance_height, prep_time);

        //芜湖起飞（核心），持续时间为launch_time
        AllLegsSpeedLimit(22.0f);//速度拉

        ChangeGainOfPID(24.0f,0.8f,0, 0);//使用刚度小，阻尼大的增益0
        SetPolarPositionAll_Delay(JumpAngle, jump_extension, launch_time);

        //飞翔过程（也即降落过程）中的姿态（核心），持续时间为fall_time
        AllLegsSpeedLimit(20.0f);
        ChangeGainOfPID(15.0f, 3.0f, 0, 0);//使用刚度小，阻尼大的增益
        SetPolarPositionAll_Delay(-25, jump_flylegheight, fall_time);

        //脚用力准备站起来
        speed_kp = 0.1f;
        ChangeGainOfPID(8.0f,0.1f, 0, 0);//使用刚度小，阻尼大的增益
        SetPolarPositionAll_Delay(-87, jump_landlegheight, strech_time);

        speed_kp = 0.14f;
        //差不多站好了，执行
        gpstate = 1;
    }
}
/**
 * 前空翻的函数
 * @return
 */
int FrontJump(void )
{

//    Control_Flag(0,0);//选择是否开启陀螺仪与视觉纠偏开关
    speed_kp = 0.24f;
    IMU_Slove(0,1);//是否开启障碍时腿时刻保持竖直
    /*跳跃过程的时间把控（以实测为主设置何时的时间，保证运动过程分段的合理性）*/
    const uint16_t prep_time = 1000;       //准备时间，即收缩退准备起跳的时间  [s]  0.4
    const uint16_t launch_time = 250;    //伸展腿的持续时间                  [s]  0.2
    /*跳跃的姿态把控（调节时，可按0.1的整数倍进行加减调整，如（LegSquatLenth-0.4））*/
    const float stance_height = LegLenthMin;  //跳跃之前腿的高度  [cm]，理论上应等于LegSquatLenth 11.2f，这里测试跳跃时可以使用LegLenthMin 10.7f

    //下蹲，准备起跳，持续时间为prep_time
    AllLegsSpeedLimit(15.0f);
    ChangeGainOfPID(7.5f, 1.0f, 0, 0);//使用刚度小，阻尼大的增益
    wait_flag = 1;
    SetPolarPositionLeg_Delay(77.0f, stance_height, prep_time,0);
    SetPolarPositionLeg_Delay(77.0f, stance_height, prep_time,2);
    SetPolarPositionLeg_Delay(85.0f, stance_height, prep_time,1);
    SetPolarPositionLeg_Delay(85.0f, stance_height, prep_time,3);
    osDelay(prep_time);

    //后腿起跳的函数
    speed_kp = 0.24f;//增加顺时阻尼
    AllLegsSpeedLimit(27.0f);//腾空不要太快留有余量利用惯性来前进
    ChangeGainOfPID(40.0f,2.0f,0, 0);//使用刚度小，阻尼大的增益0
    SetPolarPositionLeg_Delay(85.0f, LegLenthExtremeMax, launch_time,1);
    SetPolarPositionLeg_Delay(85.0f, LegLenthExtremeMax, launch_time,3);
    osDelay(300);
    speed_kp = 0.2f;
/**
 * 先将后退置于立正状态。防止没有翻过去的时候出现奇怪的姿态
 */
    AngleLoop[3].Output_limit = 25;
    AngleLoop[4].Output_limit = 25;
    AngleLoop[7].Output_limit = 25;
    AngleLoop[8].Output_limit = 25;
    PID_Set_KP_KI_KD(&AngleLoop[3],20.0f,0,0.8f);
    PID_Set_KP_KI_KD(&AngleLoop[4],20.0f,0,0.8f);
    PID_Set_KP_KI_KD(&AngleLoop[7],20.0f,0,0.8f);
    PID_Set_KP_KI_KD(&AngleLoop[8],20.0f,0,0.8f);
    Get_Target(0,PI);
    SetCoupledThetaPosition(1);
    SetCoupledThetaPosition(3);

/**
 * 如果没有翻过去还可以返回站立态
 */
    while (Pitch_Data > -87.0f)
    {
        if(gpstate == 1)
        {
            wait_flag = 0;
            return 0;
        }
    }

    osDelay(40);
/**
 * 翻过去以后上面的腿准备落地
 */
    AngleLoop[3].Output_limit = 20;
    AngleLoop[4].Output_limit = 20;
    AngleLoop[7].Output_limit = 20;
    AngleLoop[8].Output_limit = 20;
    PID_Set_KP_KI_KD(&AngleLoop[3],15.0f,0,0.8f);
    PID_Set_KP_KI_KD(&AngleLoop[4],15.0f,0,0.8f);
    PID_Set_KP_KI_KD(&AngleLoop[7],15.0f,0,0.8f);
    PID_Set_KP_KI_KD(&AngleLoop[8],15.0f,0,0.8f);
    Get_Target(-PI,2 * PI);
    SetCoupledThetaPosition(1);
    SetCoupledThetaPosition(3);

    wait_flag = 0;
    speed_kp = 0.25f;
/**
 * 前面的腿准备起跳
 */
    AllLegsSpeedLimit(40.0f);//速度拉满
    ChangeGainOfPID(45.0f,0.8f,0, 0);//使用刚度小，阻尼大的增益0
    SetPolarPositionLeg_Delay(77.0f, LegLenthExtremeMax, launch_time,0);
    SetPolarPositionLeg_Delay(77.0f, LegLenthExtremeMax, launch_time,2);

    osDelay(180);
/**
 * 缩腿过高栏
 */
    SetPolarPositionLeg_Delay(77.0f, LegLenthMin, 0,0);
    SetPolarPositionLeg_Delay(77.0f, LegLenthMin, 0,2);

    osDelay(150);

    IMU_Slove(0,0);
/**
 * 将阻尼与pid调小来做缓冲，并且将前腿准备立正
 */

    ChangeGainOfPID(20.0f,0.8f,0.03f,0.05f);//初始化pid
    AllLegsSpeedLimit(20.0f);
//以上电的位置作为基准，改变偏移量来完成空翻后的行走
    offset_front_0 -= PI;
    offset_front_1 += PI;
    offset_back_0 += PI;
    offset_back_1 -= PI;
//空翻后的前面两个腿
    SetPolarPositionLeg_Delay(-35.0f, 15.5f, 0,1);
    SetPolarPositionLeg_Delay(-35.0f, 15.5f, 0,3);

    Get_Target(0,PI);
    SetCoupledThetaPosition(0);
    SetCoupledThetaPosition(2);

    osDelay(500);
/**
 * 作缓冲
 */
    speed_kp = 0.14f;
    ChangeGainOfPID(15.0f,1.0f,0.03f,0.05f);//初始化pid


    //等待命令执行
    gpstate = 0;
}

void Turn_Jump(int16_t Jump_angle)
{
    /*跳跃过程的时间把控（以实测为主设置何时的时间，保证运动过程分段的合理性）*/
    const uint16_t prep_time = 300;       //准备时间，即收缩退准备起跳的时间  [s]  0.4
    const uint16_t launch_time = 200;    //伸展腿的持续时间                  [s]  0.2
    const uint16_t fall_time = 100;      //在空中飞翔的时间                 [s]  0.25（这个时间最好设置的小点）
    const uint16_t strech_time = 500;  //落地并用力支撑的时间              [s]  0.3（这个时间结束后就会立刻进入站立态了）
    /*跳跃的姿态把控（调节时，可按0.1的整数倍进行加减调整，如（LegSquatLenth-0.4））*/
    const float stance_height = LegLenthMin;  //跳跃之前腿的高度  [cm]，理论上应等于LegSquatLenth 11.2f，这里测试跳跃时可以使用LegLenthMin 10.7f
    const float jump_extension = LegLenthMax; //伸展跳跃的最大伸腿长度      [cm]，理论上应等于LegLenthMax 28
    const float jump_flylegheight = LegStandLenth; //飞翔时腿长度   [cm]，经验上应介于LegLenthMax 28与LegStandLenth 18.0f之间，是一个适中的值。
    const float jump_landlegheight = LegStandLenth; //落地时腿长度  [cm]，理论上应等于LegStandLenth
    switch (Jump_angle) {
        case 45:
            speed_kp = 0.24f;

            //下蹲，准备起跳，持续时间为prep_time
            AllLegsSpeedLimit(SpeedMode_EARLYEX);
            ChangeGainOfPID(10.0f, 1.0f, 0, 0);//使用刚度小，阻尼大的增益
            SetPolarPositionLeg_Delay(70.0f, TrunJump_Length, 0,0);
            SetPolarPositionLeg_Delay(70.0f, TrunJump_Length, 0,1);
            SetPolarPositionLeg_Delay(-70.0f, TrunJump_Length, 0,2);
            SetPolarPositionLeg_Delay(-70.0f, TrunJump_Length, 0,3);

            osDelay(prep_time);
            //芜湖起飞（核心），持续时间为launch_time
            AllLegsSpeedLimit(15.4f);//速度拉满
            ChangeGainOfPID(35.0f,1.5f,0, 0);//使用刚度小，阻尼大的增益0
            SetPolarPositionLeg_Delay(70.0f, jump_extension, 0,0);
            SetPolarPositionLeg_Delay(70.0f, jump_extension, 0,1);
            SetPolarPositionLeg_Delay(-70.0f, jump_extension, 0,2);
            SetPolarPositionLeg_Delay(-70.0f, jump_extension, 0,3);

            osDelay(launch_time);

            //飞翔过程（也即降落过程）中的姿态（核心），持续时间为fall_time
            AllLegsSpeedLimit(30.0f);
            ChangeGainOfPID(20.0f, 3.0f, 0, 0);//使用刚度小，阻尼大的增益
            SetPolarPositionLeg_Delay(-25.0f, TrunJump_Length, 0,0);
            SetPolarPositionLeg_Delay(-25.0f, TrunJump_Length, 0,1);
            SetPolarPositionLeg_Delay(25.0f, TrunJump_Length, 0,2);
            SetPolarPositionLeg_Delay(25.0f, TrunJump_Length, 0,3);

            osDelay(fall_time);
            speed_kp = 0.06f;

            //脚用力准备站起来
            ChangeGainOfPID(15.0f,2.0f, 0, 0);//使用刚度小，阻尼大的增益
            StandUp_Posture();
            osDelay(strech_time);
//差不多站好了，执行

//            gpstate = 1;
            speed_kp = 0.15f;
            break;
        case -45:
            speed_kp = 0.28f;
            //下蹲，准备起跳，持续时间为prep_time
            AllLegsSpeedLimit(SpeedMode_EARLYEX);
            ChangeGainOfPID(10.0f, 1.0f, 0, 0);//使用刚度小，阻尼大的增益
            SetPolarPositionLeg_Delay(-70.0f, TrunJump_Length, 0,0);
            SetPolarPositionLeg_Delay(-70.0f, TrunJump_Length, 0,1);
            SetPolarPositionLeg_Delay(70.0f, TrunJump_Length, 0,2);
            SetPolarPositionLeg_Delay(70.0f, TrunJump_Length, 0,3);

            osDelay(prep_time);
            //芜湖起飞（核心），持续时间为launch_time
            AllLegsSpeedLimit(15.4f);//速度拉满
            ChangeGainOfPID(35.0f,1.5f,0, 0);//使用刚度小，阻尼大的增益0
            SetPolarPositionLeg_Delay(-70.0f, jump_extension, 0,0);
            SetPolarPositionLeg_Delay(-70.0f, jump_extension, 0,1);
            SetPolarPositionLeg_Delay(70.0f, jump_extension, 0,2);
            SetPolarPositionLeg_Delay(70.0f, jump_extension, 0,3);

            osDelay(launch_time);

            //飞翔过程（也即降落过程）中的姿态（核心），持续时间为fall_time
            AllLegsSpeedLimit(30.0f);
            ChangeGainOfPID(20.0f, 3.0f, 0, 0);//使用刚度小，阻尼大的增益
            SetPolarPositionLeg_Delay(25.0f, TrunJump_Length, 0,0);
            SetPolarPositionLeg_Delay(25.0f, TrunJump_Length, 0,1);
            SetPolarPositionLeg_Delay(-25.0f, TrunJump_Length, 0,2);
            SetPolarPositionLeg_Delay(-25.0f, TrunJump_Length, 0,3);

            osDelay(fall_time);
            speed_kp = 0.06f;

            //脚用力准备站起来
            ChangeGainOfPID(15.0f,2.0f, 0, 0);//使用刚度小，阻尼大的增益
            StandUp_Posture();
            osDelay(strech_time);
            //差不多站好了，执行

//            gpstate = 1;
            speed_kp = 0.15f;
            break;
        default:
            break;
    }
}
