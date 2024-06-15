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
    if (JumpType == Bridge_Jump)//��׼�����Ծ���������涼�нϺõ���Ӧ�ԣ���߸߶Ⱥ�Զ�ȣ�
    {
        /***
         * ˫ľ���õ���
         */
        speed_kp = 0.24f;
        /*��Ծ���̵�ʱ��ѿأ���ʵ��Ϊ�����ú�ʱ��ʱ�䣬��֤�˶����̷ֶεĺ����ԣ�*/
        const uint16_t prep_time = 800;       //׼��ʱ�䣬��������׼��������ʱ��  [s]  0.4
        const uint16_t launch_time = 260;    //��չ�ȵĳ���ʱ��                  [s]  0.2
        const uint16_t fall_time = 100;      //�ڿ��з����ʱ��                 [s]  0.25�����ʱ��������õ�С�㣩
        const uint16_t strech_time = 700;  //��ز�����֧�ŵ�ʱ��              [s]  0.3�����ʱ�������ͻ����̽���վ��̬�ˣ�
        /*��Ծ����̬�ѿأ�����ʱ���ɰ�0.1�����������мӼ��������磨LegSquatLenth-0.4����*/
        const float stance_height = LegLenthMin;  //��Ծ֮ǰ�ȵĸ߶�  [cm]��������Ӧ����LegSquatLenth 11.2f�����������Ծʱ����ʹ��LegLenthMin 10.7f
        const float jump_extension = LegLenthExtremeMax; //��չ��Ծ��������ȳ���      [cm]��������Ӧ����LegLenthMax 28
        const float jump_flylegheight = LegStandLenth; //����ʱ�ȳ���   [cm]��������Ӧ����LegLenthMax 28��LegStandLenth 18.0f֮�䣬��һ�����е�ֵ��
        const float jump_landlegheight = LegStandLenth; //���ʱ�ȳ���  [cm]��������Ӧ����LegStandLenth
        //�¶ף�׼������������ʱ��Ϊprep_time
        AllLegsSpeedLimit(SpeedMode_EARLYEX);
        ChangeGainOfPID(7.3f, 1.0f, 0, 0);//ʹ�øն�С������������
        SetPolarPositionAll_Delay(JumpAngle, stance_height, prep_time);
        //�ߺ���ɣ����ģ�������ʱ��Ϊlaunch_time
        AllLegsSpeedLimit(30.0f);//�ٶ�����
//        AngleLoop[5].Output_limit = 28.0f;
//        AngleLoop[6].Output_limit = 28.0f;
        ChangeGainOfPID(50.0f,0.8f,0, 0);//ʹ�øն�С������������0
        SetPolarPositionAll_Delay(JumpAngle, jump_extension, launch_time);

        //������̣�Ҳ��������̣��е���̬�����ģ�������ʱ��Ϊfall_time
        AllLegsSpeedLimit(30.0f);
        ChangeGainOfPID(20.0f, 3.0f, 0, 0);//ʹ�øն�С������������
        SetPolarPositionAll_Delay(-34.0f, stance_height, fall_time);
        speed_kp = 0.06f;

        //������׼��վ����
        ChangeGainOfPID(15.0f,2.0f, 0, 0);//ʹ�øն�С������������
        SetPolarPositionAll_Delay(-80, jump_landlegheight, strech_time);
        //���վ���ˣ�ִ��
        IMU_Slove(1,0);
        gpstate = 1;
        speed_kp = 0.14f;
    }
    else if(JumpType == StepUp_Jump)
    {
        /**
         * ��̨���õ���
         */
        speed_kp = 0.24f;
//        Control_Flag(0,0);//ѡ���Ƿ������������Ӿ���ƫ����

        IMU_Slove(1,0);//�Ƿ����ϰ�ʱ��ʱ�̱�����ֱ
        /*��Ծ���̵�ʱ��ѿأ���ʵ��Ϊ�����ú�ʱ��ʱ�䣬��֤�˶����̷ֶεĺ����ԣ�*/
        const uint16_t prep_time = 800;       //׼��ʱ�䣬��������׼��������ʱ��  [s]  0.4
        const uint16_t launch_time = 190;    //��չ�ȵĳ���ʱ��                  [s]  0.2
        const uint16_t fall_time = 200;      //�ڿ��з����ʱ��                 [s]  0.25�����ʱ��������õ�С�㣩
        const uint16_t strech_time = 400;  //��ز�����֧�ŵ�ʱ��              [s]  0.3�����ʱ�������ͻ����̽���վ��̬�ˣ�
        /*��Ծ����̬�ѿأ�����ʱ���ɰ�0.1�����������мӼ��������磨LegSquatLenth-0.4����*/
        const float stance_height = LegLenthMin;  //��Ծ֮ǰ�ȵĸ߶�  [cm]��������Ӧ����LegSquatLenth 11.2f�����������Ծʱ����ʹ��LegLenthMin 10.7f
        const float jump_extension = LegLenthExtremeMax; //��չ��Ծ��������ȳ���      [cm]��������Ӧ����LegLenthMax 28
        const float jump_flylegheight = LegStandLenth; //����ʱ�ȳ���   [cm]��������Ӧ����LegLenthMax 28��LegStandLenth 18.0f֮�䣬��һ�����е�ֵ��
        const float jump_landlegheight = LegStandLenth; //���ʱ�ȳ���  [cm]��������Ӧ����LegStandLenth
        //�¶ף�׼������������ʱ��Ϊprep_time
        AllLegsSpeedLimit(SpeedMode_EARLYEX);
        ChangeGainOfPID(7.3f, 1.0f, 0, 0);//ʹ�øն�С������������
        SetPolarPositionAll_Delay(JumpAngle, stance_height, prep_time);


        //�ߺ���ɣ����ģ�������ʱ��Ϊlaunch_time
        AllLegsSpeedLimit(26.7f);//�ٶ���

        ChangeGainOfPID(40.0f,0.8f,0, 0);//ʹ�øն�С������������0
        SetPolarPositionAll_Delay(JumpAngle, jump_extension, launch_time);
        speed_kp = 0.14f;

        //������̣�Ҳ��������̣��е���̬�����ģ�������ʱ��Ϊfall_time
        AllLegsSpeedLimit(20.0f);
        ChangeGainOfPID(15.0f, 3.0f, 0, 0);//ʹ�øն�С������������
        SetPolarPositionAll_Delay(-25, jump_flylegheight, fall_time);

        //������׼��վ����
        speed_kp = 0.1f;
        ChangeGainOfPID(8.0f,0.1f, 0, 0);//ʹ�øն�С������������
        SetPolarPositionAll_Delay(73, jump_landlegheight, strech_time);

        speed_kp = 0.14f;
        //���վ���ˣ�ִ��
        gpstate = 1;
    }
    else if(JumpType == StepDown_Jump)//������Զ��Ҫ�����Ħ���ϴ�
    {
        /**
         * ��̨���õ���
         */
        speed_kp = 0.24f;
//        Control_Flag(0,0);//ѡ���Ƿ������������Ӿ���ƫ����

        IMU_Slove(1,0);//�Ƿ����ϰ�ʱ��ʱ�̱�����ֱ
        /*��Ծ���̵�ʱ��ѿأ���ʵ��Ϊ�����ú�ʱ��ʱ�䣬��֤�˶����̷ֶεĺ����ԣ�*/
        const uint16_t prep_time = 800;       //׼��ʱ�䣬��������׼��������ʱ��  [s]  0.4
        const uint16_t launch_time = 190;    //��չ�ȵĳ���ʱ��                  [s]  0.2
        const uint16_t fall_time = 200;      //�ڿ��з����ʱ��                 [s]  0.25�����ʱ��������õ�С�㣩
        const uint16_t strech_time = 300;  //��ز�����֧�ŵ�ʱ��              [s]  0.3�����ʱ�������ͻ����̽���վ��̬�ˣ�
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
        AllLegsSpeedLimit(22.0f);//�ٶ���

        ChangeGainOfPID(24.0f,0.8f,0, 0);//ʹ�øն�С������������0
        SetPolarPositionAll_Delay(JumpAngle, jump_extension, launch_time);

        //������̣�Ҳ��������̣��е���̬�����ģ�������ʱ��Ϊfall_time
        AllLegsSpeedLimit(20.0f);
        ChangeGainOfPID(15.0f, 3.0f, 0, 0);//ʹ�øն�С������������
        SetPolarPositionAll_Delay(-25, jump_flylegheight, fall_time);

        //������׼��վ����
        speed_kp = 0.1f;
        ChangeGainOfPID(8.0f,0.1f, 0, 0);//ʹ�øն�С������������
        SetPolarPositionAll_Delay(-87, jump_landlegheight, strech_time);

        speed_kp = 0.14f;
        //���վ���ˣ�ִ��
        gpstate = 1;
    }
}
/**
 * ǰ�շ��ĺ���
 * @return
 */
int FrontJump(void )
{

//    Control_Flag(0,0);//ѡ���Ƿ������������Ӿ���ƫ����
    speed_kp = 0.24f;
    IMU_Slove(0,1);//�Ƿ����ϰ�ʱ��ʱ�̱�����ֱ
    /*��Ծ���̵�ʱ��ѿأ���ʵ��Ϊ�����ú�ʱ��ʱ�䣬��֤�˶����̷ֶεĺ����ԣ�*/
    const uint16_t prep_time = 1000;       //׼��ʱ�䣬��������׼��������ʱ��  [s]  0.4
    const uint16_t launch_time = 250;    //��չ�ȵĳ���ʱ��                  [s]  0.2
    /*��Ծ����̬�ѿأ�����ʱ���ɰ�0.1�����������мӼ��������磨LegSquatLenth-0.4����*/
    const float stance_height = LegLenthMin;  //��Ծ֮ǰ�ȵĸ߶�  [cm]��������Ӧ����LegSquatLenth 11.2f�����������Ծʱ����ʹ��LegLenthMin 10.7f

    //�¶ף�׼������������ʱ��Ϊprep_time
    AllLegsSpeedLimit(15.0f);
    ChangeGainOfPID(7.5f, 1.0f, 0, 0);//ʹ�øն�С������������
    wait_flag = 1;
    SetPolarPositionLeg_Delay(77.0f, stance_height, prep_time,0);
    SetPolarPositionLeg_Delay(77.0f, stance_height, prep_time,2);
    SetPolarPositionLeg_Delay(85.0f, stance_height, prep_time,1);
    SetPolarPositionLeg_Delay(85.0f, stance_height, prep_time,3);
    osDelay(prep_time);

    //���������ĺ���
    speed_kp = 0.24f;//����˳ʱ����
    AllLegsSpeedLimit(27.0f);//�ڿղ�Ҫ̫�������������ù�����ǰ��
    ChangeGainOfPID(40.0f,2.0f,0, 0);//ʹ�øն�С������������0
    SetPolarPositionLeg_Delay(85.0f, LegLenthExtremeMax, launch_time,1);
    SetPolarPositionLeg_Delay(85.0f, LegLenthExtremeMax, launch_time,3);
    osDelay(300);
    speed_kp = 0.2f;
/**
 * �Ƚ�������������״̬����ֹû�з���ȥ��ʱ�������ֵ���̬
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
 * ���û�з���ȥ�����Է���վ��̬
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
    speed_kp = 0.25f;
/**
 * ǰ�����׼������
 */
    AllLegsSpeedLimit(40.0f);//�ٶ�����
    ChangeGainOfPID(45.0f,0.8f,0, 0);//ʹ�øն�С������������0
    SetPolarPositionLeg_Delay(77.0f, LegLenthExtremeMax, launch_time,0);
    SetPolarPositionLeg_Delay(77.0f, LegLenthExtremeMax, launch_time,2);

    osDelay(180);
/**
 * ���ȹ�����
 */
    SetPolarPositionLeg_Delay(77.0f, LegLenthMin, 0,0);
    SetPolarPositionLeg_Delay(77.0f, LegLenthMin, 0,2);

    osDelay(150);

    IMU_Slove(0,0);
/**
 * ��������pid��С�������壬���ҽ�ǰ��׼������
 */

    ChangeGainOfPID(20.0f,0.8f,0.03f,0.05f);//��ʼ��pid
    AllLegsSpeedLimit(20.0f);
//���ϵ��λ����Ϊ��׼���ı�ƫ��������ɿշ��������
    offset_front_0 -= PI;
    offset_front_1 += PI;
    offset_back_0 += PI;
    offset_back_1 -= PI;
//�շ����ǰ��������
    SetPolarPositionLeg_Delay(-35.0f, 15.5f, 0,1);
    SetPolarPositionLeg_Delay(-35.0f, 15.5f, 0,3);

    Get_Target(0,PI);
    SetCoupledThetaPosition(0);
    SetCoupledThetaPosition(2);

    osDelay(500);
/**
 * ������
 */
    speed_kp = 0.14f;
    ChangeGainOfPID(15.0f,1.0f,0.03f,0.05f);//��ʼ��pid


    //�ȴ�����ִ��
    gpstate = 0;
}

void Turn_Jump(int16_t Jump_angle)
{
    /*��Ծ���̵�ʱ��ѿأ���ʵ��Ϊ�����ú�ʱ��ʱ�䣬��֤�˶����̷ֶεĺ����ԣ�*/
    const uint16_t prep_time = 300;       //׼��ʱ�䣬��������׼��������ʱ��  [s]  0.4
    const uint16_t launch_time = 200;    //��չ�ȵĳ���ʱ��                  [s]  0.2
    const uint16_t fall_time = 100;      //�ڿ��з����ʱ��                 [s]  0.25�����ʱ��������õ�С�㣩
    const uint16_t strech_time = 500;  //��ز�����֧�ŵ�ʱ��              [s]  0.3�����ʱ�������ͻ����̽���վ��̬�ˣ�
    /*��Ծ����̬�ѿأ�����ʱ���ɰ�0.1�����������мӼ��������磨LegSquatLenth-0.4����*/
    const float stance_height = LegLenthMin;  //��Ծ֮ǰ�ȵĸ߶�  [cm]��������Ӧ����LegSquatLenth 11.2f�����������Ծʱ����ʹ��LegLenthMin 10.7f
    const float jump_extension = LegLenthMax; //��չ��Ծ��������ȳ���      [cm]��������Ӧ����LegLenthMax 28
    const float jump_flylegheight = LegStandLenth; //����ʱ�ȳ���   [cm]��������Ӧ����LegLenthMax 28��LegStandLenth 18.0f֮�䣬��һ�����е�ֵ��
    const float jump_landlegheight = LegStandLenth; //���ʱ�ȳ���  [cm]��������Ӧ����LegStandLenth
    switch (Jump_angle) {
        case 45:
            speed_kp = 0.24f;

            //�¶ף�׼������������ʱ��Ϊprep_time
            AllLegsSpeedLimit(SpeedMode_EARLYEX);
            ChangeGainOfPID(10.0f, 1.0f, 0, 0);//ʹ�øն�С������������
            SetPolarPositionLeg_Delay(70.0f, TrunJump_Length, 0,0);
            SetPolarPositionLeg_Delay(70.0f, TrunJump_Length, 0,1);
            SetPolarPositionLeg_Delay(-70.0f, TrunJump_Length, 0,2);
            SetPolarPositionLeg_Delay(-70.0f, TrunJump_Length, 0,3);

            osDelay(prep_time);
            //�ߺ���ɣ����ģ�������ʱ��Ϊlaunch_time
            AllLegsSpeedLimit(15.4f);//�ٶ�����
            ChangeGainOfPID(35.0f,1.5f,0, 0);//ʹ�øն�С������������0
            SetPolarPositionLeg_Delay(70.0f, jump_extension, 0,0);
            SetPolarPositionLeg_Delay(70.0f, jump_extension, 0,1);
            SetPolarPositionLeg_Delay(-70.0f, jump_extension, 0,2);
            SetPolarPositionLeg_Delay(-70.0f, jump_extension, 0,3);

            osDelay(launch_time);

            //������̣�Ҳ��������̣��е���̬�����ģ�������ʱ��Ϊfall_time
            AllLegsSpeedLimit(30.0f);
            ChangeGainOfPID(20.0f, 3.0f, 0, 0);//ʹ�øն�С������������
            SetPolarPositionLeg_Delay(-25.0f, TrunJump_Length, 0,0);
            SetPolarPositionLeg_Delay(-25.0f, TrunJump_Length, 0,1);
            SetPolarPositionLeg_Delay(25.0f, TrunJump_Length, 0,2);
            SetPolarPositionLeg_Delay(25.0f, TrunJump_Length, 0,3);

            osDelay(fall_time);
            speed_kp = 0.06f;

            //������׼��վ����
            ChangeGainOfPID(15.0f,2.0f, 0, 0);//ʹ�øն�С������������
            StandUp_Posture();
            osDelay(strech_time);
//���վ���ˣ�ִ��

//            gpstate = 1;
            speed_kp = 0.15f;
            break;
        case -45:
            speed_kp = 0.28f;
            //�¶ף�׼������������ʱ��Ϊprep_time
            AllLegsSpeedLimit(SpeedMode_EARLYEX);
            ChangeGainOfPID(10.0f, 1.0f, 0, 0);//ʹ�øն�С������������
            SetPolarPositionLeg_Delay(-70.0f, TrunJump_Length, 0,0);
            SetPolarPositionLeg_Delay(-70.0f, TrunJump_Length, 0,1);
            SetPolarPositionLeg_Delay(70.0f, TrunJump_Length, 0,2);
            SetPolarPositionLeg_Delay(70.0f, TrunJump_Length, 0,3);

            osDelay(prep_time);
            //�ߺ���ɣ����ģ�������ʱ��Ϊlaunch_time
            AllLegsSpeedLimit(15.4f);//�ٶ�����
            ChangeGainOfPID(35.0f,1.5f,0, 0);//ʹ�øն�С������������0
            SetPolarPositionLeg_Delay(-70.0f, jump_extension, 0,0);
            SetPolarPositionLeg_Delay(-70.0f, jump_extension, 0,1);
            SetPolarPositionLeg_Delay(70.0f, jump_extension, 0,2);
            SetPolarPositionLeg_Delay(70.0f, jump_extension, 0,3);

            osDelay(launch_time);

            //������̣�Ҳ��������̣��е���̬�����ģ�������ʱ��Ϊfall_time
            AllLegsSpeedLimit(30.0f);
            ChangeGainOfPID(20.0f, 3.0f, 0, 0);//ʹ�øն�С������������
            SetPolarPositionLeg_Delay(25.0f, TrunJump_Length, 0,0);
            SetPolarPositionLeg_Delay(25.0f, TrunJump_Length, 0,1);
            SetPolarPositionLeg_Delay(-25.0f, TrunJump_Length, 0,2);
            SetPolarPositionLeg_Delay(-25.0f, TrunJump_Length, 0,3);

            osDelay(fall_time);
            speed_kp = 0.06f;

            //������׼��վ����
            ChangeGainOfPID(15.0f,2.0f, 0, 0);//ʹ�øն�С������������
            StandUp_Posture();
            osDelay(strech_time);
            //���վ���ˣ�ִ��

//            gpstate = 1;
            speed_kp = 0.15f;
            break;
        default:
            break;
    }
}
