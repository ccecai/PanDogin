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
#include "Subordinate_Desk.h"
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
osThreadId FrontJumpHandle;
osThreadId NRFTaskHandle;
osThreadId TripodHeadHandle;
osThreadId GO_Output_LeftHandle;
osThreadId PIDHandle;
osThreadId GO_Output_RightHandle;
osThreadId DebugHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDebug(void const * argument);
void BlueTeeth_RemoteControl(void const * argument);
void GO1Init(void const * argument);
void FrontJumpTask(void const * argument);
void NRF(void const * argument);
void TripodHeadTask(void const * argument);
void GO_Output_LeftTask(void const * argument);
void PIDTask(void const * argument);
void GO_Output_RightTask(void const * argument);
void DebugTask(void const * argument);

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

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of StartTask */
  osThreadDef(StartTask, StartDebug, osPriorityLow, 0, 256);
  StartTaskHandle = osThreadCreate(osThread(StartTask), NULL);

  /* definition and creation of BlueteethTask */
  osThreadDef(BlueteethTask, BlueTeeth_RemoteControl, osPriorityRealtime, 0, 3072);
  BlueteethTaskHandle = osThreadCreate(osThread(BlueteethTask), NULL);

  /* definition and creation of GO1Init_Task */
  osThreadDef(GO1Init_Task, GO1Init, osPriorityLow, 0, 256);
  GO1Init_TaskHandle = osThreadCreate(osThread(GO1Init_Task), NULL);

  /* definition and creation of FrontJump */
  osThreadDef(FrontJump, FrontJumpTask, osPriorityAboveNormal, 0, 512);
  FrontJumpHandle = osThreadCreate(osThread(FrontJump), NULL);

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

  /* definition and creation of Debug */
  osThreadDef(Debug, DebugTask, osPriorityAboveNormal, 0, 256);
  DebugHandle = osThreadCreate(osThread(Debug), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
    vTaskResume(StartTaskHandle);
    vTaskSuspend(TripodHeadHandle);
    vTaskSuspend(GO1Init_TaskHandle);
    vTaskSuspend(BlueteethTaskHandle);
    vTaskSuspend(GO_Output_LeftHandle);
    vTaskSuspend(GO_Output_RightHandle);
    vTaskSuspend(PIDHandle);
    vTaskSuspend(FrontJumpHandle);
    vTaskSuspend(NRFTaskHandle);
    vTaskSuspend(DebugHandle);
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
    Control_Flag(0,0,1);//选择是否开启陀螺仪与视觉纠偏开关(竞速赛用的）
    IMU_Slove(0,0);//是否开启障碍时腿时刻保持竖直（障碍赛用的）

    printf("Init_Ready\n");
    osDelay(3);

    osDelay(1500); //在调试的时候延迟3秒用来打开急停开关

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

      osDelay(1);
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

    PID_Init(&Yaw_PID_Loop);
    ChangeYawOfPID(0.2f,0.1f,4000.0f,15.0f);//陀螺仪PID初始化

    PID_Init(&VisualLoop);
    ChangePID(&VisualLoop,0.12f,0.04f,4000.0f,15.0f);

    PID_Init(&RadarController);
    ChangePID(&RadarController,0.2f,0.1f,4000.0f,15.0f);

    printf("GO1 Init Ready\n");
    osDelay(3);

    vTaskResume(GO_Output_LeftHandle);
    vTaskResume(GO_Output_RightHandle);
    vTaskResume(PIDHandle);
    vTaskResume(BlueteethTaskHandle);
//    vTaskResume(NRFTaskHandle);
//    vTaskResume(TripodHeadHandle);
    vTaskResume(FrontJumpHandle);
    vTaskResume(DebugHandle);
    vTaskSuspend(NULL); //电机初始化任务完成后自挂捏
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END GO1Init */
}

/* USER CODE BEGIN Header_FrontJumpTask */
/**
* @brief Function implementing the FrontJump thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_FrontJumpTask */
void FrontJumpTask(void const * argument)
{
  /* USER CODE BEGIN FrontJumpTask */
  /* Infinite loop */
  for(;;)
  {
      if(wait_flag == 1)
      {
          AngleLoop[1].Output_limit = 30;
          AngleLoop[2].Output_limit = 30;
          AngleLoop[5].Output_limit = 30;
          AngleLoop[6].Output_limit = 30;
          PID_Set_KP_KI_KD(&AngleLoop[1],40.0f,0,2.0f);
          PID_Set_KP_KI_KD(&AngleLoop[2],40.0f,0,2.0f);
          PID_Set_KP_KI_KD(&AngleLoop[5],40.0f,0,2.0f);
          PID_Set_KP_KI_KD(&AngleLoop[6],40.0f,0,2.0f);

          SetPolarPositionLeg_Delay(77.0f, LegLenthMin, 0,0);
          SetPolarPositionLeg_Delay(77.0f, LegLenthMin, 0,2);
      }

      if(IMU_EulerAngle.EulerAngle[Pitch] < -89.5f)
      {
          osDelay(50);
          pitch = -90.0f + IMU_EulerAngle.EulerAngle[Pitch];
      }
      else
      {
          pitch = IMU_EulerAngle.EulerAngle[Pitch];
      }


    osDelay(1);
  }
  /* USER CODE END FrontJumpTask */
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
//      SetPoint_IMU(&Roll_PID_Loop,0);
//      PID_PosLocM2006(&Roll_PID_Loop,IMU_EulerAngle.EulerAngle[Roll]);

      SetPoint_IMU(&M2006_Position, AngleChange(TargetAngle));
      PID_PosLocM2006(&M2006_Position,struct_debug1[0].total_angle);

      SetPoint_IMU(&M2006_Speed, M2006_Position.Out_put);
      PID_PosLocM2006(&M2006_Speed,struct_debug1[0].speed);

      set_current(&hfdcan1,0x200,0,0,0,0);
//      osDelay(1);
//      set_current(&hfdcan1,0x200,M2006_Speed.Out_put,0,0,0);

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
      osDelay(2);
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

    osDelay(5);
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
      osDelay(2);
  }
  /* USER CODE END GO_Output_RightTask */
}

/* USER CODE BEGIN Header_DebugTask */
/**
* @brief Function implementing the Debug thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_DebugTask */
void DebugTask(void const * argument)
{
  /* USER CODE BEGIN DebugTask */
  /* Infinite loop */
  for(;;)
  {
      usart_printf("%f,%f,%f\n",Radar_FinalData.x_pos,Radar_FinalData.y_pos,Radar_FinalData.yaw);

      osDelay(10);
  }
  /* USER CODE END DebugTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

