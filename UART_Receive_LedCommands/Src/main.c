/*
 * Project	  : UART-Controlled LED Toggle
 * Target MCU : STM32F401CCU6 (Black Pill) / STM32F4xx Series
 * Description: Bare-Metal firmware to receive data via USART2 (PA3 - RX pin)
 *              and toggle an on board LED (PC13) based on received character.
 *              Send 'A' to turn LED ON, Send 'B' to turn LED OFF.
 * Protocol   : UART | Baud Rate: 9600 | 8-bit data | No Parity | 1 Stop Bit
 * Author	  : Abhishek Rawat
 */



#include "stm32f4xx.h"

uint8_t USART_data(void);

int main(void)
{
	//////		 For USART	 	/////////////

	RCC->AHB1ENR |= (1<<0);		//To enable clock for GPIO PORTA
	RCC->APB1ENR |= (1<<17);	//To enable clock for USART2

	GPIOA->MODER |= (1<<7);		//To set PA3 pin in Alternate Function Mode
	GPIOA->MODER &=~ (1<<6);	//To set PA3 pin in Alternate Function Mode

	GPIOA->AFR[0] |= (1<<12) | (1<<13) | (1<<14);	//To set PA3 into UART pin
	GPIOA->AFR[0] &=~ (1<<15);						//To set PA3 into UART pin

	USART2->CR1 |= (1<<13);			//To enable USART


	/*
		 * -----------------------------------------------------------------------
		 * BRR (Baud Rate Register) Calculation:
		 * -----------------------------------------------------------------------
		 * Formula:
		 *          USARTDIV = F_clk / (16 * Baud Rate)
		 *
		 * Given:
		 *          F_clk     = 16 MHz (default HSI clock on STM32F4)
		 *          Baud Rate = 9600 bps
		 *          Oversampling = 16x (OVER8 bit = 0 in CR1)
		 *
		 * Step 1 — Calculate USARTDIV:
		 *          USARTDIV = 16,000,000 / (16 * 9600)
		 *                   = 16,000,000 / 153,600
		 *                   = 104.1666...
		 *
		 * Step 2 — Extract Mantissa (integer part):
		 *          Mantissa = 104  →  0x68
		 *
		 * Step 3 — Extract Fraction (decimal part × 16, rounded):
		 *          Fraction = 0.1666... × 16 = 2.666... ≈ 3  →  0x3
		 *
		 * Step 4 — Load into BRR Register:
		 *          BRR = (Mantissa << 4) | Fraction
		 *          BRR = (104 << 4) | 3
		 *              = 0x683
		 * -----------------------------------------------------------------------
	*/

	USART2->BRR = (104 << 4) | 3;	//To set baud rate of 9600
	USART2->CR1 |= (1<<2);			//Receiver enable




	//////		For LED		 //////////////

	RCC->AHB1ENR |= (1<<2);		//To enable clock for GPIO PORTC
	GPIOC->MODER |= (1<<26);	//To set PC13 in Output Mode
	GPIOC->MODER &=~ (1<<27);	//To set PC13 in Output Mode
	GPIOC->ODR |= (1<<13);		//To make LED in OFF state by setting PC13 HIGH as LED is active LOW

	while(1)	//Infinite loop
	{
		uint8_t receivedData = USART_data();	//To get the received data into receivedData variable
		if ((receivedData) == 'A')				//If data is 'A' then switch ON the LED
			GPIOC->ODR &=~ (1<<13);
		else if ((receivedData) == 'B')			//If data is 'B' then switch OFF the LED
			GPIOC->ODR |= (1<<13);

	}

}

uint8_t USART_data(void)
{
	while(         (      !(   (USART2->SR) & (1<<5)  )    )==1);	//wait till the RXNE bit is set
	return (uint8_t)(USART2->DR & 0xFF);							//Then only read the bits 0-7 and return the data
}
