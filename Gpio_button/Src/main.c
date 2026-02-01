#include "stm32f4xx.h"

int main(void)
{
	RCC->AHB1ENR |= (1<<0);
	RCC->AHB1ENR |= (1<<2);

	GPIOC->MODER |= (1<<26);
	GPIOA->PUPDR |= (1<<0);

	while(1)
	{
		if((GPIOA->IDR & 0x00000001)== 0x00000001)
			GPIOC->ODR |= (1<<13);
		else
			GPIOC->ODR &=~ (1<<13);
//		if((GPIOA->IDR & 0x00000001)==0x00000000)
//			GPIOC->ODR &=~ (1<<13);
//		else
//			GPIOC->ODR |= (1<<13);
	}
}
