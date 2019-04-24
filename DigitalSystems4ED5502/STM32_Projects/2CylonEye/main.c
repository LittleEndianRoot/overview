#include "stm32l476xx.h"

void Cylon_Init();
void Cylon_Eye();
void delayMs(int n);

int main()
{
	Cylon_Init();	
	
	for(;;)
	{
		Cylon_Eye();
	}
	
	return 0;
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

void Cylon_Eye()
{
	Cylon_Init();
	GPIO_TypeDef *GPIO_Port[] = {GPIOA, GPIOB, GPIOB, GPIOB, GPIOB, GPIOA, GPIOC, GPIOC};
	uint32_t GPIO_PinSet[] = {GPIO_BSRR_BS10, GPIO_BSRR_BS3, GPIO_BSRR_BS5, GPIO_BSRR_BS4, GPIO_BSRR_BS10, GPIO_BSRR_BS8, GPIO_BSRR_BS1, GPIO_BSRR_BS0}; 
	uint32_t GPIO_PinReset[] = {GPIO_BSRR_BR10, GPIO_BSRR_BR3, GPIO_BSRR_BR5, GPIO_BSRR_BR4, GPIO_BSRR_BR10, GPIO_BSRR_BR8, GPIO_BSRR_BR1}; 
	
	for(uint32_t i=0; i<sizeof(GPIO_PinSet)/sizeof(GPIO_PinSet[0]); i++)
	{
		GPIO_Port[i]->BSRR |= GPIO_PinSet[i];
		delayMs(500);
		GPIO_Port[i]->BSRR |= GPIO_PinReset[i];
	}
	
	GPIOC->BSRR |= GPIO_BSRR_BR0;
	
	for(uint32_t j=sizeof(GPIO_PinReset)/sizeof(GPIO_PinReset[0])-1; j>0; j--)
	{
		GPIO_Port[j]->BSRR |= GPIO_PinSet[j];
		delayMs(500);
		GPIO_Port[j]->BSRR |= GPIO_PinReset[j];
	}	
}

void delayMs(int n)
{
	int i;
	for(;n > 0; n--)
	{
		for(i = 0; i < 1000; i++);
	}
	
}
