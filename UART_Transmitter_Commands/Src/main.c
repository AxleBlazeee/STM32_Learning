#include "stm32f4xx.h"
/*
 * Project     : UART String Transmitter
 * Target MCU  : STM32F401CCU6 (Black Pill) / STM32F4xx Series
 * Description : Bare-Metal firmware to continuously transmit a string "KATZE"
 *               via USART2 (PA2 - TX pin) using direct register manipulation.
 *               No HAL. No STM32CubeMX. Pure bare-metal C.
 * Protocol    : UART | Baud Rate: 9600 | 8-bit data | No Parity | 1 Stop Bit
 * Author      : Abhishek Rawat
 */


//////		 For USART	 	/////////////

#define USART2_UE_Set (1<<13)
#define USART2_BRR_Value ((104<<4) | 3)
#define USART2_TE_Set (1<<3)
#define USART2_RCC_Enable (1<<17)
#define GPIOA_RCC_Enable (1<<0)
#define GPIOA_MODER_AFM (1<<5)
#define GPIOA_AFRL_AF07 ((1<<8) | (1<<9) | (1<<10))		//AF07 for PA2 (USART2 TX)
#define USART2_TXE (1<<7)


//////		 Data to transmit	 	/////////////
static uint8_t data[] = "KATZE";


int main()
{
	RCC->APB1ENR |= USART2_RCC_Enable;	//To enable clock for USART2
	RCC->AHB1ENR |= GPIOA_RCC_Enable;	//To enable clock for GPIO PORTA

	GPIOA->MODER |= GPIOA_MODER_AFM;	//To set PA2 pin in Alternate Function Mode
	GPIOA->AFR[0] |= GPIOA_AFRL_AF07;	//To set PA2 into UART pin

	USART2->CR1 |= USART2_UE_Set;		//To enable USART
	USART2->BRR = USART2_BRR_Value;		//To set baud rate of 9600
	USART2->CR1 |= USART2_TE_Set;		//Transmitter enable

	while(1)	//Infinite loop
	{
		for(int i=0; i<sizeof(data)-1; i++)
		{
			while (!(USART2->SR & USART2_TXE));		//Wait until TXE bit is set (TX buffer is empty and ready for new data)
			USART2->DR = data[i];					//Load byte into Data Register to transmit it via UART
		}
	}
}
