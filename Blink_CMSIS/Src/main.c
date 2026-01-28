#include "stm32f4xx.h"

int main(void)
{
	RCC->AHB1ENR |= (1<<2);
	GPIOC->MODER |= (1<<26);
	GPIOC->MODER &=~ (1<<27);

	while(1)
	{
		GPIOC->ODR |= (1<<13);
		for(int i=0; i<1000000; i++);
		GPIOC->ODR &=~ (1<<13);
		for(int i=0; i<1000000; i++);
	}
}
