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
        /*��Ծ���̵�ʱ��ѿأ���ʵ��Ϊ�����ú�ʱ��ʱ�䣬��֤�˶����̷ֶεĺ����ԣ�*/
        const uint16_t prep_time = 800;       //׼��ʱ�䣬��������׼��������ʱ��  [s]  0.4
        const uint16_t launch_time = 260;    //��չ�ȵĳ���ʱ��                  [s]  0.2
        const uint16_t fall_time = 150;      //�ڿ��з����ʱ��                 [s]  0.25�����ʱ��������õ�С�㣩
        const uint16_t strech_time = 400;  //��ز�����֧�ŵ�ʱ��              [s]  0.3�����ʱ�������ͻ����̽���վ��̬�ˣ�
        /*��Ծ����̬�ѿأ�����ʱ���ɰ�0.1�����������мӼ��������磨LegSquatLenth-0.4����*/
        const float stance_height = LegLenthMin;  //��Ծ֮ǰ�ȵĸ߶�  [cm]��������Ӧ����LegSquatLenth 11.2f�����������Ծʱ����ʹ��LegLenthMin 10.7f
        const float jump_extension = LegLenthExtremeMax; //��չ��Ծ��������ȳ���      [cm]��������Ӧ����LegLenthMax 28
        const float jump_flylegheight = LegStandLenth; //����ʱ�ȳ���   [cm]��������Ӧ����LegLenthMax 28��LegStandLenth 18.0f֮�䣬��һ�����е�ֵ��
        const float jump_landlegheight = LegStandLenth; //���ʱ�ȳ���  [cm]��������Ӧ����LegStandLenth
        //�¶ף�׼������������ʱ��Ϊprep_time
        AllLegsSpeedLimit(SpeedMode_EARLYEX);
        ChangeGainOfPID(6.0f, 0.5f, 0, 0);//ʹ�øն�С������������
        SetPolarPositionAll_Delay(JumpAngle, stance_height, prep_time);
        //�ߺ���ɣ����ģ�������ʱ��Ϊlaunch_time

        AllLegsSpeedLimit(30.0f);//�ٶ�����
        AngleLoop[5].Output_limit = 29.0f;
        AngleLoop[6].Output_limit = 29.0f;
        AngleLoop[7].Output_limit = 29.0f;
        AngleLoop[8].Output_limit = 29.0f;
        ChangeGainOfPID(50.0f,2.0f,0, 0);//ʹ�øն�С������������0
        SetPolarPositionAll_Delay(JumpAngle, jump_extension, launch_time);

        //������̣�Ҳ��������̣��е���̬�����ģ�������ʱ��Ϊfall_time
        AllLegsSpeedLimit(30.0f);
        ChangeGainOfPID(20.0f, 3.0f, 0, 0);//ʹ�øն�С������������
        SetPolarPositionAll_Delay(-34.0f, stance_height, fall_time);

        //������׼��վ����
        ChangeGainOfPID(5.0f,0.5f, 0, 0);//ʹ�øն�С������������
        SetPolarPositionAll_Delay(-70, jump_landlegheight, strech_time);
        //���վ���ˣ�ִ��

        StandUp_Posture();
        osDelay(500);

        gpstate = 1;
    }
    else if(JumpType == StepUp_Jump)
    {
        /**
         * ��̨���õ���
         */

        /*��Ծ���̵�ʱ��ѿأ���ʵ��Ϊ�����ú�ʱ��ʱ�䣬��֤�˶����̷ֶεĺ����ԣ�*/
        const uint16_t prep_time = 800;       //׼��ʱ�䣬��������׼��������ʱ��  [s]  0.4
        const uint16_t launch_time = 100;    //��չ�ȵĳ���ʱ��                  [s]  0.2
        const uint16_t fall_time = 100;      //�ڿ��з����ʱ��                 [s]  0.25�����ʱ��������õ�С�㣩
        const uint16_t strech_time = 400;  //��ز�����֧�ŵ�ʱ��              [s]  0.3�����ʱ�������ͻ����̽���վ��̬�ˣ�
        /*��Ծ����̬�ѿأ�����ʱ���ɰ�0.1�����������мӼ��������磨LegSquatLenth-0.4����*/
        const float stance_height = LegLenthMin;  //��Ծ֮ǰ�ȵĸ߶�  [cm]��������Ӧ����LegSquatLenth 11.2f�����������Ծʱ����ʹ��LegLenthMin 10.7f
        const float jump_extension = LegLenthExtremeMax; //��չ��Ծ��������ȳ���      [cm]��������Ӧ����LegLenthMax 28
        const float jump_flylegheight = LegStandLenth + 2.0f; //����ʱ�ȳ���   [cm]��������Ӧ����LegLenthMax 28��LegStandLenth 18.0f֮�䣬��һ�����е�ֵ��
        const float jump_landlegheight = LegStandLenth; //���ʱ�ȳ���  [cm]��������Ӧ����LegStandLenth
        //�¶ף�׼������������ʱ��Ϊprep_time
        AllLegsSpeedLimit(SpeedMode_EARLYEX);
        ChangeGainOfPID(7.3f, 1.0f, 0, 0);//ʹ�øն�С������������
        SetPolarPositionAll_Delay(JumpAngle, stance_height, prep_time);

        //�ߺ���ɣ����ģ�������ʱ��Ϊlaunch_time
        AllLegsSpeedLimit(25.0f);//�ٶ���
//        AngleLoop[5].Output_limit = 27.2f;
//        AngleLoop[6].Output_limit = 27.2f;
//        AngleLoop[7].Output_limit = 27.2f;
//        AngleLoop[8].Output_limit = 27.2f;

        ChangeGainOfPID(50.0f,3.0f,0, 0);//ʹ�øն�С������������0
        SetPolarPositionAll_Delay(JumpAngle, jump_extension, launch_time);

        //������̣�Ҳ��������̣��е���̬�����ģ�������ʱ��Ϊfall_time
        AllLegsSpeedLimit(20.0f);
        ChangeGainOfPID(15.0f, 3.0f, 0, 0);//ʹ�øն�С������������
        SetPolarPositionAll_Delay(-80, jump_flylegheight, fall_time);

        //������׼��վ����
        StandUp_Posture();
        osDelay(300);

        gpstate = 1;
    }
    else if(JumpType == StepDown_Jump)//������Զ��Ҫ�����Ħ���ϴ�
    {
        /**
         * ��̨���õ���
         */

        /*��Ծ���̵�ʱ��ѿأ���ʵ��Ϊ�����ú�ʱ��ʱ�䣬��֤�˶����̷ֶεĺ����ԣ�*/
        const uint16_t prep_time = 800;       //׼��ʱ�䣬��������׼��������ʱ��  [s]  0.4
        const uint16_t launch_time = 190;    //��չ�ȵĳ���ʱ��                  [s]  0.2
        const uint16_t fall_time = 200;      //�ڿ��з����ʱ��                 [s]  0.25�����ʱ��������õ�С�㣩
        const uint16_t strech_time = 400;  //��ز�����֧�ŵ�ʱ��              [s]  0.3�����ʱ�������ͻ����̽���վ��̬�ˣ�
        /*��Ծ����̬�ѿأ�����ʱ���ɰ�0.1�����������мӼ��������磨LegSquatLenth-0.4����*/
        const float stance_height = LegLenthMin;  //��Ծ֮ǰ�ȵĸ߶�  [cm]��������Ӧ����LegSquatLenth 11.2f�����������Ծʱ����ʹ��LegLenthMin 10.7f
        const float jump_extension = LegLenthExtremeMax; //��չ��Ծ��������ȳ���      [cm]��������Ӧ����LegLenthMax 28
        const float jump_flylegheight =LegStandLenth - 2.0f; //����ʱ�ȳ���   [cm]��������Ӧ����LegLenthMax 28��LegStandLenth 18.0f֮�䣬��һ�����е�ֵ��
        const float jump_landlegheight = LegStandLenth; //���ʱ�ȳ���  [cm]��������Ӧ����LegStandLenth
        //�¶ף�׼������������ʱ��Ϊprep_time
        AllLegsSpeedLimit(SpeedMode_EARLYEX);
        ChangeGainOfPID(7.3f, 1.0f, 0, 0);//ʹ�øն�С������������
        SetPolarPositionAll_Delay(JumpAngle, stance_height, prep_time);

        //�ߺ���ɣ����ģ�������ʱ��Ϊlaunch_time
        AllLegsSpeedLimit(27.5f);//�ٶ���
//        AngleLoop[5].Output_limit = 26.0f;
//        AngleLoop[6].Output_limit = 26.0f;
//        AngleLoop[7].Output_limit = 25.9f;
//        AngleLoop[8].Output_limit = 25.9f;

        ChangeGainOfPID(50.0f, 3.0f, 0, 0);//ʹ�øն�С������������0
        SetPolarPositionAll_Delay(JumpAngle, jump_extension, launch_time);

        //������̣�Ҳ��������̣��е���̬�����ģ�������ʱ��Ϊfall_time
        AllLegsSpeedLimit(20.0f);
        ChangeGainOfPID(5.0f, 0.01f, 0, 0);//ʹ�øն�С������������
        SetPolarPositionAll_Delay(-50, jump_flylegheight, fall_time);

        //������׼��վ����
        StandUp_Posture_Slow();
        osDelay(800);

        gpstate = 1;
    }

    else if(JumpType == StepUp_LowJump)
    {
        /**
         * ��̨���õ���
         */

        /*��Ծ���̵�ʱ��ѿأ���ʵ��Ϊ�����ú�ʱ��ʱ�䣬��֤�˶����̷ֶεĺ����ԣ�*/
        const uint16_t prep_time = 800;       //׼��ʱ�䣬��������׼��������ʱ��  [s]  0.4
        const uint16_t launch_time = 190;    //��չ�ȵĳ���ʱ��                  [s]  0.2
        const uint16_t fall_time = 750;      //�ڿ��з����ʱ��                 [s]  0.25�����ʱ��������õ�С�㣩
        const uint16_t strech_time = 400;  //��ز�����֧�ŵ�ʱ��              [s]  0.3�����ʱ�������ͻ����̽���վ��̬�ˣ�
        /*��Ծ����̬�ѿأ�����ʱ���ɰ�0.1�����������мӼ��������磨LegSquatLenth-0.4����*/
        const float stance_height = LegLenthMin;  //��Ծ֮ǰ�ȵĸ߶�  [cm]��������Ӧ����LegSquatLenth 11.2f�����������Ծʱ����ʹ��LegLenthMin 10.7f
        const float jump_extension = LegLenthExtremeMax; //��չ��Ծ��������ȳ���      [cm]��������Ӧ����LegLenthMax 28
        const float jump_flylegheight = LegStandLenth + 2.0f; //����ʱ�ȳ���   [cm]��������Ӧ����LegLenthMax 28��LegStandLenth 18.0f֮�䣬��һ�����е�ֵ��
        const float jump_landlegheight = LegStandLenth; //���ʱ�ȳ���  [cm]��������Ӧ����LegStandLenth
        //�¶ף�׼������������ʱ��Ϊprep_time
        AllLegsSpeedLimit(SpeedMode_EARLYEX);
        ChangeGainOfPID(7.3f, 1.0f, 0, 0);//ʹ�øն�С������������
        SetPolarPositionAll_Delay(JumpAngle, stance_height, prep_time);

        //�ߺ���ɣ����ģ�������ʱ��Ϊlaunch_time
        AllLegsSpeedLimit(28.7f);//�ٶ���
        AngleLoop[5].Output_limit = 27.7f;
        AngleLoop[6].Output_limit = 27.7f;
        AngleLoop[7].Output_limit = 27.7f;
        AngleLoop[8].Output_limit = 27.7f;

        ChangeGainOfPID(50.0f,3.0f,0, 0);//ʹ�øն�С������������0
        SetPolarPositionAll_Delay(JumpAngle, jump_extension, launch_time);

        //������׼��վ����
        StandUp_Posture_Slow();
        osDelay(1100);

        gpstate = 1;
    }

}
/**
 * ǰ�շ��ĺ���
 * @return
 */
int FrontJump(void )
{
    IMU_Slove(1,0);

    /*��Ծ���̵�ʱ��ѿأ���ʵ��Ϊ�����ú�ʱ��ʱ�䣬��֤�˶����̷ֶεĺ����ԣ�*/
    const uint16_t prep_time = 1000;       //׼��ʱ�䣬��������׼��������ʱ��  [s]  0.4
    const uint16_t launch_time = 250;    //��չ�ȵĳ���ʱ��                  [s]  0.2
    /*��Ծ����̬�ѿأ�����ʱ���ɰ�0.1�����������мӼ��������磨LegSquatLenth-0.4����*/
    const float stance_height = LegLenthMin;  //��Ծ֮ǰ�ȵĸ߶�  [cm]��������Ӧ����LegSquatLenth 11.2f�����������Ծʱ����ʹ��LegLenthMin 10.7f
    //�¶ף�׼������������ʱ��Ϊprep_time

    AllLegsSpeedLimit(15.0f);
    ChangeGainOfPID(7.5f, 1.0f, 0, 0);//ʹ�øն�С������������
    SetPolarPositionLeg_Delay(62.0f, 13.2f, prep_time,0);
    SetPolarPositionLeg_Delay(62.0f, 13.2f, prep_time,2);
    SetPolarPositionLeg_Delay(Leg_Back_Angle, 13.2f, prep_time,1);
    SetPolarPositionLeg_Delay(Leg_Back_Angle, 13.2f, prep_time,3);
    osDelay(prep_time);

    //ǰ�������ĺ���
    AllLegsSpeedLimit(30.0f);
    ChangeGainOfPID(40.0f, 3.0f, 0, 0);//ʹ�øն�С������������
    wait_flag = 1;
    AngleLoop[1].Output_limit = 24.3f;
    AngleLoop[2].Output_limit = 24.3f;
    AngleLoop[5].Output_limit = 24.0f;
    AngleLoop[6].Output_limit = 24.0f;
    SetPolarPositionLeg_Delay(62.0f, LegLenthExtremeMax, launch_time,0);
    SetPolarPositionLeg_Delay(62.0f, LegLenthExtremeMax, launch_time,2);
    SetPolarPositionLeg_Delay(Leg_Back_Angle - 20, 13.2f, prep_time,1);
    SetPolarPositionLeg_Delay(Leg_Back_Angle - 20, 13.2f, prep_time,3);
    osDelay(185);
    wait_flag = 0;
//    AngleLoop[5].Output_limit = 28.5f;
//    AngleLoop[6].Output_limit = 28.5f;
    AngleLoop[7].Output_limit = 28.0f;
    AngleLoop[8].Output_limit = 28.0f;



    SetPolarPositionLeg_Delay(-36.0f, 13.2f, launch_time,0);
    SetPolarPositionLeg_Delay(-36.0f, 13.2f, launch_time,2);
    SetPolarPositionLeg_Delay(Leg_Back_Angle - 20, LegLenthExtremeMax, launch_time,1);
    SetPolarPositionLeg_Delay(Leg_Back_Angle - 20, LegLenthExtremeMax, launch_time,3);
    osDelay(270);
/**
 * ���ȹ�����
 */
    SetPolarPositionLeg_Delay(-58.0f, 6.0f, 0,1);
    SetPolarPositionLeg_Delay(-58.0f, 6.0f, 0,3);

    osDelay(320);

/**
 * ��������pid��С�������壬���ҽ�ǰ��׼������
 */

    ChangeGainOfPID(5.0f,0.4f,0.03f,0.05f);//��ʼ��pid
    AllLegsSpeedLimit(20.0f);

    Get_Target(0,PI);
    SetCoupledThetaPosition(1);
    SetCoupledThetaPosition(3);

    osDelay(500);

    IMU_Slove(0,0);
    //�ȴ�����ִ��
    StandUp_Posture();
//    gpstate = 0;
    osDelay(500);

    gpstate = 1;
}

void Turn_Jump(int16_t Jump_angle)
{
    /*��Ծ���̵�ʱ��ѿأ���ʵ��Ϊ�����ú�ʱ��ʱ�䣬��֤�˶����̷ֶεĺ����ԣ�*/
    const uint16_t prep_time = 300;       //׼��ʱ�䣬��������׼��������ʱ��  [s]  0.4
    const uint16_t launch_time = 200;    //��չ�ȵĳ���ʱ��                  [s]  0.2
    const uint16_t fall_time = 100;      //�ڿ��з����ʱ��                 [s]  0.25�����ʱ��������õ�С�㣩
    const uint16_t strech_time = 800;  //��ز�����֧�ŵ�ʱ��              [s]  0.3�����ʱ�������ͻ����̽���վ��̬�ˣ�
    /*��Ծ����̬�ѿأ�����ʱ���ɰ�0.1�����������мӼ��������磨LegSquatLenth-0.4����*/
    const float stance_height = LegLenthMin;  //��Ծ֮ǰ�ȵĸ߶�  [cm]��������Ӧ����LegSquatLenth 11.2f�����������Ծʱ����ʹ��LegLenthMin 10.7f
    const float jump_extension = LegLenthMax; //��չ��Ծ��������ȳ���      [cm]��������Ӧ����LegLenthMax 28
    const float jump_flylegheight = LegStandLenth; //����ʱ�ȳ���   [cm]��������Ӧ����LegLenthMax 28��LegStandLenth 18.0f֮�䣬��һ�����е�ֵ��
    const float jump_landlegheight = LegStandLenth; //���ʱ�ȳ���  [cm]��������Ӧ����LegStandLenth
    switch (Jump_angle) {
        case -90:
            //�¶ף�׼������������ʱ��Ϊprep_time
            AllLegsSpeedLimit(SpeedMode_EARLYEX);
            ChangeGainOfPID(10.0f, 1.0f, 0, 0);//ʹ�øն�С������������
            SetPolarPositionLeg_Delay(-70.0f, TrunJump_Length, 0,0);
            SetPolarPositionLeg_Delay(-70.0f, TrunJump_Length, 0,1);
            SetPolarPositionLeg_Delay(70.0f, TrunJump_Length, 0,2);
            SetPolarPositionLeg_Delay(70.0f, TrunJump_Length, 0,3);

            osDelay(prep_time);
            //�ߺ���ɣ����ģ�������ʱ��Ϊlaunch_time
            AllLegsSpeedLimit(20.0f);//�ٶ�����
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

            //������׼��վ����
            ChangeGainOfPID(15.0f,2.0f, 0, 0);//ʹ�øն�С������������
            StandUp_Posture();
            osDelay(strech_time);
            //���վ���ˣ�ִ��
            break;
        case 90:
            //�¶ף�׼������������ʱ��Ϊprep_time
            AllLegsSpeedLimit(SpeedMode_EARLYEX);
            ChangeGainOfPID(10.0f, 1.0f, 0, 0);//ʹ�øն�С������������
            SetPolarPositionLeg_Delay(70.0f, TrunJump_Length, 0,0);
            SetPolarPositionLeg_Delay(70.0f, TrunJump_Length, 0,1);
            SetPolarPositionLeg_Delay(-70.0f, TrunJump_Length, 0,2);
            SetPolarPositionLeg_Delay(-70.0f, TrunJump_Length, 0,3);

            osDelay(prep_time);
            //�ߺ���ɣ����ģ�������ʱ��Ϊlaunch_time
            AllLegsSpeedLimit(20.0f);//�ٶ�����
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

            //������׼��վ����
            ChangeGainOfPID(15.0f,2.0f, 0, 0);//ʹ�øն�С������������
            StandUp_Posture();
            osDelay(strech_time);
            break;
        case 45:
            //�¶ף�׼������������ʱ��Ϊprep_time
            AllLegsSpeedLimit(SpeedMode_EARLYEX);
            ChangeGainOfPID(10.0f, 1.0f, 0, 0);//ʹ�øն�С������������
            SetPolarPositionLeg_Delay(70.0f, TrunJump_Length, 0,0);
            SetPolarPositionLeg_Delay(70.0f, TrunJump_Length, 0,1);
            SetPolarPositionLeg_Delay(-70.0f, TrunJump_Length, 0,2);
            SetPolarPositionLeg_Delay(-70.0f, TrunJump_Length, 0,3);

            osDelay(prep_time);
            //�ߺ���ɣ����ģ�������ʱ��Ϊlaunch_time
            AllLegsSpeedLimit(24.0f);//�ٶ�����
            ChangeGainOfPID(35.0f,3.0f,0, 0);//ʹ�øն�С������������0
            SetPolarPositionLeg_Delay(70.0f, jump_extension, 0,0);
            SetPolarPositionLeg_Delay(70.0f, jump_extension, 0,1);
            SetPolarPositionLeg_Delay(-70.0f, jump_extension, 0,2);
            SetPolarPositionLeg_Delay(-70.0f, jump_extension, 0,3);

            osDelay(launch_time);

            //������̣�Ҳ��������̣��е���̬�����ģ�������ʱ��Ϊfall_time
            AllLegsSpeedLimit(25.0f);
            ChangeGainOfPID(20.0f, 3.0f, 0, 0);//ʹ�øն�С������������
            SetPolarPositionLeg_Delay(-25.0f, TrunJump_Length, 0,0);
            SetPolarPositionLeg_Delay(-25.0f, TrunJump_Length, 0,1);
            SetPolarPositionLeg_Delay(25.0f, TrunJump_Length, 0,2);
            SetPolarPositionLeg_Delay(25.0f, TrunJump_Length, 0,3);

            osDelay(fall_time);

            //������׼��վ����
            ChangeGainOfPID(15.0f,2.0f, 0, 0);//ʹ�øն�С������������
            MarkingTime();
            osDelay(strech_time);
            break;

        case -45:
            //�¶ף�׼������������ʱ��Ϊprep_time
            AllLegsSpeedLimit(SpeedMode_EARLYEX);
            ChangeGainOfPID(10.0f, 1.0f, 0, 0);//ʹ�øն�С������������
            SetPolarPositionLeg_Delay(-70.0f, TrunJump_Length, 0,0);
            SetPolarPositionLeg_Delay(-70.0f, TrunJump_Length, 0,1);
            SetPolarPositionLeg_Delay(70.0f, TrunJump_Length, 0,2);
            SetPolarPositionLeg_Delay(70.0f, TrunJump_Length, 0,3);

            osDelay(prep_time);
            //�ߺ���ɣ����ģ�������ʱ��Ϊlaunch_time
            AllLegsSpeedLimit(24.0f);//�ٶ�����
            ChangeGainOfPID(35.0f,3.0f,0, 0);//ʹ�øն�С������������0
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

            //������׼��վ����
            ChangeGainOfPID(15.0f,2.0f, 0, 0);//ʹ�øն�С������������
            MarkingTime();
            osDelay(strech_time);
            //���վ���ˣ�ִ��
            break;
        default:
            break;
    }

}
