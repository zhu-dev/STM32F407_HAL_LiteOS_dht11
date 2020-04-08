
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2020 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "los_sys.h"
#include "los_task.ph"
#include "los_memory.ph"
#include "los_mux.h"

#include "dht11.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
//任务句柄变量声明
UINT32 LED0_Task_Handle;
UINT32 LED1_Task_Handle;
UINT32 DHT11_Task_Handle;

/* 创建互斥锁句柄 */
UINT32 MUTEX_OS_Printf_Handle;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
static UINT32 AppTaskCreate(void);

static UINT32 Create_LED0_Task(void);
static UINT32 Create_LED1_Task(void);
static UINT32 Create_DHT11_Read_Task(void);

static void Led0_Task(void);
static void Led1_Task(void);
static void Read_DHT11_Data(void);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

	UINT32 uwRet = LOS_OK;   //定义一个任务创建返回值，默认为创建成功
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
	
  /* USER CODE BEGIN 2 */
	 	while(DHT11_Init())	//DHT11初始化	
	{
			printf("DHT11初始化失败 \r\n");
	}
	
	/* 显示shell信息 */
	printf("硬件初始化成功！！\r\n");
	printf("LiteOS内核初始化中......\r\n");
	
	/* LiteOS 内核初始化 */
	uwRet = LOS_KernelInit();
	if(uwRet != LOS_OK){
		printf("内核初始化失败....\r\n");
	}	
	printf("内核初始化成功！！\r\n");
	/* 创建用于创建任务的任务，哈哈 */
	uwRet = AppTaskCreate();
	if(uwRet != LOS_OK){
		printf("任务创建失败....\r\n");
	}
	
	/* 开启 LiteOS 任务调度 */
	LOS_Start();
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//		LED0_ON();
//		printf("led on\t\n");
//		HAL_Delay(500);
//		LED0_OFF();
//		printf("led on\t\n");
//		HAL_Delay(500);
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
static UINT32 AppTaskCreate(void){
	
	/* 创建任务返回值变量，默认为LOS_OK */
	UINT32 uwRet = LOS_OK;

	/* 创建串口资源互斥锁保护串口输出完成一次数据不被打断 */
	uwRet = LOS_MuxCreate(&MUTEX_OS_Printf_Handle);
	if(uwRet != LOS_OK){
		printf("创建串口互斥锁失败 \r\n");
	}
	
	uwRet =Create_LED0_Task();
	if(uwRet != LOS_OK){
		printf("创建LED0任务失败 \r\n");
	}
	
	uwRet =Create_LED1_Task();
	if(uwRet != LOS_OK){
		printf("创建LED1任务失败 \r\n");
	}
	
		uwRet =Create_DHT11_Read_Task();
	if(uwRet != LOS_OK){
		printf("创建DHT11_Read任务失败 \r\n");
	}
	
	return LOS_OK;
}

static UINT32 Create_LED0_Task(void){
	/* 创建任务返回值变量，默认为LOS_OK */
	UINT32 uwRet = LOS_OK; 

	TSK_INIT_PARAM_S tsk_init_param;
	tsk_init_param.pcName = "LED0_Task";
	tsk_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)Led0_Task;
	tsk_init_param.usTaskPrio = 5;
	tsk_init_param.uwStackSize = 1024;
	
	uwRet=LOS_TaskCreate(&LED0_Task_Handle,&tsk_init_param);/*创建任务 */
	return uwRet;
}

static UINT32 Create_LED1_Task(void){
	/* 创建任务返回值变量，默认为LOS_OK */
	UINT32 uwRet = LOS_OK; 

	TSK_INIT_PARAM_S tsk_init_param;
	tsk_init_param.pcName = "LED1_Task";
	tsk_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)Led1_Task;
	tsk_init_param.usTaskPrio = 6;
	tsk_init_param.uwStackSize = 1024;
	
	uwRet=LOS_TaskCreate(&LED1_Task_Handle,&tsk_init_param);/*创建任务 */
	return uwRet;
}


static UINT32 Create_DHT11_Read_Task(void){
	/* 创建任务返回值变量，默认为LOS_OK */
	UINT32 uwRet = LOS_OK; 

	TSK_INIT_PARAM_S tsk_init_param;
	tsk_init_param.pcName = "Read_DHT11_Data";
	tsk_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)Read_DHT11_Data;
	tsk_init_param.usTaskPrio = 7;
	tsk_init_param.uwStackSize = 1024;
	
	uwRet=LOS_TaskCreate(&DHT11_Task_Handle,&tsk_init_param);/*创建任务 */
	return uwRet;
}

static void Led0_Task(void){
	while(1){
		LED0_ON();
		LOS_TaskDelay(1000);
		//HAL_Delay(1000);
		LED0_OFF();
		LOS_TaskDelay(1000);
		//HAL_Delay(1000);
		
		
		
	}
}

static void Led1_Task(void){
	while(1){
		
		//OS_Debug_Printf("\r\n进入LED1任务\r\n");
		LED1_ON();
		LOS_TaskDelay(300);
		LED1_OFF();
		LOS_TaskDelay(300);
	}
}

static void Read_DHT11_Data(void){
	
	uint8_t temp;
	uint8_t humi;
	
	
	while(1){
		DHT11_Read_Data(&temp,&humi);
		OS_Debug_Printf("\r\n温度：%d\t湿度：%d\r\n",temp,humi);
		LOS_TaskDelay(500);
	}
}




/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
