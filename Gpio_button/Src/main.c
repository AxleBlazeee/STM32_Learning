#include "stm32f4xx.h"  // Including all the register addresses

int main(void)
{
	RCC->AHB1ENR |= (1<<0); //Clock enable for GPIOA
	RCC->AHB1ENR |= (1<<2);	//Clock enable for GPIOC

	GPIOC->MODER |= (1<<26); //Output mode on pin PC13
	GPIOA->PUPDR |= (1<<0); //Using internal Pull up resistor on pin PA0

	while(1)
	{
		if((GPIOA->IDR & 0x00000001)== 0x00000001) //If button pressed 
			GPIOC->ODR |= (1<<13);		   //led ON
		else					   //Else
			GPIOC->ODR &=~ (1<<13);		   //led OFF
//		if((GPIOA->IDR & 0x00000001)==0x00000000)
//			GPIOC->ODR &=~ (1<<13);
//		else
//			GPIOC->ODR |= (1<<13);
	}
}
