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
    if (JumpType == Standard_Jump)//��׼�����Ծ���������涼�нϺõ���Ӧ�ԣ���߸߶Ⱥ�Զ�ȣ�
    {
        /*
         * ˫ľ���õ���
         */
        /*��Ծ���̵�ʱ��ѿأ���ʵ��Ϊ�����ú�ʱ��ʱ�䣬��֤�˶����̷ֶεĺ����ԣ�*/
        const uint16_t prep_time = 800;       //׼��ʱ�䣬��������׼��������ʱ��  [s]  0.4
        const uint16_t launch_time = 250;    //��չ�ȵĳ���ʱ��                  [s]  0.2
        const uint16_t fall_time = 100;      //�ڿ��з����ʱ��                 [s]  0.25�����ʱ��������õ�С�㣩
        const uint16_t strech_time = 1000;  //��ز�����֧�ŵ�ʱ��              [s]  0.3�����ʱ�������ͻ����̽���վ��̬�ˣ�
        /*��Ծ����̬�ѿأ�����ʱ���ɰ�0.1�����������мӼ��������磨LegSquatLenth-0.4����*/
        const float stance_height = LegLenthMin;  //��Ծ֮ǰ�ȵĸ߶�  [cm]��������Ӧ����LegSquatLenth 11.2f�����������Ծʱ����ʹ��LegLenthMin 10.7f
        const float jump_extension = LegLenthMax; //��չ��Ծ��������ȳ���      [cm]��������Ӧ����LegLenthMax 28
        const float jump_flylegheight = LegStandLenth; //����ʱ�ȳ���   [cm]��������Ӧ����LegLenthMax 28��LegStandLenth 18.0f֮�䣬��һ�����е�ֵ��
        const float jump_landlegheight = LegStandLenth; //���ʱ�ȳ���  [cm]��������Ӧ����LegStandLenth
        //�¶ף�׼������������ʱ��Ϊprep_time
        AllLegsSpeedLimit(SpeedMode_EARLYEX);
        ChangeGainOfPID(7.3f, 1.0f, 0, 0);//ʹ�øն�С������������
        SetPolarPositionAll_Delay(JumpAngle, stance_height, prep_time);

        //�ߺ���ɣ����ģ�������ʱ��Ϊlaunch_time
        AllLegsSpeedLimit(29.0f);//�ٶ�����

//        AngleLoop[3].Output_limit = 29.4f;
//        AngleLoop[4].Output_limit = 29.4f;
        AngleLoop[5].Output_limit = 26.5f;
        AngleLoop[6].Output_limit = 26.5f;
//        AngleLoop[7].Output_limit = 28.0f;
//        AngleLoop[8].Output_limit = 28.0f;

        ChangeGainOfPID(50.0f,0.8f,0, 0);//ʹ�øն�С������������0
        SetPolarPositionAll_Delay(JumpAngle, jump_extension, launch_time);

        /*
        �߸նȵ�ʵ�֣�
        pos_kp�ܴ�
                �����������һ���ܴ��ֵ���ٶȻ�����ֵ���ԽǶȲΪ������ٶ�Ŀ��ֵ��3508�ٶ�����8900���ǶȲ��С10�ȼ��㣬��pos_kp��������Ϊ890��
        sp_kp�ܴ�
                �����ڵ�ǰ�ٶ���Ŀ���ٶȲ���Ǻܴ��ʱ��Ҳ������ܴ�ĵ��������ο��ٶȲ��ϴ��ʱ�򡣴Ӷ�����ʹ������ٶȱ��ֽϴ�
                C620�����������ֵ16384����Ӧ20A������ֵ��������������С��ͨ������Ҫ���ٶ�ֵ��
                ���ǵĿ��ƹ�������Ҫ���ٶȴ�8900���ٱ�Ϊ0���ٶ�ֵΪ250ʱ�Ѿ������ˣ�����Ϊ��׼������sp_kpΪ8�Ѿ������ˣ�����ٶȻ���PID�ǱȽ����ʵģ�һ��8�����¡�
        ��ˣ��ܵĿ���Ч����������ʽ������Speed=Current=angle*pos_kp����ˣ�������ϣ�����ٽ�Ŀ��Ƕ�10��ǰ�����ʼ�ձ�������ٶȣ���ô�����ǿ�������ʽ���㣺
        pos_kp = CurrentMAX/angle_Thresh = 8900/10 = 890��10���Ѿ��ǱȽ�С�ˣ�angle_ThreshԽС��Խ���׳������Ӷ����Σ������˲�����С��10�ȡ�
        �߸նȱ���Ϊ��ת����С�ĽǶȶ��ܷ�����������ת�������
        */
        //������̣�Ҳ��������̣��е���̬�����ģ�������ʱ��Ϊfall_time
        AllLegsSpeedLimit(20.0f);
        ChangeGainOfPID(15.0f, 3.0f, 0, 0);//ʹ�øն�С������������
        SetPolarPositionAll_Delay(-25, jump_flylegheight, fall_time);
        /*130
        �͸նȣ�
            ����������ʽ��pos_kp*sp_kp = CurrentMAX/angle_Thresh���͸ն���ζ������Ҫѡ��һ����΢��һ���angle_Thresh���Ӷ����ǿ���С�Ƕ��ڱȽ����װڶ����������һ���Ƕ����������ˡ�
            ��������ѡ��30�ȣ����У�
            pos_kp = CurrentMAX/angle_Thresh = 8900/30 �� 300���ʣ�Ϊ�˿�������pos_kpΪ300��
            �͸նȱ���Ϊ����һ���Ƕȷ�Χ�ڱȽ�����ת�������Ƕ�Խ��Խ�������ﵽ�ٽ���ֵ�ǶȺ��Ѽ���ת�������ҳ���ʱ��Խ��Խ��������Ϊ��Ĭ�ϵ�I����
        */
        //������׼��վ����
        speed_kp = 0.05f;
        ChangeGainOfPID(3.0f,0.01f, 0, 0);//ʹ�øն�С������������
        SetPolarPositionAll_Delay(-80, jump_landlegheight, strech_time);

        speed_kp = 0.22f;
        //���վ���ˣ�ִ��
        gpstate = 1;
    }
    else if(JumpType == High_Jump)//��ԭ�������ߣ��κε��涼�У�
    {

        /*��Ծ���̵�ʱ��ѿأ���ʵ��Ϊ�����ú�ʱ��ʱ�䣬��֤�˶����̷ֶεĺ����ԣ�*/
        const uint16_t prep_time = 800;       //׼��ʱ�䣬��������׼��������ʱ��  [s]  0.4
        const uint16_t launch_time = 200;    //��չ�ȵĳ���ʱ��                  [s]  0.2
        const uint16_t fall_time = 100;      //�ڿ��з����ʱ��                 [s]  0.25�����ʱ��������õ�С�㣩
        const uint16_t strech_time = 100;  //��ز�����֧�ŵ�ʱ��              [s]  0.3�����ʱ�������ͻ����̽���վ��̬�ˣ�
        /*��Ծ����̬�ѿأ�����ʱ���ɰ�0.1�����������мӼ��������磨LegSquatLenth-0.4����*/
        const float stance_height = LegLenthMin;  //��Ծ֮ǰ�ȵĸ߶�  [cm]��������Ӧ����LegSquatLenth 11.2f�����������Ծʱ����ʹ��LegLenthMin 10.7f
        const float jump_extension = LegLenthMax; //��չ��Ծ��������ȳ���      [cm]��������Ӧ����LegLenthMax 28
        const float jump_flylegheight = LegStandLenth; //����ʱ�ȳ���   [cm]��������Ӧ����LegLenthMax 28��LegStandLenth 18.0f֮�䣬��һ�����е�ֵ��
        const float jump_landlegheight = LegStandLenth; //���ʱ�ȳ���  [cm]��������Ӧ����LegStandLenth 18.0f
        //�¶ף�׼������������ʱ��Ϊprep_time
        AllLegsSpeedLimit(SpeedMode_VERYFAST);
        ChangeGainOfPID(10.0f,2.0f,0,0);//ʹ�øն�С������������
        SetPolarPositionAll_Delay(-(JumpAngle + 2),stance_height,prep_time);
        //�ߺ���ɣ����ģ�������ʱ��Ϊlaunch_time
        AllLegsSpeedLimit(28.0f);//�ٶ�����
        ChangeGainOfPID(21.0f,3.0f,0,0);// ʹ�ø߸նȺ͵�����ִ����ת
        SetPolarPositionAll_Delay(-JumpAngle,jump_extension,launch_time);
        //������̣�Ҳ��������̣��е���̬�����ģ�������ʱ��Ϊfall_time
        AllLegsSpeedLimit(SpeedMode_VERYFAST);
        ChangeGainOfPID(10.0f,1.0f,0,0);//ʹ�õ͸նȺʹ����������������½�
        SetPolarPositionAll_Delay(40,jump_flylegheight,fall_time);
        //������׼��վ����
        ChangeGainOfPID(2.0f,0.01f,0.0f,0.0f);//ʹ�õ͸նȺʹ����������������½�
        SetPolarPositionAll_Delay(85,jump_landlegheight,strech_time);
        //���վ���ˣ�ִ�����
        gpstate = HALT;
        IMU_Slove(1,0);
    }
    else if(JumpType == Far_Jump)//������Զ��Ҫ�����Ħ���ϴ�
    {
        /*��Ծ���̵�ʱ��ѿأ���ʵ��Ϊ�����ú�ʱ��ʱ�䣬��֤�˶����̷ֶεĺ����ԣ�*/
        const uint16_t prep_time = 1000;       //׼��ʱ�䣬��������׼��������ʱ��  [s]  0.4
        const uint16_t launch_time = 230;    //��չ�ȵĳ���ʱ��                  [s]  0.2
        const uint16_t fall_time = 220;      //�ڿ��з����ʱ��                 [s]  0.25�����ʱ��������õ�С�㣩
        const uint16_t strech_time = 600;  //��ز�����֧�ŵ�ʱ��              [s]  0.3�����ʱ�������ͻ����̽���վ��̬�ˣ�
        /*��Ծ����̬�ѿأ�����ʱ���ɰ�0.1�����������мӼ��������磨LegSquatLenth-0.4����*/
        const float stance_height = LegLenthMin;  //��Ծ֮ǰ�ȵĸ߶�  [cm]��������Ӧ����LegSquatLenth 11.2f�����������Ծʱ����ʹ��LegLenthMin 10.7f
        const float jump_extension = LegLenthExtremeMax; //��չ��Ծ��������ȳ���      [cm]��������Ӧ����LegLenthMax 28
        const float jump_flylegheight = LegStandLenth; //����ʱ�ȳ���   [cm]��������Ӧ����LegLenthMax 28��LegStandLenth 18.0f֮�䣬��һ�����е�ֵ��
        const float jump_landlegheight = LegStandLenth; //���ʱ�ȳ���  [cm]��������Ӧ����LegStandLenth 18.0f
        //�¶ף�׼������������ʱ��Ϊprep_time
        AllLegsSpeedLimit(SpeedMode_EXTREME);
        ChangeGainOfPID(11.0f,2.0f,0.0f,0.0f);//ʹ�øն�С������������
        SetPolarPositionAll_Delay(JumpAngle ,stance_height,prep_time);
        //�ߺ���ɣ����ģ�������ʱ��Ϊlaunch_time

        speed_kp = 0.27f;
        AllLegsSpeedLimit(35.0f);//�ٶ�����
        ChangeGainOfPID(50.0f,0.0f,0.0f,0.0f);
        SetPolarPositionAll_Delay(JumpAngle,jump_extension,launch_time);

        //������̣�Ҳ��������̣��е���̬�����ģ�������ʱ��Ϊfall_time
        AllLegsSpeedLimit(30.0f);
        ChangeGainOfPID(30.0f,2.0f,0.0f,0.0f);//ʹ�õ͸նȺʹ����������������½�
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
        //������׼��վ����
        speed_kp = 0.2f;

//        speed_kp = 0.06f;
        ChangeGainOfPID(20.0f,0.01f,0.0f,0.0f);//ʹ�õ͸նȺʹ����������������½�
        SetPolarPositionAll_Delay(-92,jump_landlegheight,strech_time);
//        speed_kp = 0.2f;
        //���վ���ˣ�ִ�����
        gpstate = 1;
    }
    else if(JumpType == Leap_Jump)//�ǳ����޵�����ͬʱ����
    {
        /*��Ծ���̵�ʱ��ѿأ���ʵ��Ϊ�����ú�ʱ��ʱ�䣬��֤�˶����̷ֶεĺ����ԣ�*/
        const uint16_t prep_time = 800;       //׼��ʱ�䣬��������׼��������ʱ��  [s]  0.4
        const uint16_t launch_time = 200;    //��չ�ȵĳ���ʱ��                  [s]  0.2
        const uint16_t fall_time = 100;      //�ڿ��з����ʱ��                 [s]  0.25�����ʱ��������õ�С�㣩
        const uint16_t strech_time = 200;  //��ز�����֧�ŵ�ʱ��              [s]  0.3�����ʱ�������ͻ����̽���վ��̬�ˣ�
        /*��Ծ����̬�ѿأ�����ʱ���ɰ�0.1�����������мӼ��������磨LegSquatLenth-0.4����*/
        const float stance_height = LegLenthMin;  //��Ծ֮ǰ�ȵĸ߶�  [cm]��������Ӧ����LegSquatLenth 11.2f�����������Ծʱ����ʹ��LegLenthMin 10.7f
        const float jump_extension = LegLenthMax; //��չ��Ծ��������ȳ���      [cm]��������Ӧ����LegLenthMax 28
        const float jump_flylegheight = LegStandLenth; //����ʱ�ȳ���   [cm]��������Ӧ����LegLenthMax 28��LegStandLenth 18.0f֮�䣬��һ�����е�ֵ��
        const float jump_landlegheight = LegStandLenth; //���ʱ�ȳ���  [cm]��������Ӧ����LegStandLenth 18.0f
        //�¶ף�׼������������ʱ��Ϊprep_time
        AllLegsSpeedLimit(SpeedMode_VERYFAST);
        ChangeGainOfPID(12.0f,2.0f,0,0);//ʹ�øն�С������������
        SetPolarPositionAll_Delay(-(JumpAngle + 2),stance_height,prep_time);
        //�ߺ���ɣ����ģ�������ʱ��Ϊlaunch_time
        AllLegsSpeedLimit(26.0f);//�ٶ�����
        ChangeGainOfPID(15.0f,3.0f,0,0);// ʹ�ø߸նȺ͵�����ִ����ת

        SetPolarPositionAll_Delay(-JumpAngle,jump_extension,launch_time);
        //������̣�Ҳ��������̣��е���̬�����ģ�������ʱ��Ϊfall_time
        AllLegsSpeedLimit(SpeedMode_VERYFAST);
        ChangeGainOfPID(12.0f,0.8f,0,0);//ʹ�õ͸նȺʹ����������������½�
        SetPolarPositionAll_Delay(40,jump_flylegheight,fall_time);
        //������׼��վ����
        speed_kp = 0.06f;

        ChangeGainOfPID(3.0f,0.01f,0.0f,0.0f);//ʹ�õ͸նȺʹ����������������½�
        SetPolarPositionAll_Delay(86,jump_landlegheight,strech_time);

        speed_kp = 0.2f;
        //���վ���ˣ�ִ�����
        gpstate = HALT;
        IMU_Slove(1,0);
    }
}

int FrontJump(void )
{
    /*
         * ˫ľ���õ���
         */
    /*��Ծ���̵�ʱ��ѿأ���ʵ��Ϊ�����ú�ʱ��ʱ�䣬��֤�˶����̷ֶεĺ����ԣ�*/
    const uint16_t prep_time = 1000;       //׼��ʱ�䣬��������׼��������ʱ��  [s]  0.4
    const uint16_t launch_time = 250;    //��չ�ȵĳ���ʱ��                  [s]  0.2
    /*��Ծ����̬�ѿأ�����ʱ���ɰ�0.1�����������мӼ��������磨LegSquatLenth-0.4����*/
    const float stance_height = LegLenthMin;  //��Ծ֮ǰ�ȵĸ߶�  [cm]��������Ӧ����LegSquatLenth 11.2f�����������Ծʱ����ʹ��LegLenthMin 10.7f

    //�¶ף�׼������������ʱ��Ϊprep_time
    AllLegsSpeedLimit(15.0f);
    ChangeGainOfPID(7.5f, 1.0f, 0, 0);//ʹ�øն�С������������
    wait_flag = 1;
    SetPolarPositionLeg_Delay(80.0f, stance_height, prep_time,0);
    SetPolarPositionLeg_Delay(80.0f, stance_height, prep_time,2);
    SetPolarPositionLeg_Delay(85.0f, stance_height, prep_time,1);
    SetPolarPositionLeg_Delay(85.0f, stance_height, prep_time,3);
    osDelay(prep_time);

    //���������ĺ���
    speed_kp = 0.3f;//����˳ʱ����
    AllLegsSpeedLimit(27.5f);//�ڿղ�Ҫ̫�������������ù�����ǰ��
    ChangeGainOfPID(40.0f,2.0f,0, 0);//ʹ�øն�С������������0
    SetPolarPositionLeg_Delay(85.0f, LegLenthExtremeMax, launch_time,1);
    SetPolarPositionLeg_Delay(85.0f, LegLenthExtremeMax, launch_time,3);
    osDelay(300);
    speed_kp = 0.2f;
/**
 * �Ƚ�������������״̬����ֹû�з���ȥ��ʱ�������ֵ���̬
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
 * ���û�з���ȥ�����Է���վ��̬
 */
    while (IMU_EulerAngle.EulerAngle[Pitch] > -85.0f)
    {
        if(gpstate == 1)
            return 0;
    }
/**
 * ����ȥ�Ժ��������׼�����
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
 * ǰ�����׼������
 */
    AllLegsSpeedLimit(40.0f);//�ٶ�����
    ChangeGainOfPID(45.0f,0.8f,0, 0);//ʹ�øն�С������������0
    SetPolarPositionLeg_Delay(80.0f, LegLenthExtremeMax, launch_time,0);
    SetPolarPositionLeg_Delay(80.0f, LegLenthExtremeMax, launch_time,2);

    osDelay(350);

    IMU_Slove(0,0);
/**
 * ��������pid��С�������壬���ҽ�ǰ��׼������
 */
    speed_kp = 0.06f;

    ChangeGainOfPID(20.0f,0.8f,0.03f,0.05f);//��ʼ��pid
    AllLegsSpeedLimit(20.0f);
    Get_Target(PI,0);
    SetCoupledThetaPosition(0);
    SetCoupledThetaPosition(2);

    osDelay(200);
/**
 * ������
 */
    ChangeGainOfPID(10.0f,0.1f,0.03f,0.05f);//��ʼ��pid
    speed_kp = 0.2f;
    //�ȴ�����ִ��
    gpstate = 0;
}

/***
//ǰ�շ���Ծ�������������������ǰ�ȡ�
//����Ծ�в�ͬ��ģʽ����������ǰ�շ�������������˫��ֱ�����ȣ���ԾЧ�������ɹ������������������أ���
//ִ�и���Ծʱ�����ñ�����ʩ���������������ʧ��
***/
void FrontFlipJump(uint8_t mode)
{

        uint8_t SeconTime=0;//�ж��Ƿ��ǵڶ��ε���
        uint32_t timedelay=0;//��������ʱ������Ծ��IMU��ϸ�����
        //IMU�����ض��Ƕȷ�Χ��Ծ�Ŀ��Ʊ���
        uint8_t imu_wait_lock = 1;//IMU����
        float imu_fullscale_correction = 3;//��Ϊ���������̣�90�ȣ���ʵ��������֮�
        float takeoff_inclination = 67-imu_fullscale_correction;//���Ĳ��� ��inclination������
        float imu_angle_half_range = 1.5f;//��������ʹ��
        /*��Ծ���̵�ʱ��ѿأ���ʵ��Ϊ�����ú�ʱ��ʱ�䣬��֤�˶����̷ֶεĺ����ԣ�*/
        const uint16_t prep_time = 850;            //׼��ʱ�䣬��������׼��������ʱ�� [ms]
        const uint16_t backleg_jump_time  = 150;   //�������ĳ���ʱ��				   [ms]
        const uint16_t frontleg_jump_time = 180;   //ǰ�����ĳ���ʱ��                [ms]
        /*��Ծ����̬�ѿ�*/
        const float stance_height = LegLenthMin+1.0f;//��Ծ֮ǰ�ȵĸ߶�  [cm]
        const float jump_extension = LegLenthExtremeMax;//�����LegLenthExtremeMax
        const float backleg_jump_angle  =  81.0f;//������ǰ�Ⱥ�����Ҫ�ɸýǶȲ���������
        const float frontleg_squat_angle = 60.0f;
        const float delta_angle = 10;
        /***********************************************************/
        /*�¶�׼������������ʱ��Ϊprep_time*/
        ChangeGainOfPID(8.0f,1.0f,0,0);//ʹ�øն�С������������
        AllLegsSpeedLimit(SpeedMode_EARLYEX);
        //�����¶�
        SetPolarPositionFB_Delay(Leg_Back,backleg_jump_angle,stance_height,0);
        //ǰ�ȡ���б���¶�
        SetPolarPositionFB_Delay(Leg_Front,frontleg_squat_angle,stance_height,prep_time);
        /***********************************************************/
        /*����������ǰ�����̱仯һ�Σ���������£���ǰ�ȱ仯���ʣ�������ת����֧�㲻������ˣ���Ӧ���������Źؽڣ�������ʱ��Ϊlaunch_time*/
        AllLegsSpeedLimit(SpeedMode_VERYEX);//�ٶ�����
        LegPID_Set(1,8,0.1f,5.5f,40,0.03f);
        LegPID_Set(3,8,0.1f,5.5f,40,0.03f);//ʹ�ø߸նȺ͵�����ִ����ת
        /*********��������**********/
        SetPolarPositionFB_Delay(Leg_Back,backleg_jump_angle,jump_extension,0);
        /*********ǰ����������ǰһ��**********/
        LegPID_Set(0,5,0.1f,0.7f,12,2.0f);//ChangeAllGainOfPID(5,0.1,0.7,25,2.0);//վ��̬PID
        LegPID_Set(2,5,0.1f,0.7f,12,2.0f);
        LegSpeedLimit(1,SpeedMode_EARLYEX);
        LegSpeedLimit(2,SpeedMode_EARLYEX);
        LegSpeedLimit(5,SpeedMode_EARLYEX);
        LegSpeedLimit(6,SpeedMode_EARLYEX);
        SetPolarPositionFB_Delay(Leg_Front,frontleg_squat_angle+delta_angle,stance_height,0);
        /***********************************************************/
        //ǰ���𽥽���ˮƽ̬�����ģ���IMU������pitch�����ִ����Ծ��������Ѹ���Ե�
        while(imu_wait_lock)
        {
            /***********���ȵȴ�������ʱ��************/
            timedelay++;
            if(timedelay == backleg_jump_time/5) //��������ʱ��Լ5msһ�Ρ�
            {
                LegPID_Set(1,7,0.1f,1.1f,12.0f,0.9f);
                LegPID_Set(3,7,0.1f,1.1f,12.0f,0.9f);
                /******������������̬����Ҫ������Ῠס��******/
                LegSpeedLimit(3,SpeedMode_EARLYEX);
                LegSpeedLimit(4,SpeedMode_EARLYEX);
                LegSpeedLimit(7,SpeedMode_EARLYEX);
                LegSpeedLimit(8,SpeedMode_EARLYEX);
                //���ȷ���ת
                TargetAngle1=-PI;
                TargetAngle2=2 * PI;
                SetCoupledThetaPosition(1);
                SetCoupledThetaPosition(3);
            }
            /***********ǰ������ˮƽ************/
            LegPID_Set(0,7.5f,0.1f,0.8f,10.0f,0.8f);
            LegPID_Set(2,7.5f,0.1f,0.8f,10.0f,0.8f);
            LegSpeedLimit(1,SpeedMode_EARLYEX);
            LegSpeedLimit(2,SpeedMode_EARLYEX);
            LegSpeedLimit(5,SpeedMode_EARLYEX);
            LegSpeedLimit(6,SpeedMode_EARLYEX);
            SetCartesianPositionFB_Delay(Leg_Front,LegSquatLenth,-0.15f,0);
            //һֱ���ǶȺ��ʣ�Ȼ���л�����һ����̬��
            if(SeconTime==0 && IMU_EulerAngle.EulerAngle[Pitch]> (takeoff_inclination-imu_angle_half_range) && IMU_EulerAngle.EulerAngle[Pitch]< (takeoff_inclination+imu_angle_half_range) )
            {
                SeconTime=2;
            }
            else if(SeconTime==2 && IMU_EulerAngle.EulerAngle[Pitch]> (88-imu_fullscale_correction) && IMU_EulerAngle.EulerAngle[Pitch]< (90-imu_fullscale_correction) )//��pitch�Ƕȳ���ϵͳ���ʱ�����ܽǶȷ�Χ�ﲻ���������޷�ǰ������������imu.c�����ж�������ϵͳ��
            {
                SeconTime=1;
            }
            else if(SeconTime==1 && IMU_EulerAngle.EulerAngle[Pitch]> (takeoff_inclination-imu_angle_half_range) && IMU_EulerAngle.EulerAngle[Pitch]< (takeoff_inclination+imu_angle_half_range) )
            {
                imu_wait_lock=0;
                //������췭ת���º���û���ü��Ե���������ǿ���ٽ���һ���Ե���
                FBLegsPID_Set(Leg_Back,6,0.1f,0.04f,10.0f,0.1f);
                FBLegsSpeedLimit(Leg_Back,SpeedMode_EARLYEX);
                //���ȷ���ת
                TargetAngle1=-PI;TargetAngle2=2 * PI;
                SetCoupledThetaPosition(1);
                SetCoupledThetaPosition(3);
            }
            osDelay(5);//ÿ����ʱ5ms����֤������������ִ�е�ͬʱ��Ҳ��������Ϊ��ʱ���߶����IMU�Ƕȣ�ͬʱҲ������м�����ʱ��
        }
        /***********************************************************/
        //ǰ����Ծ
        LegSpeedLimit(1,SpeedMode_VERYEX);
        LegSpeedLimit(2,SpeedMode_VERYEX);
        LegSpeedLimit(5,SpeedMode_VERYEX);
        LegSpeedLimit(6,SpeedMode_VERYEX);
        ChangeAllGainOfPID(8,0.1f,5.5f,50.0f,0.01f);//ʹ�ø߸նȺ͵�����ִ����ת
        SetCartesianPositionFB_Delay(Leg_Front,jump_extension,-0.15f,frontleg_jump_time);

    LegPID_Set(0,7,0.1f,1.1f,12.0f,0.9f);
    LegPID_Set(2,7,0.1f,1.1f,12.0f,0.9f);
    /******������������̬����Ҫ������Ῠס��******/
    LegSpeedLimit(1,SpeedMode_EARLYEX);
    LegSpeedLimit(2,SpeedMode_EARLYEX);
    LegSpeedLimit(5,SpeedMode_EARLYEX);
    LegSpeedLimit(6,SpeedMode_EARLYEX);
    //���ȷ���ת
    TargetAngle1=-PI;
    TargetAngle2=2 * PI;
    SetCoupledThetaPosition(0);
    SetCoupledThetaPosition(2);
        /***********************************************************/
//        //ǰ����վ��
//        AllLegsSpeedLimit(SpeedMode_EXTREME);
//        ChangeAllGainOfPID(8.0f,0.1f,0.25f,10.0f,0.01f);
////		ChangeAllGainOfPID(5,0.1,0.7,25,2.0);//վ��̬PID
//        TargetAngle1=0;TargetAngle2=PI;
//        SetCoupledThetaPosition(0);
//        SetCoupledThetaPosition(2);
//        osDelay(300);
//        /***********************************************************/
//        //���׶Σ�ǰ�����Ⱦ���Ϊ�Ľų���̬����ת���ķ����෴��ֹ��е��λ��
//        ChangeAllGainOfPID(8.0f,0.1f,0.25f,100,0.1f);//ʹ�õ͸նȺʹ����������������½�
//        //���ȷ��������ת��
//        ReverseMoveOpen();
//        SetCartesianPositionFB_Delay(Leg_Back,0,-LegStandLenth,0);
//        ReverseMoveClose();
//        //ǰ����������ǰ��ת��
//        SetCartesianPositionFB_Delay(Leg_Front,0,-LegStandLenth,0);
        gpstate = 0;//�ص�ֹ̬ͣ


}
