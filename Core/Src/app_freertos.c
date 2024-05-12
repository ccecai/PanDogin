/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "fdcan.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId StartTaskHandle;
osThreadId BlueteethTaskHandle;
osThreadId GO1Init_TaskHandle;
osThreadId VisualHandle;
osThreadId NRFTaskHandle;
osThreadId TripodHeadHandle;
osThreadId GO_Output_LeftHandle;
osThreadId PIDHandle;
osThreadId GO_Output_RightHandle;
osMessageQId VisialHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDebug(void const * argument);
void BlueTeeth_RemoteControl(void const * argument);
void GO1Init(void const * argument);
void VisualTask(void const * argument);
void NRF(void const * argument);
void TripodHeadTask(void const * argument);
void GO_Output_LeftTask(void const * argument);
void PIDTask(void const * argument);
void GO_Output_RightTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of Visial */
  osMessageQDef(Visial, 1, uint8_t);
  VisialHandle = osMessageCreate(osMessageQ(Visial), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of StartTask */
  osThreadDef(StartTask, StartDebug, osPriorityLow, 0, 128);
  StartTaskHandle = osThreadCreate(osThread(StartTask), NULL);

  /* definition and creation of BlueteethTask */
  osThreadDef(BlueteethTask, BlueTeeth_RemoteControl, osPriorityRealtime, 0, 512);
  BlueteethTaskHandle = osThreadCreate(osThread(BlueteethTask), NULL);

  /* definition and creation of GO1Init_Task */
  osThreadDef(GO1Init_Task, GO1Init, osPriorityLow, 0, 128);
  GO1Init_TaskHandle = osThreadCreate(osThread(GO1Init_Task), NULL);

  /* definition and creation of Visual */
  osThreadDef(Visual, VisualTask, osPriorityHigh, 0, 512);
  VisualHandle = osThreadCreate(osThread(Visual), NULL);

  /* definition and creation of NRFTask */
  osThreadDef(NRFTask, NRF, osPriorityLow, 0, 512);
  NRFTaskHandle = osThreadCreate(osThread(NRFTask), NULL);

  /* definition and creation of TripodHead */
  osThreadDef(TripodHead, TripodHeadTask, osPriorityNormal, 0, 256);
  TripodHeadHandle = osThreadCreate(osThread(TripodHead), NULL);

  /* definition and creation of GO_Output_Left */
  osThreadDef(GO_Output_Left, GO_Output_LeftTask, osPriorityHigh, 0, 512);
  GO_Output_LeftHandle = osThreadCreate(osThread(GO_Output_Left), NULL);

  /* definition and creation of PID */
  osThreadDef(PID, PIDTask, osPriorityNormal, 0, 512);
  PIDHandle = osThreadCreate(osThread(PID), NULL);

  /* definition and creation of GO_Output_Right */
  osThreadDef(GO_Output_Right, GO_Output_RightTask, osPriorityHigh, 0, 512);
  GO_Output_RightHandle = osThreadCreate(osThread(GO_Output_Right), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
    vTaskResume(StartTaskHandle);
    vTaskSuspend(TripodHeadHandle);
    vTaskSuspend(GO1Init_TaskHandle);
    vTaskSuspend(BlueteethTaskHandle);
    vTaskSuspend(GO_Output_LeftHandle);
    vTaskSuspend(GO_Output_RightHandle);
    vTaskSuspend(PIDHandle);
    vTaskSuspend(VisualHandle);
    vTaskSuspend(NRFTaskHandle);
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDebug */
/**
  * @brief  Function implementing the StartTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDebug */
void StartDebug(void const * argument)
{
  /* USER CODE BEGIN StartDebug */
    Myinit();
    RemoteControl_Init(1,0); //选择要使用的远程控制模式
    Control_Flag(0,0);//选择是否开启陀螺仪与视觉纠偏开关
    IMU_Slove(0);//是否开启障碍时腿时刻保持竖直

    printf("Init_Ready\n");
    osDelay(3);

    osDelay(1000); //在调试的时候延迟3秒用来打开急停开关

    vTaskResume(GO1Init_TaskHandle);

  /* Infinite loop */
  for(;;)
  {
      LED1_Flash;
      LED2_Flash;

      osDelay(500);
  }

  /* USER CODE END StartDebug */
}

/* USER CODE BEGIN Header_BlueTeeth_RemoteControl */
/**
* @brief Function implementing the BlueteethTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_BlueTeeth_RemoteControl */
void BlueTeeth_RemoteControl(void const * argument)
{
  /* USER CODE BEGIN BlueTeeth_RemoteControl */
  /* Infinite loop */
  for(;;)
  {
      Remote_Controller();
//      usart_printf("%f,%f,%f,%f\n",(((end_pos[1] - began_pos[1])*2*pi)/(6.33f*32768)),(((end_pos[2] - began_pos[2])*2*pi)/(6.33f*32768)),
//                   (((end_pos[5] - began_pos[5])*2*pi)/(6.33f*32768)),(((end_pos[6] - began_pos[6])*2*pi)/(6.33f*32768)));
//      usart_printf("%d,%d,%d,%d\n",real_speed[1],real_speed[2],real_speed[3],real_speed[4]);
//      usart_printf("%d,%d\n",gpstate,dpstate);
//      usart_printf("%f,%f,%f,%f,%f,%f\n",IMU_EulerAngle.EulerAngle[Yaw],visual.offset,state_detached_params[1].detached_params_0.step_length,
//                   state_detached_params[1].detached_params_0.freq,state_detached_params[1].detached_params_2.step_length,state_detached_params[1].detached_params_2.freq);
      //usart_printf("%f,%f\n",visual.distance,visual.offset);
//      usart_printf("%f,%f,%f.%f\n", AngleLoop[1].Out_put,AngleLoop[2].Out_put,AngleLoop[3].Out_put,AngleLoop[4].Out_put);
//      usart_printf("%f,%f,%d,%f,%f,%d,%f,%f\n",IMU_EulerAngle.EulerAngle[Yaw],Yaw_PID_Loop.Out_put,Race_count,visual.distance,visual.offset,gpstate,x,y);
//      usart_printf("%f,%f,%f,%f,%f,%f\n",Yaw_PID_Loop.Setpoint,IMU_EulerAngle.EulerAngle[Yaw],Yaw_PID_Loop.Out_put,state_detached_params[1].detached_params_0.step_length,state_detached_params[1].detached_params_2.step_length,visual.offset);
//      usart_printf("%f,%f,%f\n", IMU_EulerAngle.EulerAngle[Yaw],IMU_EulerAngle.EulerAngle[Pitch],IMU_EulerAngle.EulerAngle[Roll]);
//      osDelay(10);
  }
  /* USER CODE END BlueTeeth_RemoteControl */
}

/* USER CODE BEGIN Header_GO1Init */
/**
* @brief Function implementing the GO1Init_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_GO1Init */
void GO1Init(void const * argument)
{
  /* USER CODE BEGIN GO1Init */
    MOTOR_Send_Init(); //初始化电机发送帧头
    Eight_PID_Init();//八个电机PID结构体初始化
    ChangeGainOfPID(6.0f,1.0f,0.03f,0.05f);//初始化pid

    Get_motor_began_pos();       //获得各个电机的初始位
    EndPosture();                //锁住电机

    for (int i = 1; i < 9; ++i) {
        B_pos[i] = end_pos[i];
    }

    visual.offset = 100;

    PID_Init(&Yaw_PID_Loop);
    ChangeYawOfPID(0.2f,0.1f,4000.0f,15.0f);//陀螺仪PID初始化

    PID_Init(&Roll_PID_Loop);
    Roll_PID_Loop.P = 0.2f;
    Roll_PID_Loop.D = 0.02f;
    Roll_PID_Loop.SumError = 4000.0f;
    Roll_PID_Loop.Output_limit = 15.0f;

    PID_Init(&VisualLoop);
    VisualLoop.P = 0.12f;
    VisualLoop.D = 0.04f;
    VisualLoop.SumError = 4000.0f;
    VisualLoop.Output_limit = 15.0f;

    printf("GO1 Init Ready\n");
    osDelay(3);

    vTaskResume(GO_Output_LeftHandle);
    vTaskResume(GO_Output_RightHandle);
    vTaskResume(PIDHandle);
    vTaskResume(BlueteethTaskHandle);
//    vTaskResume(NRFTaskHandle);
    vTaskResume(VisualHandle);
    vTaskResume(TripodHeadHandle);
    vTaskSuspend(NULL); //电机初始化任务完成后自挂捏
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END GO1Init */
}

/* USER CODE BEGIN Header_VisualTask */
/**
* @brief Function implemaenting the Visual thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_VisualTask */
void VisualTask(void const * argument)
{
  /* USER CODE BEGIN VisualTask */

  /* Infinite loop */
  for(;;)
  {
//        visual_process();
//      usart_printf("%d,%d,%f,%f\n",visual.data_8[1],visual.data_8[5],IMU_EulerAngle.EulerAngle[Pitch],IMU_EulerAngle.EulerAngle[Yaw]
//              );

//      if (Jump_flag == 1)
//      {
//          AngleLoop[1].Output_limit = 15;
//          AngleLoop[2].Output_limit = 15;
//          AngleLoop[5].Output_limit = 15;
//          AngleLoop[6].Output_limit = 15;
//          PID_Set_KP_KI_KD(&AngleLoop[1],5.0f,0,0.1f);
//          PID_Set_KP_KI_KD(&AngleLoop[2],5.0f,0,0.1f);
//          PID_Set_KP_KI_KD(&AngleLoop[5],5.0f,0,0.1f);
//          PID_Set_KP_KI_KD(&AngleLoop[6],5.0f,0,0.1f);
//
//          Get_Target(0,PI);
//          SetCoupledThetaPosition(0);
//          SetCoupledThetaPosition(2);
////          SetPolarPositionLeg_Delay(76.0f, LegLenthMin, 0,0);
////          SetPolarPositionLeg_Delay(76.0f, LegLenthMin, 0,2);
//      }

      osDelay(1);
  }
  /* USER CODE END VisualTask */
}

/* USER CODE BEGIN Header_NRF */
/**
* @brief Function implementing the NRFTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_NRF */
void NRF(void const * argument)
{
  /* USER CODE BEGIN NRF */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END NRF */
}

/* USER CODE BEGIN Header_TripodHeadTask */
/**
* @brief Function implementing the TripodHead thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_TripodHeadTask */
void TripodHeadTask(void const * argument)
{
  /* USER CODE BEGIN TripodHeadTask */
    PID_Init(&M2006_Speed);
    PID_Init(&M2006_Position);

    PID_Set_KP_KI_KD(&M2006_Speed,5.0f,0.05f,0.0f);//2006电机速度环初始化
    PID_Set_KP_KI_KD(&M2006_Position,0.4f,0.0f,0.9f);//2006电机位置环初始化

    M2006_Speed.Output_limit = 4000;
    M2006_Position.Output_limit = 10000;
  /* Infinite loop */
  for(;;)
  {
      SetPoint_IMU(&Roll_PID_Loop,0);
      PID_PosLocM2006(&Roll_PID_Loop,IMU_EulerAngle.EulerAngle[Roll]);

      SetPoint_IMU(&M2006_Position, AngleChange(TargetAngle));
      PID_PosLocM2006(&M2006_Position,struct_debug1[0].total_angle);

      SetPoint_IMU(&M2006_Speed, M2006_Position.Out_put);
      PID_PosLocM2006(&M2006_Speed,struct_debug1[0].speed);

      set_current(&hfdcan1,0x200,M2006_Speed.Out_put,0,0,0);

    osDelay(5);
  }
  /* USER CODE END TripodHeadTask */
}

/* USER CODE BEGIN Header_GO_Output_LeftTask */
/**
* @brief Function implementing the GO_Output_Left thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_GO_Output_LeftTask */
void GO_Output_LeftTask(void const * argument)
{
  /* USER CODE BEGIN GO_Output_LeftTask */
  /* Infinite loop */
  for(;;)
  {
      leg_pos_controll02();
  }
  /* USER CODE END GO_Output_LeftTask */
}

/* USER CODE BEGIN Header_PIDTask */
/**
* @brief Function implementing the PID thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_PIDTask */
void PIDTask(void const * argument)
{
  /* USER CODE BEGIN PIDTask */
  /* Infinite loop */
  for(;;)
  {
      for (int i = 1; i < 9; ++i)
      {
          SetPoint(&AngleLoop[i], AngleWant_MotorX[i], i);
          PID_PosLocCalc(&AngleLoop[i], end_pos[i]);
      }

    osDelay(8);
  }
  /* USER CODE END PIDTask */
}

/* USER CODE BEGIN Header_GO_Output_RightTask */
/**
* @brief Function implementing the GO_Output_Right thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_GO_Output_RightTask */
void GO_Output_RightTask(void const * argument)
{
  /* USER CODE BEGIN GO_Output_RightTask */
  /* Infinite loop */
  for(;;)
  {
      leg_pos_controll();
  }
  /* USER CODE END GO_Output_RightTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

