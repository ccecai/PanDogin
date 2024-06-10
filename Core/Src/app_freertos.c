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
#include "StartTask.h"
#include "RemoteController.h"
#include "GO1_Motor.h"
#include "FrontJumpTask.h"
#include "DebugTask.h"
#include "TripodHeadTask.h"
#include "imuTask.h"
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
osThreadId TripodHeadHandle;
osThreadId GO_Output_LeftHandle;
osThreadId PIDHandle;
osThreadId GO_Output_RightHandle;
osThreadId DebugHandle;
osThreadId IMUHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDebug(void const * argument);
void BlueTeeth_RemoteControl(void const * argument);
void GO1Init(void const * argument);
void FrontJumpTask(void const * argument);
void TripodHeadTask(void const * argument);
void GO_Output_LeftTask(void const * argument);
void PIDTask(void const * argument);
void GO_Output_RightTask(void const * argument);
void DebugTask(void const * argument);
void IMUTask(void const * argument);

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
  osThreadDef(Debug, DebugTask, osPriorityLow, 0, 256);
  DebugHandle = osThreadCreate(osThread(Debug), NULL);

  /* definition and creation of IMU */
  osThreadDef(IMU, IMUTask, osPriorityAboveNormal, 0, 512);
  IMUHandle = osThreadCreate(osThread(IMU), NULL);

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
    vTaskSuspend(DebugHandle);
    vTaskSuspend(IMUHandle);
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

    vTaskResume(GO1Init_TaskHandle);

  /* Infinite loop */
  for(;;)
  {
      Start_Task();
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
      RemoteControl_Task();
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
    GO1_Init();

    vTaskResume(GO_Output_LeftHandle);
    vTaskResume(GO_Output_RightHandle);
    vTaskResume(PIDHandle);
    vTaskResume(BlueteethTaskHandle);
    vTaskResume(TripodHeadHandle);
    vTaskResume(FrontJumpHandle);
    vTaskResume(DebugHandle);
    vTaskResume(IMUHandle);
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
      FrontJump_Task();
  }
  /* USER CODE END FrontJumpTask */
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

  /* Infinite loop */
  for(;;)
  {
      TripodHead_Task();
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
      GO1_LeftOutput_Task();
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
      PID_CalcTask();
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
      GO1_RightOutput_Task();
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
      Debug_Task();
  }
  /* USER CODE END DebugTask */
}

/* USER CODE BEGIN Header_IMUTask */
/**
* @brief Function implementing the IMU thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_IMUTask */
void IMUTask(void const * argument)
{
  /* USER CODE BEGIN IMUTask */
  /* Infinite loop */
  for(;;)
  {
      IMU_Task();
  }
  /* USER CODE END IMUTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

