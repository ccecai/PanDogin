//
// Created by 1 on 2023-11-07.
//
#include "Attitude_Slove.h"

float AngleWant_MotorX[9] = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
float step_angle[4] = {0};

float times = 0.0f;
float x,y;

//float MidPoint = 97.6f;
float MidPoint = 94.0f;

uint8_t reverse_move_flag = 0;

float StepLenthMin = 0.0f;
float StepLenthMax = 0.0f; //��С�����45cm
float Target_Slope = 0.0f;
float Target_Xpos = -0.26f;
float Target_Ypos = 0.0f;

float offset_front_0 = 0.49179f;
float offset_front_1 = 0.989861f;
float offset_back_0 = 0.49179f;//(-121.9f)
float offset_back_1 = 0.974f;//207.2f

float normal_step_left = 0,normal_step_right = 0;

uint8_t Barrier_flag = 0,Radar_control_flag = 0,FrontJump_flag = 0;
//���ڸ����Ϸ�״̬������Ϊ�����׼��
DetachedParam StateDetachedParams_Copy[StatesMaxNum] = {0};
//����ʱ�����ı����ɵĹ켣����
void Change_SinStateDetachedParams(DetachedParam *State,int8_t id,int8_t legid,float stance_height,float step_length,
                                float up_amp,float down_amp,float flight_percent,float freq)
{
    State[id].GaitID = id;

    switch (legid) {
        case 1:
            state_detached_params[1].detached_params_0.stance_height = stance_height;
            state_detached_params[1].detached_params_0.step_length = step_length;
            state_detached_params[1].detached_params_0.up_amp = up_amp;
            state_detached_params[1].detached_params_0.down_amp = down_amp;
            state_detached_params[1].detached_params_0.flight_percent = flight_percent;
            state_detached_params[1].detached_params_0.freq = freq;

            break;
        case 2:
            state_detached_params[1].detached_params_1.stance_height = stance_height;
            state_detached_params[1].detached_params_1.step_length = step_length;
            state_detached_params[1].detached_params_1.up_amp = up_amp;
            state_detached_params[1].detached_params_1.down_amp = down_amp;
            state_detached_params[1].detached_params_1.flight_percent = flight_percent;
            state_detached_params[1].detached_params_1.freq = freq;

            break;
        case 3:
            state_detached_params[1].detached_params_2.stance_height = stance_height;
            state_detached_params[1].detached_params_2.step_length = step_length;
            state_detached_params[1].detached_params_2.up_amp = up_amp;
            state_detached_params[1].detached_params_2.down_amp = down_amp;
            state_detached_params[1].detached_params_2.flight_percent = flight_percent;
            state_detached_params[1].detached_params_2.freq = freq;

            break;
        case 4:
            state_detached_params[1].detached_params_3.stance_height = stance_height;
            state_detached_params[1].detached_params_3.step_length = step_length;
            state_detached_params[1].detached_params_3.up_amp = up_amp;
            state_detached_params[1].detached_params_3.down_amp = down_amp;
            state_detached_params[1].detached_params_3.flight_percent = flight_percent;
            state_detached_params[1].detached_params_3.freq = freq;
            break;
        default:
            break;
    }

}

void SetCoupledThetaPositionAll(void)
{
    SetCoupledThetaPosition(0);
    SetCoupledThetaPosition(1);
    SetCoupledThetaPosition(2);
    SetCoupledThetaPosition(3);
}

void SetCoupledThetaPosition(int LegId)
{
    if(FrontJump_flag == 1 && Barrier_flag == 0)
    {
        switch(LegId) {
            case 0:
                AngleWant_MotorX[1] = TargetAngle1 - offset_front_0;
                AngleWant_MotorX[2] = TargetAngle2 - offset_front_1;
                break;
            case 1:
                AngleWant_MotorX[3] = TargetAngle1 - offset_back_0 - Pitch_Data / 180 * PI;//+5.0f
                AngleWant_MotorX[4] = TargetAngle2 - offset_back_1 + Pitch_Data / 180 * PI;
                break;
            case 2:
                AngleWant_MotorX[5] =-TargetAngle2 + offset_front_1;//-4.0f
                AngleWant_MotorX[6] =-TargetAngle1 + offset_front_0;
                break;
            case 3:
                AngleWant_MotorX[7] =-TargetAngle2 + offset_back_1 - Pitch_Data / 180 * PI;
                AngleWant_MotorX[8] =-TargetAngle1 + offset_back_0 + Pitch_Data / 180 * PI;
                break;
            default:
                break;
        }
    }
    else if(FrontJump_flag == 0 && Barrier_flag == 1)
    {
        switch(LegId) {
            case 0:
                AngleWant_MotorX[1] = TargetAngle1 - offset_front_0 - Pitch_Data / 180 * PI;
                AngleWant_MotorX[2] = TargetAngle2 - offset_front_1 + Pitch_Data / 180 * PI;
                break;
            case 1:
                AngleWant_MotorX[3] = TargetAngle1 - offset_back_0 - Pitch_Data / 180 * PI;//+5.0f
                AngleWant_MotorX[4] = TargetAngle2 - offset_back_1 + Pitch_Data / 180 * PI;
                break;
            case 2:
                AngleWant_MotorX[5] =-TargetAngle2 + offset_front_1 - Pitch_Data / 180 * PI;//-4.0f
                AngleWant_MotorX[6] =-TargetAngle1 + offset_front_0 + Pitch_Data / 180 * PI;
                break;
            case 3:
                AngleWant_MotorX[7] =-TargetAngle2 + offset_back_1 - Pitch_Data/ 180 * PI;
                AngleWant_MotorX[8] =-TargetAngle1 + offset_back_0 + Pitch_Data/ 180 * PI;
                break;
            default:
                break;
        }
    }
    else if(Barrier_flag == 0 && FrontJump_flag == 0)
    {
        switch(LegId) {
            case 0:
                AngleWant_MotorX[1] = TargetAngle1 - offset_front_0;
                AngleWant_MotorX[2] = TargetAngle2 - offset_front_1;
                break;
            case 1:
                AngleWant_MotorX[3] = TargetAngle1 - offset_back_0;//+5.0f
                AngleWant_MotorX[4] = TargetAngle2 - offset_back_1;
                break;
            case 2:
                AngleWant_MotorX[5] = -TargetAngle2 + offset_front_1;
                AngleWant_MotorX[6] = -TargetAngle1 + offset_front_0;
                break;
            case 3:
                AngleWant_MotorX[7] = -TargetAngle2 + offset_back_1;
                AngleWant_MotorX[8] = -TargetAngle1 + offset_back_0;
                break;
            default:
                break;
        }
    }

    //ע��Ƕȸ�ֵ���ݲ�ͬ�ĵ��˳���������ͬ��ͬʱҲ�ܻ�е�����װ��Ӱ�졣���иĶ���������ĽǶȵĶ�Ӧ��ϵҲҪ�䡣
    /*
    �������½Ƕ�������Ϣ�����ǿ��Եõ����½��ۣ�
    01��67�ŵ��(�������һ�£���75��23�ŵ�����������һ������ǰ����������෴���ֱ��Ӧ�����Խ��ߡ�

                                     ͷ
            ��ǰ�� /~**********************************~\��ǰ��
                    theta0 12 theta1  -theta1 56 -theta0
                     leftleg(usart1)	rightleg(usart2)
                   theta0 34 theta1  -theta1 78 -theta0
            ����� \~**********************************~/�Һ���
                                     β

    ����ͼ��ʾ��
        2��3��ǰ���ڲࡢ5��8�Ǻ�����࣬���Ǿ���Ӧtheta0����2��3����ǶԳƵĶ���һ�µģ���˽Ƕȷ���Ҫ����һ�¡�
        �������ͬ����
    */
}
/*
* NAME: void CartesianToThetaGamma(void)
* FUNCTION: �ѿ������꣨���ֱ������ϵ��ת�����Ƕ����꣨����Ƕ����꣩�� Ҳ���ǽ�xyת����theta���˶�ѧ����̣����ĺ�����
* ���������ı�theta0��theta1��ֵ
* ������ڲ�����������ȫ�ֱ�������x��yĿ������
*/
void CartesianToTheta(void)
{
    float L=0,N=0;
    double M=0;
    float A1=0,A2=0;
    //�����ȳ����㼰�ȳ���λ
    L=sqrt(pow(x,2) + pow(y,2));
    if(L<LegLenthMin) L=LegLenthMin;
    else if(L>LegLenthExtremeMax) L=LegLenthExtremeMax;//L�ķ�Χ��L1+-L2�����ΪL1+L2
    //�����ȳ����㡰�м�Ƕȡ�N��M������Ƕȷ�Χ-180��~180�㡣
    N = asin(y / L) * 180.0 / PI;////�Ƕȷ�ΧΪ-90��~90�㡣
    if((x < 0)&&(y > 0)) N = 180 - N;////�Ƕȷ�ΧΪ90��~180��
    else if((x < 0)&&(y < 0)) N =-180-N;////�Ƕȷ�ΧΪ-180��~-90��
    M=acos(	( pow(L,2)+pow(L1,2)-pow(L2,2) )/(2*L1*L) )*180.0/PI;////���ԵĽǶȴ�С���Ƕȷ�ΧΪ0��~90�㡣
    //����ת�����Ȳ�����offset�������ո�ֵ�ٽ��е�������
    A1=M+N-90;
    A2=90-(N-M);
    //����ȷ������Ƕȡ��Ƕȷ�Χ�ֱ�Ϊ0��~360���-360��~0�㡣
    TargetAngle1=-(A1-90);
    TargetAngle2=-(A2-270);

    if(reverse_move_flag == 1)//�˶��������
    {
        TargetAngle1-=360;
        TargetAngle2+=360;
    }

    TargetAngle1 = TargetAngle1 / 180 * 3.1415926535;
    TargetAngle2 = TargetAngle2 / 180 * 3.1415926535;
}
/*
* NAME: SinTrajectory (float t,GaitParams params, float gaitOffset)
* FUNCTION : ���ҹ켣�����������ĺ�����������CoupledMoveLeg�����С�
* ��ڲ�����
			t���������Ʊ�������������ʱ������š��������ϲ��������ʱ�������ʵ�����ǣ�tt=times*5/1000;��ttԼÿ5ms�仯5/1000����0.005��
			GaitParams����̬����
			gaitOffset����λ����ڹ��ɲ�ͬ��̬�ĺ��Ĳ�������
			leg_diretion�������ȵ�ǰ�������
			angle��
* ���Ż�����ʼ��λ�Ŀ��ƱȽ���Ҫ���������𲽵�ʱ���Ƿ�ƽ�ȣ�
*/
void SinTrajectory (float t,GaitParams params, float gaitOffset,float leg_diretion,float angle,int LegId)
{
//tt=times*5/1000����ÿ1s�仯1
    //��ȡ���Һ�������Ҫ���õĲ���
    float stanceHeight = params.stance_height;//��������ظ߶�
    float downAMP = params.down_amp;//����ֵ
    float upAMP = params.up_amp;//����ֵ
    float flightPercent = params.flight_percent;//�ڶ���ռ��
    float stepLength = params.step_length ;//����
    float FREQ = params.freq;//Ƶ��
    if(leg_diretion<0) stepLength = -stepLength;//�������
    //ԭʼ�����ʼ��
    float x0=0,y0=0;
    /******��λ��ʱ�䡢����ѭ��������******/
    static float p = 0,prev_t = 0;//��λʱ���ۼ�(Ҫ��ʵ�ֲ�ͬ�Ȳ�ͬƵ�ʣ��Ͳ��ܹ���һ���������Ӧ�ý����Ϊ�Ȳ���������)��
    //����tÿ�ν��뺯���仯����0.005�����FREQ������ҪС��200������p�ı仯�������ڵ���1���Ӷ������˶�������
    //���統FREQ=1ʱ��ÿ����1s��t�仯1����p�պñ仯1���ʴ�ʱƵ��Ϊ1Hz����FREQ=nʱ��Ƶ����Ȼ��ΪnHz����Ƶ�����Ϊ200Hz��
    //����Ƶ�ʲ�Ҫ������ΪƵ��Խ����ζ�Ų�������Խ�١���ʵ�������ǲ���Ҫ��ô��Ƶ�ʣ�Ӧ��Ƶ��������0-5�����䷶Χ�ڡ�
    p += FREQ * (t - prev_t);//Ƶ��*ʱ��仯����Ϊ��λ�仯����pÿ�α仯��������ʱ���ǹ̶���5ms�������ǿ���ͨ���ı�ÿ�α仯�Ĵ�С����Ӵ���仯Ƶ�ʡ�FREQԽ�󣬵��α仯�ľ�Խ��
    float gp = fmod((p+gaitOffset),1.0);//�ú������� x/y ����������1.0����ȡС�����֣�����gp������0-1��Χ�ڡ�
    prev_t = t;//����ǰtֵ����������
    /******���ҹ켣����******/
    //���ڶ���
    if (gp <= flightPercent) //gp����gaitOffset��ʼ����˵�gaitOffset����flightPercentʱ����ֱ��ת��֧���ࡣ
    {
        x0 = (gp/flightPercent)*stepLength - stepLength/2.0f;//��-stepLength/2��+stepLength/2���ƶ�ʱ�䲻��stepLength�ı䣬��stepLengthԽ��ʵ���ƶ��ٶ�Խ�졣
        y0 = -upAMP*sin(PI*gp/flightPercent) + stanceHeight;//Χ��stanceHeightΪ�����������Ҳ�����ͬ����upAMPԽ���ƶ��ٶ�Խ�졣
    }
        //���֧����
    else //�ڶ����Ǵ����ҹ켣����ʼλ�ô�ִ�С�
    {
        float percentBack = (gp-flightPercent)/(1.0f-flightPercent);//percentBack��(gp/flightPercent)��һ������
        x0 = -percentBack*stepLength + stepLength/2.0f;//һ����˵���״ν���ʱ���Ǵ�stepLength/2��ʼ��Ȼ��֮�������˶���
        y0 = downAMP*sin(PI*percentBack) + stanceHeight;//
    }
    //��������ϵת����õ����ս��(angleĿǰ����0���Ӷ�x=x0��y=y0)
    x =  cos(angle*PI/180)*x0 + sin(angle*PI/180)*y0;
    y = -sin(angle*PI/180)*x0 + cos(angle*PI/180)*y0;

}

/*
* NAME: CoupledMoveLeg
* FUNCTION :�����˶�����Ͽ��ƣ�����gait_detached������
* ��ڲ���:
	GaitParams һֻ�ȵĲ���
	gait_offset ��̬������
	leg_diretion �ȵ�ǰ����1Ϊǰ������-1Ϊ���˷���
	LegId �Ⱥ�
	angle �Ƕ�
*/
void CoupledMoveLeg(float t, GaitParams params,float gait_offset, float leg_direction, int LegId, float angle)
{
    SinTrajectory(t,params,gait_offset,leg_direction,angle,LegId);//������ҹ켣������
    CartesianToTheta();//�ѿ�������ת�����Ƕ�����
    SetCoupledThetaPosition(LegId);//�������ݸ������������
}
/*
* NAME: gait_detached
* FUNCTION : ���ȷ�����Ȳ����ƺ���
* ��ڲ�����
	DetachedParam ��Ҫʵ�ֵĲ�̬��Ϣ���ýṹ�����ÿ���˵Ĳ�̬��Ϣ
	legx_offset   ÿ���ȵ���λ��ʱ����trot��̬2��3����0.5����ʱ����walk��̬��
	legx_direction �ߵķ���
*/
void gait_detached(	DetachedParam d_params,
                       float leg0_offset, float leg1_offset,float leg2_offset, float leg3_offset,
                       float leg0_direction, float leg1_direction,float leg2_direction, float leg3_direction)
{
    float tt=times*5/1000;
    CoupledMoveLeg(tt,d_params.detached_params_0,leg0_offset,leg0_direction,0,step_angle[0]);
    CoupledMoveLeg(tt,d_params.detached_params_1,leg1_offset,leg1_direction,1,step_angle[1]);
    CoupledMoveLeg(tt,d_params.detached_params_2,leg2_offset,leg2_direction,2,step_angle[2]);
    CoupledMoveLeg(tt,d_params.detached_params_3,leg3_offset,leg3_direction,3,step_angle[3]);
}
//�����Ϲ�ʹ��3508�����Ҫ���ļ��ٱ�ת������������ò���
/*
void Output_Angle(void)
{
    for(int i=0;i<8;i++)
    {
        Angle_Output[i] = AngleWant_MotorX[i] / 360 * 8192 * 19;
    }
}
*/
//��Ŀ���ٶ�ֵ
void Get_Target(float theta1,float theta2)
{
    TargetAngle1 = theta1;
    TargetAngle2 = theta2;
}


/*
 * Ϊ�˲�Ӱ������Ķ����������������������ṹ��������������ͨ��Change_SinStateDetachedParams(DetachedParam *State,int8_t id,float stance_height,float step_length,
 * float up_amp,float down_amp,float flight_percent,float freq);
 * �������ض�ʹ�õ����������������������и��ģ��������޸�pid�ĺ���ֱ�ӽ��벽̬�����У�
 * �·�����ĳ�ʼ�����������Ϊһ�����õĴ洢/�ݴ����򣬿���ʡȥgit�Ĳ���
 */
DetachedParam state_detached_params[StatesMaxNum] = {


        {
            0,//ת�䣨��ת�亯���л�����ò�̬��ʵ��ת�䣩
            {19.0f, 5.0f, 6.0f, 1.0f, 0.17f, 3.5f},
            {19.0f, 5.0f, 6.0f, 1.0f, 0.17f, 3.5f},
            {19.0f, 5.0f, 6.0f, 1.0f, 0.17f, 3.5f},// 6����������Ϊstance_height; step_length; up_amp; down_amp; flight_percent; freq
            {19.0f, 5.0f, 6.0f, 1.0f, 0.17f, 3.5f}
        },
        {

                1,//��Trot�����٣�,�������õĲ�̬����
                {18.0f, 20.0f,  8.0f, 0.8f, 0.13f, 3.5f},
                {18.0f, 20.0f,  8.0f, 0.8f, 0.13f, 3.5f},
                {18.0f, 20.0f,  8.0f, 0.8f, 0.13f, 3.5f},
                {18.0f, 20.0f,  8.0f, 0.8f, 0.13f, 3.5f}

        },
        {
            2,//ԭ��̤��//���ֶ��ֲ�̬���߲������ǻ�ʧЧ
            {18.0f, 0.0f,  4.0f, 0.5f, 0.17f, 4.0f},
            {18.0f, 0.0f,  4.0f, 0.5f, 0.17f, 4.0f},
            {18.0f, 0.0f,  4.0f, 0.5f, 0.17f, 4.0f},
            {18.0f, 0.0f,  4.0f, 0.5f, 0.17f, 4.0f}
        },
        {
            3,//Walk��̬��û�е��ã�
            {15.0f, 20.0f,  5.0f, 5.0f, 0.18f, 2.1f},
            {15.0f, 20.0f,  5.0f, 5.0f, 0.18f, 2.1f},
            {15.0f, 20.0f,  5.0f, 5.0f, 0.18f, 2.1f},
            {15.0f, 20.0f,  5.0f, 5.0f, 0.18f, 2.1f}
        },
        {
            4,//˫ľ�����ߵĲ�̬
            {17.0f, 21.0f,  6.0f, 1.0f, 0.15f, 2.9f},
            {17.0f, 21.0f,  6.0f, 1.0f, 0.15f, 2.9f},
            {17.0f, 21.0f,  6.0f, 1.0f, 0.15f, 2.9f},
            {17.0f, 21.0f,  6.0f, 1.0f, 0.15f, 2.9f}
        },
        {

            5,//�ϰ����𲽵Ĳ�̬
            {18.0f, 4.0f,  6.5f, 1.0f, 0.19f, 2.0f},
            {18.0f, 4.0f,  6.5f, 1.0f, 0.19f, 2.0f},
            {18.0f, 4.0f,  6.5f, 1.0f, 0.19f, 2.0f},
            {18.0f, 4.0f,  6.5f, 1.0f, 0.19f, 2.0f}
        },
        {
            6,//��΢
            {14.0f, 0.0f,  0.6f, 0.9f, 0.1f, 3.5f},
            {14.0f, 0.0f,  0.6f, 0.9f, 0.1f, 3.5f},
            {21.0f, 0.0f,  8.0f, 0.9f, 0.1f, 3.5f},
            {21.0f, 0.0f,  8.0f, 0.9f, 0.1f, 3.5f}
        },
        {
            7,//��΢
            {21.0f, 0.0f,  8.0f, 0.9f, 0.1f, 3.5f},
            {21.0f, 0.0f,  8.0f, 0.9f, 0.1f, 3.5f},
            {14.0f, 0.0f,  0.6f, 0.9f, 0.1f, 3.5f},
            {14.0f, 0.0f,  0.6f, 0.9f, 0.1f, 3.5f}
        },
        {
            8,//С��Trot�����٣�
            {18.0f, 13.5f, 6.5f, 0.9f, 0.11f, 2.5f},
            {18.0f, 13.5f, 6.5f, 0.9f, 0.11f, 2.5f},
            {18.0f, 13.5f, 6.5f, 0.9f, 0.11f, 2.5f},
            {18.0f, 13.5f, 6.5f, 0.9f, 0.11f, 2.5f}

        },
        {

            9,//�ϰ����������ߵĲ�̬
            {18.0f, 11.0f,  6.5f, 1.0f, 0.18f, 3.0f},
            {18.0f, 11.0f,  6.5f, 1.0f, 0.18f, 3.0f},
            {18.0f, 11.0f,  6.5f, 1.0f, 0.18f, 3.0f},
            {18.0f, 11.0f,  6.5f, 1.0f, 0.18f, 3.0f}
        },
        {

                10,//�ϰ����������ߵĲ�̬
                {18.0f, 9.0f,  6.0f, 1.0f, 0.2f, 4.5f},
                {18.0f, 9.0f,  6.0f, 1.0f, 0.2f, 4.5f},
                {18.0f, 24.0f,  6.0f, 1.0f, 0.2f, 4.5f},
                {18.0f, 24.0f,  6.0f, 1.0f, 0.2f, 4.5f}
        },
        {

                11,//�ϰ����������ߵĲ�̬
                {18.0f, 5.0f,  4.0f, 1.0f, 0.15f, 3.0f},
                {18.0f, 5.0f,  4.0f, 1.0f, 0.15f, 3.0f},
                {18.0f, 5.0f,  4.0f, 1.0f, 0.15f, 3.0f},
                {18.0f, 5.0f,  4.0f, 1.0f, 0.15f, 3.0f}
        },
        {

                12,//�ϰ����𲽵Ĳ�̬
                {18.0f, 6.0f, 4.0f, 1.0f, 0.19f, 2.5f},
                {18.0f, 6.0f, 4.0f, 1.0f, 0.19f, 2.5f},
                {18.0f, 6.0f, 4.0f, 1.0f, 0.19f, 2.5f},
                {18.0f, 6.0f,4.0f,  1.0f, 0.19f, 2.5f}
        },
        {

                13,//�ϰ����𲽵Ĳ�̬
                {18.0f, 20.0f,  8.0f, 1.7f, 0.11f,3.5f},
                {18.0f, 20.0f,  8.0f, 1.7f, 0.11f,3.5f},
                {18.0f, 20.0f,  8.0f, 1.7f, 0.11f,3.5f},
                {18.0f, 20.0f,  8.0f, 1.7f, 0.11f,3.5f}
        },
};


//�°�IMU���ƻ���һ�����ƵĻ�׼����IMU���ڣ������˸��ַǹ�����ɵ�BUG��
void YawControl(float yaw_set,DetachedParam *State_Detached_Params,int direction)
{
    StepLenthMin = StateDetachedParams_Copy[State_Detached_Params->GaitID].detached_params_0.step_length - 7.0f;
    StepLenthMax = StateDetachedParams_Copy[State_Detached_Params->GaitID].detached_params_0.step_length + 7.0f;

    if(IMU_Control_Flag == 1)
    {
        ChangeYawOfPID(1.7f,0.5f,3000.0f,10.0f);
        /*******IMU��PID���*******/
        //PIDĿ���趨��һ�㶼��0������Pitch��ʱҪ������һ���Ƕȣ�
        SetPoint_IMU(&Yaw_PID_Loop,yaw_set);
        PID_Pos(&Yaw_PID_Loop,Yaw_Data);

        if(direction != 1) Yaw_PID_Loop.Out_put = -Yaw_PID_Loop.Out_put;
        /**********��̬����*********/
        //Yaw�������������
        normal_step_left  = StateDetachedParams_Copy[State_Detached_Params->GaitID].detached_params_0.step_length - Yaw_PID_Loop.Out_put;//���Ȳ�������
        normal_step_right = StateDetachedParams_Copy[State_Detached_Params->GaitID].detached_params_0.step_length + Yaw_PID_Loop.Out_put;//���Ȳ�����С
        //�����޷�
        if(normal_step_right > StepLenthMax)
            normal_step_right = StepLenthMax;
        else if(normal_step_right < StepLenthMin)
            normal_step_right = StepLenthMin;

        if(normal_step_left > StepLenthMax)
            normal_step_left = StepLenthMax;
        else if(normal_step_left < StepLenthMin)
            normal_step_left = StepLenthMin;

        //���ո�ֵ��ǰ��Ĳ����޷���֤�˲������������ں����ķ�Χ�ڶ����������Ӹ����Ͻ���˳���IMU���ƻ���BUG�Ŀ����ԣ�
        State_Detached_Params->detached_params_0.step_length = normal_step_left;
        State_Detached_Params->detached_params_1.step_length = normal_step_left;

        State_Detached_Params->detached_params_2.step_length = normal_step_right;
        State_Detached_Params->detached_params_3.step_length = normal_step_right;

    }

    else if(visual_control_flag == 1 && Race_flag == 1)
    {
        ChangeYawOfPID(0.6f,0.6f,3000.0f,10.0f);
        SetPoint_IMU(&Yaw_PID_Loop,yaw_set);
        PID_Pos(&Yaw_PID_Loop,Yaw_Data);

        ChangePID(&VisualLoop,0.265f,0.0f,4400.0f,25.0f);
        VisualLoop.I = 0.3f;
        SetPoint_Visual(&VisualLoop,MidPoint);
        PID_Pos(&VisualLoop,visual.offset);

        if(direction != 1)
        {
            Yaw_PID_Loop.Out_put = -Yaw_PID_Loop.Out_put;
        }

        normal_step_left  = StateDetachedParams_Copy[State_Detached_Params->GaitID].detached_params_0.step_length + VisualLoop.Out_put;///���Ȳ�������
        normal_step_right = StateDetachedParams_Copy[State_Detached_Params->GaitID].detached_params_0.step_length - VisualLoop.Out_put;//

        //Yaw�������������
//        normal_step_left  = StateDetachedParams_Copy[State_Detached_Params->GaitID].detached_params_0.step_length + VisualLoop.Out_put - Yaw_PID_Loop.Out_put;//���Ȳ�������
//        normal_step_right = StateDetachedParams_Copy[State_Detached_Params->GaitID].detached_params_0.step_length - VisualLoop.Out_put + Yaw_PID_Loop.Out_put;//���Ȳ�����С
        //�����޷�

        if(normal_step_right > StepLenthMax)
            normal_step_right = StepLenthMax;
        else if(normal_step_right < StepLenthMin)
            normal_step_right = StepLenthMin;

        if(normal_step_left > StepLenthMax)
            normal_step_left = StepLenthMax;
        else if(normal_step_left < StepLenthMin)
            normal_step_left = StepLenthMin;

        //���ո�ֵ��ǰ��Ĳ����޷���֤�˲������������ں����ķ�Χ�ڶ����������Ӹ����Ͻ���˳���IMU���ƻ���BUG�Ŀ����ԣ�
        State_Detached_Params->detached_params_0.step_length = normal_step_left;
        State_Detached_Params->detached_params_1.step_length = normal_step_left;

        State_Detached_Params->detached_params_2.step_length = normal_step_right;
        State_Detached_Params->detached_params_3.step_length = normal_step_right;
    }

    else if(visual_control_flag == 1 && BarrierMode_flag == 1)
    {
        ChangePID(&VisualLoop,1.5f,0.7f,4000.0f,15.0f);
        SetPoint_Visual(&VisualLoop,MidPoint );
        PID_Pos(&VisualLoop,visual.offset);

        //Yaw�������������
        normal_step_left  = StateDetachedParams_Copy[State_Detached_Params->GaitID].detached_params_0.step_length + VisualLoop.Out_put;//���Ȳ�������
        normal_step_right = StateDetachedParams_Copy[State_Detached_Params->GaitID].detached_params_0.step_length - VisualLoop.Out_put;//���Ȳ�����С
        //�����޷�

        if(normal_step_right > StepLenthMax)
            normal_step_right = StepLenthMax;
        else if(normal_step_right < StepLenthMin)
            normal_step_right = StepLenthMin;

        if(normal_step_left > StepLenthMax)
            normal_step_left = StepLenthMax;
        else if(normal_step_left < StepLenthMin)
            normal_step_left = StepLenthMin;

        //���ո�ֵ��ǰ��Ĳ����޷���֤�˲������������ں����ķ�Χ�ڶ����������Ӹ����Ͻ���˳���IMU���ƻ���BUG�Ŀ����ԣ�
        State_Detached_Params->detached_params_0.step_length = normal_step_left;
        State_Detached_Params->detached_params_1.step_length = normal_step_left;

        State_Detached_Params->detached_params_2.step_length = normal_step_right;
        State_Detached_Params->detached_params_3.step_length = normal_step_right;
    }

}

//ֱ����������x��yλ�ý��е������
void SetCoupledCartesianPosition(int LegId,float x_want,float y_want)
{
    x=x_want;
    y=y_want;
    CartesianToTheta();
    SetCoupledThetaPosition(LegId);

}

//�����ȵ�ֱ���������
void SetCartesianPositionAll_Delay(float x_want,float y_want,uint16_t delaytime)
{

    SetCoupledCartesianPosition(3,x_want,y_want);
    SetCoupledCartesianPosition(0,x_want,y_want);
    SetCoupledCartesianPosition(1,x_want,y_want);
    SetCoupledCartesianPosition(2,x_want,y_want);

    osDelay(delaytime);
}

void SetPolarPositionLeg_Delay(float polar_angle,float polar_diameter,uint16_t delaytime,uint8_t Legid)
{
    float x_want,y_want;

    if(polar_angle>=0)
    {
        x_want = -polar_diameter*cos(polar_angle*PI/180);
        y_want =  polar_diameter*sin(polar_angle*PI/180);
    }
    else
    {
        x_want =  polar_diameter*cos(polar_angle*PI/180);
        y_want = -polar_diameter*sin(polar_angle*PI/180);
    }
    SetCoupledCartesianPosition(Legid,x_want,y_want);
}

//�����ȵļ��������
void SetPolarPositionAll_Delay(float polar_angle,float polar_diameter,uint16_t delaytime)
{
    float x_want,y_want;

    if(polar_angle>=0)
    {
        x_want = -polar_diameter*cos(polar_angle*PI/180);
        y_want =  polar_diameter*sin(polar_angle*PI/180);
    }
    else
    {
        x_want =  polar_diameter*cos(polar_angle*PI/180);
        y_want = -polar_diameter*sin(polar_angle*PI/180);
    }
    SetCartesianPositionAll_Delay(x_want,y_want,delaytime);
}
//ǰ/���ȵ�ֱ���������
void SetCartesianPositionFB_Delay(int Leg_FB,float x_want,float y_want,uint16_t delaytime)
{
    if(Leg_FB==Leg_Front)
    {
        SetCoupledCartesianPosition(1,x_want,y_want);
        SetCoupledCartesianPosition(3,x_want,y_want);
    }
    else
    {
        SetCoupledCartesianPosition(2,x_want,y_want);
        SetCoupledCartesianPosition(4,x_want,y_want);
    }
    osDelay(delaytime);
}

void ReverseMoveOpen(void)
{
    reverse_move_flag=1;
}
//�ر��˶�����
void ReverseMoveClose(void)
{
    reverse_move_flag=0;
}

void IMU_Slove(uint8_t b_flag,uint8_t f_flag)
{
    if(b_flag == 1)
        Barrier_flag = 1;
    else if(b_flag == 0)
        Barrier_flag = 0;
    if(f_flag == 1)
        FrontJump_flag = 1;
    else if(f_flag == 0)
        FrontJump_flag = 0;
}