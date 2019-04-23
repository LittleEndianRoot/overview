#include "stm32L476xx.h"

void delayMs(int n);

int main()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	GPIOA->MODER &= GPIO_MODER_MODE5;
	GPIOA->MODER |= GPIO_MODER_MODE5_0;
	
	for(;;)
	{
		GPIOA->IDR |= GPIO_BSRR_BS5;
		delayMs(500);
		GPIOA->IDR &= ~GPIO_BSRR_BR5;
		delayMs(500);
	}
	
	return 0;
}

void delayMs(int n)
{
	int i;
	for(;n > 0; n--)
	{
		for(i = 0; i < 80000; i++);
	}
}
