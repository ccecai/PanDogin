//
// Created by Joerange on 2024/1/18.
//
#include "main.h"
#include "Jump_Task.h"

extern float times;
extern uint8_t reverse_move_flag;
uint8_t Jump_flag = 0;
uint8_t wait_flag = 0;
float pitch = 0;
int ExecuteJump(uint8_t JumpType,float JumpAngle)
{
    if (JumpType == Standard_Jump)//标准跳（对绝大多数地面都有较好的适应性，兼具高度和远度）
    {
        /*
         * 双木桥用的跳
         */
        /*跳跃过程的时间把控（以实测为主设置何时的时间，保证运动过程分段的合理性）*/
        const uint16_t prep_time = 800;       //准备时间，即收缩退准备起跳的时间  [s]  0.4
        const uint16_t launch_time = 250;    //伸展腿的持续时间                  [s]  0.2
        const uint16_t fall_time = 100;      //在空中飞翔的时间                 [s]  0.25（这个时间最好设置的小点）
        const uint16_t strech_time = 1000;  //落地并用力支撑的时间              [s]  0.3（这个时间结束后就会立刻进入站立态了）
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
        AllLegsSpeedLimit(29.0f);//速度拉满

//        AngleLoop[3].Output_limit = 29.4f;
//        AngleLoop[4].Output_limit = 29.4f;
        AngleLoop[5].Output_limit = 26.5f;
        AngleLoop[6].Output_limit = 26.5f;
//        AngleLoop[7].Output_limit = 28.0f;
//        AngleLoop[8].Output_limit = 28.0f;

        ChangeGainOfPID(50.0f,0.8f,0, 0);//使用刚度小，阻尼大的增益0
        SetPolarPositionAll_Delay(JumpAngle, jump_extension, launch_time);

        /*
        高刚度的实现：
        pos_kp很大：
                可以立刻输出一个很大的值给速度环，其值乘以角度差即为输出的速度目标值。3508速度上限8900，角度差按最小10度计算，则pos_kp可以设置为890。
        sp_kp很大：
                可以在当前速度与目标速度差别不是很大的时候也能输出很大的电流，更何况速度差别较大的时候。从而可以使电机的速度保持较大。
                C620电调电流上限值16384（对应20A），该值明显在数量级上小于通常所需要的速度值。
                我们的控制过程是需要将速度从8900快速变为0，速度值为250时已经很慢了，以它为标准，配置sp_kp为8已经够用了，因此速度环的PID是比较普适的，一个8走天下。
        因此，总的控制效果近似由下式决定：Speed=Current=angle*pos_kp。因此，若我们希望在临界目标角度10度前电机能始终保持最大速度，那么，我们可以由下式计算：
        pos_kp = CurrentMAX/angle_Thresh = 8900/10 = 890。10度已经是比较小了，angle_Thresh越小，越容易超调，从而造成危害。因此不建议小于10度。
        高刚度表现为，转动很小的角度都很费力，即很难转动电机。
        */
        //飞翔过程（也即降落过程）中的姿态（核心），持续时间为fall_time
        AllLegsSpeedLimit(20.0f);
        ChangeGainOfPID(15.0f, 3.0f, 0, 0);//使用刚度小，阻尼大的增益
        SetPolarPositionAll_Delay(-25, jump_flylegheight, fall_time);
        /*130
        低刚度：
            根据上述公式：pos_kp*sp_kp = CurrentMAX/angle_Thresh，低刚度意味着我们要选择一个略微大一点的angle_Thresh，从而我们可以小角度内比较容易摆动电机，而到一定角度则掰不动了。
            比如我们选择30度，则有：
            pos_kp = CurrentMAX/angle_Thresh = 8900/30 ≈ 300。故，为此可以配置pos_kp为300。
            低刚度表现为，在一定角度范围内比较容易转动，但角度越大越费力，达到临界阈值角度很难继续转动，并且持续时间越久越费力（因为有默认的I）。
        */
        //脚用力准备站起来
        speed_kp = 0.05f;
        ChangeGainOfPID(3.0f,0.01f, 0, 0);//使用刚度小，阻尼大的增益
        SetPolarPositionAll_Delay(-80, jump_landlegheight, strech_time);

        speed_kp = 0.22f;
        //差不多站好了，执行
        gpstate = 1;
    }
    else if(JumpType == High_Jump)//简单原地跳个高（任何地面都行）
    {

        /*跳跃过程的时间把控（以实测为主设置何时的时间，保证运动过程分段的合理性）*/
        const uint16_t prep_time = 800;       //准备时间，即收缩退准备起跳的时间  [s]  0.4
        const uint16_t launch_time = 200;    //伸展腿的持续时间                  [s]  0.2
        const uint16_t fall_time = 100;      //在空中飞翔的时间                 [s]  0.25（这个时间最好设置的小点）
        const uint16_t strech_time = 100;  //落地并用力支撑的时间              [s]  0.3（这个时间结束后就会立刻进入站立态了）
        /*跳跃的姿态把控（调节时，可按0.1的整数倍进行加减调整，如（LegSquatLenth-0.4））*/
        const float stance_height = LegLenthMin;  //跳跃之前腿的高度  [cm]，理论上应等于LegSquatLenth 11.2f，这里测试跳跃时可以使用LegLenthMin 10.7f
        const float jump_extension = LegLenthMax; //伸展跳跃的最大伸腿长度      [cm]，理论上应等于LegLenthMax 28
        const float jump_flylegheight = LegStandLenth; //飞翔时腿长度   [cm]，经验上应介于LegLenthMax 28与LegStandLenth 18.0f之间，是一个适中的值。
        const float jump_landlegheight = LegStandLenth; //落地时腿长度  [cm]，理论上应等于LegStandLenth 18.0f
        //下蹲，准备起跳，持续时间为prep_time
        AllLegsSpeedLimit(SpeedMode_VERYFAST);
        ChangeGainOfPID(10.0f,2.0f,0,0);//使用刚度小，阻尼大的增益
        SetPolarPositionAll_Delay(-(JumpAngle + 2),stance_height,prep_time);
        //芜湖起飞（核心），持续时间为launch_time
        AllLegsSpeedLimit(28.0f);//速度拉满
        ChangeGainOfPID(21.0f,3.0f,0,0);// 使用高刚度和低阻尼执行跳转
        SetPolarPositionAll_Delay(-JumpAngle,jump_extension,launch_time);
        //飞翔过程（也即降落过程）中的姿态（核心），持续时间为fall_time
        AllLegsSpeedLimit(SpeedMode_VERYFAST);
        ChangeGainOfPID(10.0f,1.0f,0,0);//使用低刚度和大量的阻尼来处理下降
        SetPolarPositionAll_Delay(40,jump_flylegheight,fall_time);
        //脚用力准备站起来
        ChangeGainOfPID(2.0f,0.01f,0.0f,0.0f);//使用低刚度和大量的阻尼来处理下降
        SetPolarPositionAll_Delay(85,jump_landlegheight,strech_time);
        //差不多站好了，执行完毕
        gpstate = HALT;
        IMU_Slove(1,0);
    }
    else if(JumpType == Far_Jump)//简单跳个远（要求地面摩擦较大）
    {
        /*跳跃过程的时间把控（以实测为主设置何时的时间，保证运动过程分段的合理性）*/
        const uint16_t prep_time = 1000;       //准备时间，即收缩退准备起跳的时间  [s]  0.4
        const uint16_t launch_time = 230;    //伸展腿的持续时间                  [s]  0.2
        const uint16_t fall_time = 220;      //在空中飞翔的时间                 [s]  0.25（这个时间最好设置的小点）
        const uint16_t strech_time = 600;  //落地并用力支撑的时间              [s]  0.3（这个时间结束后就会立刻进入站立态了）
        /*跳跃的姿态把控（调节时，可按0.1的整数倍进行加减调整，如（LegSquatLenth-0.4））*/
        const float stance_height = LegLenthMin;  //跳跃之前腿的高度  [cm]，理论上应等于LegSquatLenth 11.2f，这里测试跳跃时可以使用LegLenthMin 10.7f
        const float jump_extension = LegLenthExtremeMax; //伸展跳跃的最大伸腿长度      [cm]，理论上应等于LegLenthMax 28
        const float jump_flylegheight = LegStandLenth; //飞翔时腿长度   [cm]，经验上应介于LegLenthMax 28与LegStandLenth 18.0f之间，是一个适中的值。
        const float jump_landlegheight = LegStandLenth; //落地时腿长度  [cm]，理论上应等于LegStandLenth 18.0f
        //下蹲，准备起跳，持续时间为prep_time
        AllLegsSpeedLimit(SpeedMode_EXTREME);
        ChangeGainOfPID(11.0f,2.0f,0.0f,0.0f);//使用刚度小，阻尼大的增益
        SetPolarPositionAll_Delay(JumpAngle ,stance_height,prep_time);
        //芜湖起飞（核心），持续时间为launch_time

        speed_kp = 0.27f;
        AllLegsSpeedLimit(35.0f);//速度拉满
        ChangeGainOfPID(50.0f,0.0f,0.0f,0.0f);
        SetPolarPositionAll_Delay(JumpAngle,jump_extension,launch_time);

        //飞翔过程（也即降落过程）中的姿态（核心），持续时间为fall_time
        AllLegsSpeedLimit(30.0f);
        ChangeGainOfPID(30.0f,2.0f,0.0f,0.0f);//使用低刚度和大量的阻尼来处理下降
//        SetPolarPositionAll_Delay(20,jump_flylegheight,fall_time);
        SetPolarPositionLeg_Delay(20,jump_flylegheight,fall_time,0);
        SetPolarPositionLeg_Delay(20,jump_flylegheight,fall_time,2);

        AngleWant_MotorX[3] = 0;//+5.0f
        AngleWant_MotorX[4] = 0;
        AngleWant_MotorX[7] = 0;
        AngleWant_MotorX[8] = 0;

        osDelay(fall_time);

//        AngleWant_MotorX[3] = -1.26f;//+5.0f
//        AngleWant_MotorX[4] = PI - offset_back_1;
//        AngleWant_MotorX[7] = -PI + offset_back_1 ;
//        AngleWant_MotorX[8] = 1.26f;
//
//        osDelay(50);
        //脚用力准备站起来
        speed_kp = 0.2f;

//        speed_kp = 0.06f;
        ChangeGainOfPID(20.0f,0.01f,0.0f,0.0f);//使用低刚度和大量的阻尼来处理下降
        SetPolarPositionAll_Delay(-92,jump_landlegheight,strech_time);
//        speed_kp = 0.2f;
        //差不多站好了，执行完毕
        gpstate = 1;
    }
    else if(JumpType == Leap_Jump)//非常极限的四足同时跳。
    {
        /*跳跃过程的时间把控（以实测为主设置何时的时间，保证运动过程分段的合理性）*/
        const uint16_t prep_time = 800;       //准备时间，即收缩退准备起跳的时间  [s]  0.4
        const uint16_t launch_time = 200;    //伸展腿的持续时间                  [s]  0.2
        const uint16_t fall_time = 100;      //在空中飞翔的时间                 [s]  0.25（这个时间最好设置的小点）
        const uint16_t strech_time = 200;  //落地并用力支撑的时间              [s]  0.3（这个时间结束后就会立刻进入站立态了）
        /*跳跃的姿态把控（调节时，可按0.1的整数倍进行加减调整，如（LegSquatLenth-0.4））*/
        const float stance_height = LegLenthMin;  //跳跃之前腿的高度  [cm]，理论上应等于LegSquatLenth 11.2f，这里测试跳跃时可以使用LegLenthMin 10.7f
        const float jump_extension = LegLenthMax; //伸展跳跃的最大伸腿长度      [cm]，理论上应等于LegLenthMax 28
        const float jump_flylegheight = LegStandLenth; //飞翔时腿长度   [cm]，经验上应介于LegLenthMax 28与LegStandLenth 18.0f之间，是一个适中的值。
        const float jump_landlegheight = LegStandLenth; //落地时腿长度  [cm]，理论上应等于LegStandLenth 18.0f
        //下蹲，准备起跳，持续时间为prep_time
        AllLegsSpeedLimit(SpeedMode_VERYFAST);
        ChangeGainOfPID(12.0f,2.0f,0,0);//使用刚度小，阻尼大的增益
        SetPolarPositionAll_Delay(-(JumpAngle + 2),stance_height,prep_time);
        //芜湖起飞（核心），持续时间为launch_time
        AllLegsSpeedLimit(26.0f);//速度拉满
        ChangeGainOfPID(15.0f,3.0f,0,0);// 使用高刚度和低阻尼执行跳转

        SetPolarPositionAll_Delay(-JumpAngle,jump_extension,launch_time);
        //飞翔过程（也即降落过程）中的姿态（核心），持续时间为fall_time
        AllLegsSpeedLimit(SpeedMode_VERYFAST);
        ChangeGainOfPID(12.0f,0.8f,0,0);//使用低刚度和大量的阻尼来处理下降
        SetPolarPositionAll_Delay(40,jump_flylegheight,fall_time);
        //脚用力准备站起来
        speed_kp = 0.06f;

        ChangeGainOfPID(3.0f,0.01f,0.0f,0.0f);//使用低刚度和大量的阻尼来处理下降
        SetPolarPositionAll_Delay(86,jump_landlegheight,strech_time);

        speed_kp = 0.2f;
        //差不多站好了，执行完毕
        gpstate = HALT;
        IMU_Slove(1,0);
    }
}

int FrontJump(void )
{
    /*
         * 双木桥用的跳
         */
    /*跳跃过程的时间把控（以实测为主设置何时的时间，保证运动过程分段的合理性）*/
    const uint16_t prep_time = 1000;       //准备时间，即收缩退准备起跳的时间  [s]  0.4
    const uint16_t launch_time = 250;    //伸展腿的持续时间                  [s]  0.2
    /*跳跃的姿态把控（调节时，可按0.1的整数倍进行加减调整，如（LegSquatLenth-0.4））*/
    const float stance_height = LegLenthMin;  //跳跃之前腿的高度  [cm]，理论上应等于LegSquatLenth 11.2f，这里测试跳跃时可以使用LegLenthMin 10.7f

    //下蹲，准备起跳，持续时间为prep_time
    AllLegsSpeedLimit(15.0f);
    ChangeGainOfPID(7.5f, 1.0f, 0, 0);//使用刚度小，阻尼大的增益
    wait_flag = 1;
    SetPolarPositionLeg_Delay(80.0f, stance_height, prep_time,0);
    SetPolarPositionLeg_Delay(80.0f, stance_height, prep_time,2);
    SetPolarPositionLeg_Delay(85.0f, stance_height, prep_time,1);
    SetPolarPositionLeg_Delay(85.0f, stance_height, prep_time,3);
    osDelay(prep_time);

    //后腿起跳的函数
    speed_kp = 0.3f;//增加顺时阻尼
    AllLegsSpeedLimit(27.5f);//腾空不要太快留有余量利用惯性来前进
    ChangeGainOfPID(40.0f,2.0f,0, 0);//使用刚度小，阻尼大的增益0
    SetPolarPositionLeg_Delay(85.0f, LegLenthExtremeMax, launch_time,1);
    SetPolarPositionLeg_Delay(85.0f, LegLenthExtremeMax, launch_time,3);
    osDelay(300);
    speed_kp = 0.2f;
/**
 * 先将后退置于立正状态。防止没有翻过去的时候出现奇怪的姿态
 */
    AngleLoop[3].Output_limit = 20;
    AngleLoop[4].Output_limit = 20;
    AngleLoop[7].Output_limit = 20;
    AngleLoop[8].Output_limit = 20;
    PID_Set_KP_KI_KD(&AngleLoop[3],15.0f,0,0.8f);
    PID_Set_KP_KI_KD(&AngleLoop[4],15.0f,0,0.8f);
    PID_Set_KP_KI_KD(&AngleLoop[7],15.0f,0,0.8f);
    PID_Set_KP_KI_KD(&AngleLoop[8],15.0f,0,0.8f);
    Get_Target(0,PI);
    SetCoupledThetaPosition(1);
    SetCoupledThetaPosition(3);

/**
 * 如果没有翻过去还可以返回站立态
 */
    while (IMU_EulerAngle.EulerAngle[Pitch] > -85.0f)
    {
        if(gpstate == 1)
            return 0;
    }
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
    speed_kp = 0.3f;
/**
 * 前面的腿准备起跳
 */
    AllLegsSpeedLimit(40.0f);//速度拉满
    ChangeGainOfPID(45.0f,0.8f,0, 0);//使用刚度小，阻尼大的增益0
    SetPolarPositionLeg_Delay(80.0f, LegLenthExtremeMax, launch_time,0);
    SetPolarPositionLeg_Delay(80.0f, LegLenthExtremeMax, launch_time,2);

    osDelay(350);

    IMU_Slove(0,0);
/**
 * 将阻尼与pid调小来做缓冲，并且将前腿准备立正
 */
    speed_kp = 0.06f;

    ChangeGainOfPID(20.0f,0.8f,0.03f,0.05f);//初始化pid
    AllLegsSpeedLimit(20.0f);
    Get_Target(PI,0);
    SetCoupledThetaPosition(0);
    SetCoupledThetaPosition(2);

    osDelay(200);
/**
 * 作缓冲
 */
    ChangeGainOfPID(10.0f,0.1f,0.03f,0.05f);//初始化pid
    speed_kp = 0.2f;
    //等待命令执行
    gpstate = 0;
}

/***
//前空翻跳跃，即后腿先跳，随后是前腿。
//该跳跃有不同的模式，包括正常前空翻跳、侧旋跳、双足直立跳等（跳跃效果乃至成功与否与地面材质密切相关）。
//执行该跳跃时请做好保护措施，否则容易造成损失。
***/
void FrontFlipJump(uint8_t mode)
{

        uint8_t SeconTime=0;//判断是否是第二次到达
        uint32_t timedelay=0;//非阻塞延时，让跳跃与IMU结合更合理
        //IMU到达特定角度范围跳跃的控制变量
        uint8_t imu_wait_lock = 1;//IMU锁定
        float imu_fullscale_correction = 3;//即为理论满量程（90度）与实际满量程之差。
        float takeoff_inclination = 67-imu_fullscale_correction;//核心参数 （inclination：倾向）
        float imu_angle_half_range = 1.5f;//可以满足使用
        /*跳跃过程的时间把控（以实测为主设置何时的时间，保证运动过程分段的合理性）*/
        const uint16_t prep_time = 850;            //准备时间，即收缩退准备起跳的时间 [ms]
        const uint16_t backleg_jump_time  = 150;   //后退跳的持续时间				   [ms]
        const uint16_t frontleg_jump_time = 180;   //前腿跳的持续时间                [ms]
        /*跳跃的姿态把控*/
        const float stance_height = LegLenthMin+1.0f;//跳跃之前腿的高度  [cm]
        const float jump_extension = LegLenthExtremeMax;//最大是LegLenthExtremeMax
        const float backleg_jump_angle  =  81.0f;//理论上前腿后移主要由该角度不合适引起
        const float frontleg_squat_angle = 60.0f;
        const float delta_angle = 10;
        /***********************************************************/
        /*下蹲准备起跳，持续时间为prep_time*/
        ChangeGainOfPID(8.0f,1.0f,0,0);//使用刚度小，阻尼大的增益
        AllLegsSpeedLimit(SpeedMode_EARLYEX);
        //后腿下蹲
        SetPolarPositionFB_Delay(Leg_Back,backleg_jump_angle,stance_height,0);
        //前腿“倾斜”下蹲
        SetPolarPositionFB_Delay(Leg_Front,frontleg_squat_angle,stance_height,prep_time);
        /***********************************************************/
        /*后腿起跳，前腿立刻变化一段（这种情况下，若前腿变化合适，理论上转动的支点不再是足端，而应当看成是髋关节），持续时间为launch_time*/
        AllLegsSpeedLimit(SpeedMode_VERYEX);//速度拉满
        LegPID_Set(1,8,0.1f,5.5f,40,0.03f);
        LegPID_Set(3,8,0.1f,5.5f,40,0.03f);//使用高刚度和低阻尼执行跳转
        /*********后腿起跳**********/
        SetPolarPositionFB_Delay(Leg_Back,backleg_jump_angle,jump_extension,0);
        /*********前腿立刻再向前一点**********/
        LegPID_Set(0,5,0.1f,0.7f,12,2.0f);//ChangeAllGainOfPID(5,0.1,0.7,25,2.0);//站立态PID
        LegPID_Set(2,5,0.1f,0.7f,12,2.0f);
        LegSpeedLimit(1,SpeedMode_EARLYEX);
        LegSpeedLimit(2,SpeedMode_EARLYEX);
        LegSpeedLimit(5,SpeedMode_EARLYEX);
        LegSpeedLimit(6,SpeedMode_EARLYEX);
        SetPolarPositionFB_Delay(Leg_Front,frontleg_squat_angle+delta_angle,stance_height,0);
        /***********************************************************/
        //前腿逐渐进入水平态（核心，与IMU反馈的pitch角配合执行跳跃），后腿迅速卧倒
        while(imu_wait_lock)
        {
            /***********后腿等待倒立的时机************/
            timedelay++;
            if(timedelay == backleg_jump_time/5) //非阻塞延时，约5ms一次。
            {
                LegPID_Set(1,7,0.1f,1.1f,12.0f,0.9f);
                LegPID_Set(3,7,0.1f,1.1f,12.0f,0.9f);
                /******后腿慢慢倒立态（必要，否则会卡住）******/
                LegSpeedLimit(3,SpeedMode_EARLYEX);
                LegSpeedLimit(4,SpeedMode_EARLYEX);
                LegSpeedLimit(7,SpeedMode_EARLYEX);
                LegSpeedLimit(8,SpeedMode_EARLYEX);
                //后腿反向转
                TargetAngle1=-PI;
                TargetAngle2=2 * PI;
                SetCoupledThetaPosition(1);
                SetCoupledThetaPosition(3);
            }
            /***********前腿慢慢水平************/
            LegPID_Set(0,7.5f,0.1f,0.8f,10.0f,0.8f);
            LegPID_Set(2,7.5f,0.1f,0.8f,10.0f,0.8f);
            LegSpeedLimit(1,SpeedMode_EARLYEX);
            LegSpeedLimit(2,SpeedMode_EARLYEX);
            LegSpeedLimit(5,SpeedMode_EARLYEX);
            LegSpeedLimit(6,SpeedMode_EARLYEX);
            SetCartesianPositionFB_Delay(Leg_Front,LegSquatLenth,-0.15f,0);
            //一直到角度合适，然后切换到下一个步态。
            if(SeconTime==0 && IMU_EulerAngle.EulerAngle[Pitch]> (takeoff_inclination-imu_angle_half_range) && IMU_EulerAngle.EulerAngle[Pitch]< (takeoff_inclination+imu_angle_half_range) )
            {
                SeconTime=2;
            }
            else if(SeconTime==2 && IMU_EulerAngle.EulerAngle[Pitch]> (88-imu_fullscale_correction) && IMU_EulerAngle.EulerAngle[Pitch]< (90-imu_fullscale_correction) )//当pitch角度出现系统误差时，可能角度范围达不到这里，因此无法前翻，必须先在imu.c接收中断中修正系统误差。
            {
                SeconTime=1;
            }
            else if(SeconTime==1 && IMU_EulerAngle.EulerAngle[Pitch]> (takeoff_inclination-imu_angle_half_range) && IMU_EulerAngle.EulerAngle[Pitch]< (takeoff_inclination+imu_angle_half_range) )
            {
                imu_wait_lock=0;
                //避免过快翻转导致后腿没来得及卧倒，故这里强制再进行一次卧倒。
                FBLegsPID_Set(Leg_Back,6,0.1f,0.04f,10.0f,0.1f);
                FBLegsSpeedLimit(Leg_Back,SpeedMode_EARLYEX);
                //后腿反向转
                TargetAngle1=-PI;TargetAngle2=2 * PI;
                SetCoupledThetaPosition(1);
                SetCoupledThetaPosition(3);
            }
            osDelay(5);//每次延时5ms，保证其它任务正常执行的同时，也不会有因为延时过高而错过IMU角度，同时也方便进行计数计时。
        }
        /***********************************************************/
        //前腿跳跃
        LegSpeedLimit(1,SpeedMode_VERYEX);
        LegSpeedLimit(2,SpeedMode_VERYEX);
        LegSpeedLimit(5,SpeedMode_VERYEX);
        LegSpeedLimit(6,SpeedMode_VERYEX);
        ChangeAllGainOfPID(8,0.1f,5.5f,50.0f,0.01f);//使用高刚度和低阻尼执行跳转
        SetCartesianPositionFB_Delay(Leg_Front,jump_extension,-0.15f,frontleg_jump_time);

    LegPID_Set(0,7,0.1f,1.1f,12.0f,0.9f);
    LegPID_Set(2,7,0.1f,1.1f,12.0f,0.9f);
    /******后腿慢慢倒立态（必要，否则会卡住）******/
    LegSpeedLimit(1,SpeedMode_EARLYEX);
    LegSpeedLimit(2,SpeedMode_EARLYEX);
    LegSpeedLimit(5,SpeedMode_EARLYEX);
    LegSpeedLimit(6,SpeedMode_EARLYEX);
    //后腿反向转
    TargetAngle1=-PI;
    TargetAngle2=2 * PI;
    SetCoupledThetaPosition(0);
    SetCoupledThetaPosition(2);
        /***********************************************************/
//        //前腿正站立
//        AllLegsSpeedLimit(SpeedMode_EXTREME);
//        ChangeAllGainOfPID(8.0f,0.1f,0.25f,10.0f,0.01f);
////		ChangeAllGainOfPID(5,0.1,0.7,25,2.0);//站立态PID
//        TargetAngle1=0;TargetAngle2=PI;
//        SetCoupledThetaPosition(0);
//        SetCoupledThetaPosition(2);
//        osDelay(300);
//        /***********************************************************/
//        //最后阶段（前、后腿均变为四脚朝天态，但转动的方向相反防止机械卡位）
//        ChangeAllGainOfPID(8.0f,0.1f,0.25f,100,0.1f);//使用低刚度和大量的阻尼来处理下降
//        //后腿反方向（向后）转动
//        ReverseMoveOpen();
//        SetCartesianPositionFB_Delay(Leg_Back,0,-LegStandLenth,0);
//        ReverseMoveClose();
//        //前腿正常（向前）转动
//        SetCartesianPositionFB_Delay(Leg_Front,0,-LegStandLenth,0);
        gpstate = 0;//回到停止态


}
