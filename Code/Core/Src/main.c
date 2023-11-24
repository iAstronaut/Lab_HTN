/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "button.h"
#include "buzzer.h"
#include "software_timer.h"
#include "lcd.h"
#include "sensor.h"
#include "uart.h"
#include "ds3231.h"
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void sys_init(){
	lcd_init();
	sensor_init();
	buzzer_init();
	button_init();
	uart_init_rs232();
	set_timer(0, READ_SENSOR_TIME);
	set_timer(1, LCD_SENSOR_TIME);
	set_timer(2, ONE_SECOND);
}
void LCD_show_sensor();
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_FSMC_Init();
  MX_I2C1_Init();
  MX_TIM13_Init();
  MX_TIM2_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);
  sys_init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  lcd_Clear(WHITE);
  int volval = 99;
  uint8_t isnotify = 0;
  while (1)
  {
	  if(!is_timer_on(0)){
		  set_timer(0, READ_SENSOR_TIME);
		  sensor_Read();
  		  if(sensor_GetPotentiometer() >= 4095 * 0.7){
  			  if(isnotify == 1){
  				  if(!is_timer_on(2)){
  					  set_timer(2, ONE_SECOND);
  					  isnotify = 0;
  	  				  uart_Rs232SendString("Potentiometer > 70%\n");
  	  				  uart_Rs232SendString("Please reduce Potentiometer\n");
  				  }
  				  buzzer_SetVolume(volval);
  			  }
  			  else if(isnotify == 0){
  				  buzzer_SetVolume(0);
  				  if(!is_timer_on(2)){
  					  set_timer(2, ONE_SECOND);
  					  isnotify = 1;
  				  }
  			  }
  	  	  }
  		  else{
  			  buzzer_SetVolume(0);
  		  }
	  }
	  if(!is_timer_on(1)){
		  set_timer(1, LCD_SENSOR_TIME);
		  LCD_show_sensor();
	  }
	  if(!is_timer_on(3)){
		  set_timer(3, READ_BUTTON_TIME);
		  button_Scan();
		  if(button_count[3] == 1){
			  volval += 10;
			  if(volval > 99){
				  volval = 0;
			  }
			  lcd_ShowIntNum(10, 300, volval, 2, BLUE, WHITE, 16);
		  }
		  if(button_count[7] == 1){
			  volval -= 10;
			  if(volval < 0){
				  volval = 99;
			  }
			  lcd_ShowIntNum(10, 300, volval, 2, BLUE, WHITE, 16);
		  }
	  }

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
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void LCD_show_sensor(){
	lcd_ShowStr(10, 100, "Voltage:", RED, WHITE, 16, 0) ;
	lcd_ShowFloatNum(130, 100, sensor_GetVoltage(), 4, RED,	WHITE, 16);
	lcd_ShowStr(180, 100, "V", RED, WHITE, 16, 0) ;

	lcd_ShowStr(10, 120, "Current:", RED, WHITE, 16, 0) ;
	lcd_ShowFloatNum(130, 120, sensor_GetCurrent(), 4, RED, WHITE, 16) ;
	lcd_ShowStr(180, 120, "mA", RED, WHITE, 16, 0) ;

	lcd_ShowStr(10, 140, "Power:", RED, WHITE, 16, 0) ;
	lcd_ShowFloatNum(130, 140, sensor_GetCurrent() * sensor_GetVoltage(), 4, RED, WHITE, 16);
	lcd_ShowStr(180, 140, "mW", RED, WHITE, 16, 0) ;

	lcd_ShowStr(10, 160, "Light:", RED, WHITE, 16, 0);
	if(sensor_GetLight() <= 4095*0.75){
			lcd_ShowStr(60, 160, "(Strong)", RED, WHITE, 16, 0);
		}
		else{
			lcd_ShowStr(60, 160, "(Weak)  ", RED, WHITE, 16, 0);
		}
	lcd_ShowIntNum(130, 160, sensor_GetLight(), 4, RED, WHITE, 16);


	lcd_ShowStr(10, 180, "Potentiometer:", RED, WHITE, 16, 0);
	lcd_ShowIntNum(130, 180, sensor_GetPotentiometer()*100/4095, 2, RED, WHITE, 16);
	lcd_ShowStr(180, 180, "%", RED, WHITE, 16, 0);

	lcd_ShowStr (10, 200, "Temperature:", RED, WHITE, 16, 0);
	lcd_ShowFloatNum (130, 200, sensor_GetTemperature(), 4, RED, WHITE, 16);
	lcd_ShowStr(180, 200, "C", RED, WHITE, 16, 0) ;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
