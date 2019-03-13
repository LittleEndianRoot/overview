#include "stm32l476xx.h"

//Function declarations 
void USART_Init(void);
void USART2_write(char ch);
uint32_t ADC1_Init(void);
void delayMs(int ms);
void Cylon_Init();
void CylonEye();
void CylonEyeFill();
void CylonEyeEmty();
uint32_t buttonPress();

// gloabal variables
int32_t ADC_Result;
char Msg[19] = "LED Cylon Display\r\n";

int main(void)
{
	int32_t DisplayMode;
	uint32_t ADC_Init_Status;
	ADC_Result = 0;
	
	ADC_Init_Status = ADC1_Init();
	
	if(ADC_Init_Status)
	{
		GPIOC->BSRR |= GPIO_BSRR_BS0;
		delayMs(500000);
		GPIOC->BSRR |= GPIO_BSRR_BR0;
		delayMs(500000);
	}

	while(1)
	{
		DisplayMode = buttonPress();
		if(DisplayMode)
		{
			
			CylonEyeFill();
				// Start the ADC
			ADC1->CR |= ADC_CR_ADSTART;
			delayMs(1);
			
			while(!(ADC1->ISR & ADC_ISR_EOC)){};
			ADC_Result = ADC1->DR;
			
			if(ADC_Result > ADC_ISR_EOS)
			{
					GPIOC->BSRR = GPIO_BSRR_BS0;
			}
			else
			{
					GPIOC->BSRR = GPIO_BSRR_BR0;
			}
				CylonEyeEmty();
				
				for(int i = 0; i <= 19; i++)
				{
					USART2_write(Msg[i]);
					CylonEye();
				}
		}
	
	}
	
}

uint32_t ADC1_Init()
{
	// ------------| LED PC0 |-------------------
	
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	GPIOC->MODER &= ~(GPIO_MODER_MODE0);
	GPIOC->MODER |= GPIO_MODER_MODE0_0;
	
	//-------------| ADC PA0 |-------------------
	
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	GPIOA->MODER &= ~(GPIO_MODER_MODE2);
	GPIOA->MODER |= GPIO_MODER_MODE2;
	
	//-------------| ADC1 Setup |----------------
	// Clock setup
	
	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;
	RCC->CCIPR |= RCC_CCIPR_ADCSEL;
	
	// Take ADC out of Deep power down mode and enable the voltage regulator
	
	ADC1->CR &= ~(ADC_CR_DEEPPWD);
	ADC1->CR |= ADC_CR_ADVREGEN;
	delayMs(1);
	
	// Input status register for ADC1
	
	ADC1->ISR |= ADC_ISR_ADRDY;
	delayMs(1);
	
	// Configuration register setup
	
	ADC1->CFGR &= ~(ADC_CFGR_RES); // 12 resolution 
	ADC1->CFGR &= ~(ADC_CFGR_EXTEN); // SW trigger (HW trigger disabled) 
	ADC1->CFGR &= ~(ADC_CFGR_EXTSEL); // Even 0: 0000
	//ADC1->CFGR &= ~(ADC_CFGR_ALIGN); // right alignment 
	ADC1->CFGR &= ~(ADC_CFGR_DMAEN); // turning off DMA
	
	// ADC Sample Time Register setup
	
	ADC1->SMPR1 = ADC_SMPR1_SMP5_1; // 12.5 ADC sampling time oh Channel 5
	
	// ADC Regular Sequence Register, 1 channel conversion
	
	ADC1->SQR1 &= ~(ADC_SQR1_SQ1);
	ADC1->SQR1 |= ADC_SQR1_SQ1_0;
	
	// Length

	ADC1->SQR1 &= ~(ADC_SQR1_L);
	ADC1->SQR1 |= ADC_SQR1_L_0;
	
	// Enable ADC 
	
	ADC1->CR |= ADC_CR_ADEN; 
	delayMs(1);
	
	if(ADC1->ISR & ADC_ISR_ADRDY)
	{
		return 1;
	}
	else
	{
		return 0;
	}
	
}

// Initialize UART pins, Baudrate
void USART2_init(void)
{
	//RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;
	
	/*Configure PA2 for USART2_TX*/
	GPIOA->AFR[0] &= ~0x0f00;
	GPIOA->AFR[0] |= 0x0700;
	GPIOA->MODER &= ~(GPIO_MODER_MODE2);
	GPIOA->MODER |= GPIO_MODER_MODE2_1;
	
	USART2->BRR = 0x208d;   // 9600 baud @ 4MHz
	USART2->CR1 = USART_CR1_OVER8 | ~USART_CR1_M | USART_CR1_TE; // enable Tx, 8-bit data
	USART2->CR2 = ~(USART_CR2_STOP); // 1 stop bit
	USART2->CR3 = ~(USART_CR3_CTSE); // Hardware flow control disabled 
	USART2->CR1 = USART_CR1_UE; // Enable USART 
	

}

void USART2_write(char ch)
{
	while(!(USART2->ISR & USART_ISR_TXE)) {} // wait until the Tx buffer is empty
	
	for(int i = 0; i <=19; i++)	
	{
		USART2->TDR = (Msg[i]);
	}
}

void Cylon_Init()
{
	//--------| GPIO CLOCKS |--------------
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN;
	
	//--------| GPIO MODE REGISTERS |---------------
	GPIOA->MODER &= ~(GPIO_MODER_MODE10);
	GPIOA->MODER |= GPIO_MODER_MODE10_0;
	
	GPIOB->MODER &= ~(GPIO_MODER_MODE3);
	GPIOB->MODER |= GPIO_MODER_MODE3_0;
	
	GPIOB->MODER &= ~(GPIO_MODER_MODE5);
	GPIOB->MODER |= GPIO_MODER_MODE5_0;
	
	GPIOB->MODER &= ~(GPIO_MODER_MODE4);
	GPIOB->MODER |= GPIO_MODER_MODE4_0;
	
	GPIOB->MODER &= ~(GPIO_MODER_MODE10);
	GPIOB->MODER |= GPIO_MODER_MODE10_0;
	
	GPIOA->MODER &= ~(GPIO_MODER_MODE8);
	GPIOA->MODER |= GPIO_MODER_MODE8_0;
	
	GPIOC->MODER &= ~(GPIO_MODER_MODE1);
	GPIOC->MODER |= GPIO_MODER_MODE1_0;
	
	GPIOC->MODER &= ~(GPIO_MODER_MODE0);
	GPIOC->MODER |= GPIO_MODER_MODE0_0;
	
	//----------| OUTPUT TYPES REGISTERS |-----------------
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT10);											
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT3);										
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT5);												
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT4);												
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT10);													
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT8);
	GPIOC->OTYPER &= ~(GPIO_OTYPER_OT1);									
	GPIOC->OTYPER &= ~(GPIO_OTYPER_OT0);
	
	//----------| OUTPUT SPEED REGISTERS |--------------
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED10;
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED3;
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED5;
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED4;
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED10;
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED8;
	GPIOC->OSPEEDR |= GPIO_OSPEEDR_OSPEED1;
	GPIOC->OSPEEDR |= GPIO_OSPEEDR_OSPEED0;
	
	//-----------| PULL UP PULL DOWN REGIESTERS |-----------------
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD10);											
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD3);										
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD5);												
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD4);												
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD10);													
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD8);
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD1);									
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD0);
	
}

void CylonEye()
{
	Cylon_Init();
	GPIO_TypeDef *GPIO_Port[] = {GPIOA, GPIOB, GPIOB, GPIOB, GPIOB, GPIOA, GPIOC, GPIOC};
	uint32_t GPIO_PinSet[] = {GPIO_BSRR_BS10, GPIO_BSRR_BS3, GPIO_BSRR_BS5, GPIO_BSRR_BS4, GPIO_BSRR_BS10, GPIO_BSRR_BS8, GPIO_BSRR_BS1, GPIO_BSRR_BS0}; 
	uint32_t GPIO_PinReset[] = {GPIO_BSRR_BR10, GPIO_BSRR_BR3, GPIO_BSRR_BR5, GPIO_BSRR_BR4, GPIO_BSRR_BR10, GPIO_BSRR_BR8, GPIO_BSRR_BR1, GPIO_BSRR_BR0}; 
	
	for(uint32_t i=0; i<sizeof(GPIO_PinSet)/sizeof(GPIO_PinSet[0]); i++)
	{
		GPIO_Port[i]->BSRR |= GPIO_PinSet[i];
		delayMs(100000);
		GPIO_Port[i]->BSRR |= GPIO_PinReset[i];
	}
	
	for(uint32_t j=sizeof(GPIO_PinSet)/sizeof(GPIO_PinSet[0])-1; j>0; j--)
	{
		GPIO_Port[j]->BSRR |= GPIO_PinSet[j];
		delayMs(100000);
		GPIO_Port[j]->BSRR |= GPIO_PinReset[j];
		delayMs(10000);
	}
	
}

void CylonEyeFill()
{
	Cylon_Init();
	GPIO_TypeDef *GPIO_Port[] = {GPIOA, GPIOB, GPIOB, GPIOB, GPIOB, GPIOA, GPIOC, GPIOC};
	uint32_t GPIO_PinSet[] = {GPIO_BSRR_BS10, GPIO_BSRR_BS3, GPIO_BSRR_BS5, GPIO_BSRR_BS4, GPIO_BSRR_BS10, GPIO_BSRR_BS8, GPIO_BSRR_BS1, GPIO_BSRR_BS0}; 
	
	for(uint32_t i=0; i<sizeof(GPIO_PinSet)/sizeof(GPIO_PinSet[0]); i++)
	{
		GPIO_Port[i]->BSRR |= GPIO_PinSet[i];
		delayMs(100000);
	}
	
}

void CylonEyeEmty()
{
	Cylon_Init();
	GPIO_TypeDef *GPIO_Port[] = {GPIOA, GPIOB, GPIOB, GPIOB, GPIOB, GPIOA, GPIOC, GPIOC};
	uint32_t GPIO_PinReset[] = {GPIO_BSRR_BR10, GPIO_BSRR_BR3, GPIO_BSRR_BR5, GPIO_BSRR_BR4, GPIO_BSRR_BR10, GPIO_BSRR_BR8, GPIO_BSRR_BR1, GPIO_BSRR_BR0}; 
	
	for(uint32_t j=sizeof(GPIO_PinReset)/sizeof(GPIO_PinReset[0])-1; j>0; --j)
	{
		GPIO_Port[j]->BSRR |= GPIO_PinReset[j];
		delayMs(100000);
	}
	
}

uint32_t buttonPress()
{
		// Setting up the Push button on PA6
		// GPIOA Clock 
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

		GPIOA->MODER &= ~(GPIO_MODER_MODE6);
		GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD6);
		
	if(GPIOA->IDR & GPIO_IDR_ID6)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void delayMs(int ms)
{
	volatile int msTicks;
	for(msTicks=0; msTicks < ms; msTicks++);
}
