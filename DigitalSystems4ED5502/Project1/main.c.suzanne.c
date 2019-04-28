/* USER CODE BEGIN Header */
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
  * COPYRIGHT(c) 2019 STMicroelectronics
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "stdio.h"
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
ADC_HandleTypeDef hadc1;

/* USER CODE BEGIN PV */
#define LED_NORMAL 0
#define LED_INVERSE 1
#define LED_RESETALL 2
#define LED_SETALL 3
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_ADC1_Init(void);
/* USER CODE BEGIN PFP */
//Have declared all my functions as static to limit the scope
static void MyGPIO_Init(void);
static void MyUSART2_Init(void);
static void MyADC1_Init(void);
static void MyUSART2_writeline(char *usartmsg);
static void MyDelay(uint32_t millisecs);
static void LED_Array(uint8_t ArrayType, uint8_t start_num);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
uint16_t 	Button_stateS1; 		//Store value of S1 pushbutton
uint16_t 	DisplayMode = 1;		//Control DisplayMode - start in Normal mode
uint8_t 	led_start = 0; 			//Variable start position for array
int32_t 	ADC_Result = 0;			//Store ADC conversion result
uint16_t 	ADC_Range = 0;			//Calculate range of ADC result
char 			adc_result_str[5];	//Store conversion of ADC result to string for display
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
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
	MyGPIO_Init();
	MyUSART2_Init();
	MyADC1_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		//Read pin PA6 by testing the value in the IDR
		if ((GPIOA->IDR & GPIO_PIN_6) != 0x00)
		{
			Button_stateS1 = 1;
		}
		else
		{
			Button_stateS1 = 0;
		}
		if (Button_stateS1 == 1)
		{
			if (DisplayMode == 1)
			{
				LED_Array(LED_RESETALL, 0);
				MyDelay(1000);  //Delay for 1 second to make the change of mode easily visible
				LED_Array(LED_NORMAL, 0);
			}
			else
			{
				LED_Array(LED_SETALL, 0);
				MyDelay(1000);	//Delay for 1 second to make the change of mode easily visible
				LED_Array(LED_INVERSE, 0);
			}
			DisplayMode = !DisplayMode; //Invert the Display Mode for the next iteration
			MyUSART2_writeline("LED Cylon Display"); 
			MyUSART2_writeline("\n"); //Have chosen not to include '\r' here so that each new line is indented and clearly visible
		}
		else
		{
			LED_Array(LED_RESETALL, 0);
			//Read the ADC value and show the relevant on-screen message and LED Array pattern
			ADC1->CR |= 0x00000004;				// Set ADSTART (bit 2 of CR) = 1 to start a conversion */
			while (!(ADC1->ISR & 0x00000004)) {} //Keep looping until the conversion is completed (EOC (bit 2 of ISR) = 1)

			ADC_Result = ADC1->DR; 				//Read the conversion result
			ADC_Range = (ADC_Result/512); //Determine the array range of the result
			switch (ADC_Range) {
				case 0: led_start = 7;
								break;
				case 1: led_start = 6;
								break;
				case 2: led_start = 5;
								break;
				case 3: led_start = 4;
								break;
				case 4: led_start = 3;
								break;
				case 5: led_start = 2;
								break;
				case 6: led_start = 1;
								break;
				case 7: led_start = 0;
								break;
				default:
								break;
				}
			LED_Array(LED_SETALL,led_start);
			sprintf(adc_result_str,"%d",ADC_Result);	//Using sprintf function to convert int to string
			MyUSART2_writeline("ADC Result ");
			MyUSART2_writeline(adc_result_str);
			MyUSART2_writeline("\r\n");		
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 16;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_ADC1CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /**Configure the main internal regulator output voltage 
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /**Common config 
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /**Configure the ADC multi-mode 
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }
  /**Configure Regular Channel 
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */

/* USER CODE BEGIN 4 */
static void MyGPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
	RCC->AHB2ENR |= 0x1; 		//Enable bit 0 for GPIOAEN
	RCC->AHB2ENR |= 0x2; 		//Enable bit 1 for GPIOBEN
	RCC->AHB2ENR |= (1<<2); //Enable bit 2 for GPIOCEN
		
	/*Reset all ports we are using */
	GPIOA->BRR = (1<<6)|(1<<8)|(1<<10); 				//Pins 6, 8 and 10
	GPIOB->BRR = (1<<3)|(1<<4)|(1<<5)|(1<<10);	//Pins 3, 4, 5 and 10
	GPIOC->BRR = 0x1|0x2;												//Pins 0 and 1
	
	//Configure PC0 and PC1 as Outputs with no Pull and Low speed
	GPIOC->MODER &= 0x5; 		//Force bits 3 and 1 to 0
	GPIOC->MODER |= 0x5; 		//Force bits 2 and 0 to 1
	GPIOC->OSPEEDR &= 0x0; 	//Force bits 3->0 to 0
	GPIOC->PUPDR &= 0x0;		//Force bits 3->0 to 0

  //Configure PA6 as Input with Pull Up
	GPIOA->MODER &= ~0x3000; 		//Force bits 13 and 12 to 0
	GPIOA->OSPEEDR &= ~0x3000; 	//Force bits 13 and 12 to 0
	GPIOA->PUPDR &= ~0x2000;		//Force bit 13 to 0
	GPIOA->PUPDR |= 0x1000; 		//Force bit 12 to 1

  //Configure PB10 PB3 PB4 PB5 as Outputs with no Pull and Low speed
	GPIOB->MODER &= ~0x200a80; 		//Force bits 7, 9, 11 and 21 to 0
	GPIOB->MODER |= 0x100540; 		//Force bits 6, 8, 10 and 20 to 1
	GPIOB->OSPEEDR &= ~0x300fc0;	//Force bits 6->11 and 20, 21 to 0
	GPIOB->PUPDR &= ~0x300fc0;		//Force bits 6->11 and 20, 21 to 0

  //Configure PA8 PA10 as Outputs with no Pull and Low speed
	GPIOA->MODER &= ~0x220000; 		//Force bits 17 and 21 to 0
	GPIOA->MODER |= 0x110000; 		//Force bits 16 and 20 to 1
	GPIOA->OSPEEDR &= ~0x330000;	//Force bits 16, 17 and 20, 21 to 0
	GPIOA->PUPDR &= ~0x330000;		//Force bits 16, 17 and 20, 21 to 0
	
}


static void MyUSART2_Init(void)
{
	  RCC->AHB1ENR |= 0x1;          // Enable GPIOA clock - also part of GPIO_Init but leaving it here for function independence
    RCC->APB1ENR1 |= (1<<17);    	// Enable USART2 clock by setting USART2EN (bit 17)

    //Configure PA2 & PA3 for USART2
    GPIOA->AFR[0] &= ~0x8800;			//AFR[0] is the Low register - setting AF7 for PA2 & PA3 (to USART2_TX &_RX respectively)
    GPIOA->AFR[0] |= 0x7700;    	//Prev statement sets the 0 bits, this sets the 1 bits
    GPIOA->MODER  &= ~0x50;				//Enable alternate function mode for PA2 & PA3 - this sets the 0 bits
    GPIOA->MODER  |= 0xa0;		   	//This sets the 1 bits
	
    USART2->BRR = 0x02b6;       //Set to USARTDIV calculated for 115200 baud @ 80 MHz */
    USART2->CR1 = (1<<3);       //Set Transmit Enable (bit 3)
    USART2->CR2 = 0x0000;       //Set bits 12 & 13 to 0 (1 stop bit)
    USART2->CR1 |= 0x1;		      //Set USART Enable (bit 0)

}
static void MyADC1_Init(void)
{
	RCC->AHB1ENR |= 0x1;          // Enable GPIOA clock - also part of GPIO_Init but leaving it here for function independence
	RCC->AHB2ENR |= 0x00002000;   // Set ADC Clock enable (bit 13)

	ADC1->CR &= ~0x20000000;			// Exit Deep Power Down Mode mode - set DEEPPWD (bit 29) to 0
	ADC1->CR |= 0x10000000;				// Enable ADC Voltage Regulator - set ADVREGEN (bit 28) to 1

	MyDelay(1000);							//Delay for 1 second

	//Below calibration logic provided by Ciaran as ADC conversion was only reaching 4027
	ADC1->CR &= ~0x40000000;		/* Calibrate for single-ended input */
	ADC1->CR |= 0x80000000;			/* Start calibration */

	while((ADC1->CR & 0x80000000) == 0x80000000)
		;									/* Wait for calibration to finish */

	MyDelay(1);
	
	//PA0 is set up to ADC1_IN5 through STMCube32MX configuration
	
	ADC1->CFGR = 0x80000000;		/* Allow Software Trigger, turn off DMA, 12-bit res */

	ADC1->SMPR1 = 0x00010000;		// Set SMP5 to 12.5 ADC clock cycles (bit 16 to 1)
	ADC1->SQR1 = 0x00000140;		// Set Channel 5 on SQ1 (bits 10 to 6) - using Single ADC Conv so bits 3:0 stay at 0

	ADC123_COMMON->CCR = 0x00200000;			/* This is the common control register. Setting prescale to 1000 => divide by 32  */
		
	ADC1->CR |= 0x1;						// Enable ADC2 - set ADEN (bit 0) to 1 

	MyDelay(1000);							//Delay for 1 second
}


static void MyUSART2_writeline(char *usartmsg)
{
	for (uint8_t i=0; i<strlen(usartmsg);i++)
	{
			while (!(USART2->ISR & (1<<7))) {} //Test if ISR TXE bit (bit 7) is empty - if so keep looping   
			USART2->TDR = (usartmsg[i] & 0xFF); //Write the value to the TDR - & 0xFF converts it to an 8-bit value (9 bits are allowed)
	}
}

static void MyDelay(uint32_t millisecs)
{
	RCC->APB1ENR1 |= 1;              // Enable TIM2 clock (bit 0)
  TIM2->PSC = 8000 - 1;            // Prescale by 8000 
  TIM2->ARR = (millisecs*10) - 1;  // Set auto reload value to milliseconds by 10 (e.g. 1 second = 10000) 
  TIM2->CNT = 0;                   // Clear timer counter 
  TIM2->CR1 = 1;                   // Enable TIM2 by setting CEN bit (bit 0)

  while(!(TIM2->SR & 1)) {}   //Test if SR UIF bit (bit 0) is empty - if so keep looping
  TIM2->SR &= ~1;             //Set the UIF bit to 0

}
static void LED_Array(uint8_t ArrayType, uint8_t start_num)
{
	
int8_t led_count = 0; //LED counter used in for loops

//Defining GPIOC and Pins 0 & 1 here for illustration
#define MyGPIOC ((GPIO_TypeDef *) 0x48000800) 
#define MyGPIO_PIN0 ((uint16_t)0x0001)
#define MyGPIO_PIN1 ((uint16_t)0x0002)  


GPIO_TypeDef *LED_Port[8] = {GPIOA, GPIOB, GPIOB, GPIOB, GPIOB, GPIOA, MyGPIOC, MyGPIOC};
uint16_t LED_Pin_Number[8] = {GPIO_PIN_10, GPIO_PIN_3, GPIO_PIN_5, GPIO_PIN_4, GPIO_PIN_10, GPIO_PIN_8, MyGPIO_PIN1, MyGPIO_PIN0};
uint16_t delay_ms = 250; //Standard delay is 0.25sec

	switch (ArrayType) 
	{
		case LED_RESETALL: //Turn all the LEDs off
					for (led_count = 0; led_count < 8; led_count++) 
					{					
							LED_Port[led_count]->BRR = (uint32_t)LED_Pin_Number[led_count];
					}
			break;
		case LED_SETALL: //Turn all the LEDs on
					for (led_count = start_num; led_count < 8; led_count++) 
					{					
							LED_Port[led_count]->BSRR = (uint32_t)LED_Pin_Number[led_count];
					}
			break;
		case LED_NORMAL: //Turn on one LED at a time
					for (led_count = 0; led_count < 7; led_count++) //Go up from LED 2 to 8 (we start at 9 in the other loop)
					{
						if (led_count == 0) //If it's the first LED, we can't use our standard logic so need to call out the positions 
						{
							LED_Port[1]->BRR = (uint32_t)LED_Pin_Number[1];
							LED_Port[0]->BSRR = (uint32_t)LED_Pin_Number[0];
						}
						else
						{
							LED_Port[led_count-1]->BRR = (uint32_t)LED_Pin_Number[led_count-1];
							LED_Port[led_count]->BSRR = (uint32_t)LED_Pin_Number[led_count];
						}
						MyDelay(delay_ms);
					}
					for (led_count = 7; led_count >= 0; led_count--) //Go back from LED 9 to 3 (we start at 2 in the other loop)
					{
						if (led_count == 7) //If it's the first LED, we can't use our standard logic so need to call out the positions 
						{
							LED_Port[6]->BRR = (uint32_t)LED_Pin_Number[6];
							LED_Port[7]->BSRR = (uint32_t)LED_Pin_Number[7];
						}
						else
						{
							LED_Port[led_count+1]->BRR = (uint32_t)LED_Pin_Number[led_count+1];
							LED_Port[led_count]->BSRR = (uint32_t)LED_Pin_Number[led_count];
						}
						MyDelay(delay_ms);
					}
			break;
		case LED_INVERSE: //Turn off one LED at a time
					for (led_count = 0; led_count < 7; led_count++) //Go up from LED 2 to 8 (we start at 9 in the other loop)
					{
						if (led_count == 0) //If it's the first LED, we can't use our standard logic so need to call out the positions 
						{
							LED_Port[1]->BSRR = (uint32_t)LED_Pin_Number[1];
							LED_Port[0]->BRR = (uint32_t)LED_Pin_Number[0];
						}
						else
						{
							LED_Port[led_count-1]->BSRR = (uint32_t)LED_Pin_Number[led_count-1];
							LED_Port[led_count]->BRR = (uint32_t)LED_Pin_Number[led_count];
						}
						MyDelay(delay_ms);
					}
					for (led_count = 7; led_count >= 0; led_count--) //Go back from LED 9 to 3 (we start at 2 in the other loop)
					{
						if (led_count == 7) //If it's the first LED, we can't use our standard logic so need to call out the positions 
						{
							LED_Port[6]->BSRR = (uint32_t)LED_Pin_Number[6];
							LED_Port[7]->BRR = (uint32_t)LED_Pin_Number[7];
						}
						else
						{
							LED_Port[led_count+1]->BSRR = (uint32_t)LED_Pin_Number[led_count+1];
							LED_Port[led_count]->BRR = (uint32_t)LED_Pin_Number[led_count];
						}
						MyDelay(delay_ms);
					}
			break;
		default:
			break;
	}
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
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
