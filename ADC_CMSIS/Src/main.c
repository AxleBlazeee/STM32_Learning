/*
 * Project     : ADC to UART Transmitter
 * Target MCU  : STM32F401CCU6 (Black Pill) / STM32F4xx Series
 * Description : Bare-Metal firmware that reads an analog voltage on PA4
 *               (ADC1_IN4) using the on-chip 12-bit ADC configured for
 *               8-bit resolution, and continuously transmits the converted
 *               value over USART2 (PA2 - TX pin) via direct register
 *               manipulation. No HAL. No STM32CubeMX. Pure bare-metal C.
 * ADC Config  : ADC1 | Channel 4 (PA4) | 8-bit resolution | Continuous mode
 * UART Config : USART2 | Baud Rate: 9600 | 8-bit data | No Parity | 1 Stop Bit
 * Author      : Abhishek Rawat
 */

#include "stm32f4xx.h"

//////		 For USART	 	/////////////

#define USART2_UE_Set (1<<13)
#define USART2_BRR_Value ((104<<4) | 3)
#define USART2_TE_Set (1<<3)
#define USART2_RCC_Enable (1<<17)
#define GPIOA_RCC_Enable (1<<0)
#define GPIOA_MODER_AFM (1<<5)
#define GPIOA_AFRL_AF07 ((1<<8) | (1<<9) | (1<<10))		//AF07 for PA2 (USART2 TX)
#define USART2_TXE (1<<7)



//////		For ADC			/////////////

#define ADC1_RCC_Enable (1<<8)
#define GPIOA_MODER_ADC ((1<<8) | (1<<9))
#define ADC1_ADCON (1<<0)
#define ADC1_RES (1<<25)
#define ADC1_CONTMODE (1<<1)
#define ADC1_CHANNEL (4<<0)
#define ADC1_STARTCONV (1<<30)
#define ADC1_SR_EOC (1<<1)



//////		 Data to transmit	 	/////////////

static uint8_t adc_value;


int main()
{
	///////////////		For UART		///////////////////////////////

	RCC->APB1ENR |= USART2_RCC_Enable;	//To enable clock for USART2
	RCC->AHB1ENR |= GPIOA_RCC_Enable;	//To enable clock for GPIO PORTA

	GPIOA->MODER |= GPIOA_MODER_AFM;	//To set PA2 pin in Alternate Function Mode
	GPIOA->AFR[0] |= GPIOA_AFRL_AF07;	//To set PA2 into UART pin

	USART2->CR1 |= USART2_UE_Set;		//To enable USART
	USART2->BRR = USART2_BRR_Value;		//To set baud rate of 9600
	USART2->CR1 |= USART2_TE_Set;		//Transmitter enable



	///////////////		For ADC			//////////////////////////////

	RCC->APB2ENR |= ADC1_RCC_Enable;	//To enable clock for ADC

	GPIOA->MODER |= GPIOA_MODER_ADC;	//To set PA4 pin in Analog Mode


	ADC1->CR1 |= ADC1_RES;				//To set the ADC resolution to 8 bit
	ADC1->CR2 |= ADC1_CONTMODE;			//To set ADC in Continuous Mode
	ADC1->SQR3 |= ADC1_CHANNEL;			//To set the slot1 with channel 4 ADC1_IN4
	ADC1->CR2 |= ADC1_ADCON;			//To switch ON the ADC
	ADC1->CR2 |= ADC1_STARTCONV;		//To start the ADC Conversion


	while(1)	//Infinite loop
	{

		///////////////////////		To send the ADC value as a character		/////////////////////
		// (Uncomment this block and comment the RAW byte block below to see readable digits like "255")


//		while(!(ADC1->SR & (1<<1)));
//		adc_value = ADC1->DR & 0xFF;
//
//		// hundreds digit
//		while(!(USART2->SR & USART2_TXE));
//		USART2->DR = (adc_value / 100) + '0';
//
//		// tens digit
//		while(!(USART2->SR & USART2_TXE));
//		USART2->DR = ((adc_value / 10) % 10) + '0';
//
//		// units digit
//		while(!(USART2->SR & USART2_TXE));
//		USART2->DR = (adc_value % 10) + '0';
//
//		// carriage return
//		while(!(USART2->SR & USART2_TXE));
//		USART2->DR = '\r';
//
//		// newline
//		while(!(USART2->SR & USART2_TXE));
//		USART2->DR = '\n';
//
//		for(int i=0; i<100000; i++);   // delay so it's readable




		///////////////////////		To send the ADC value as a RAW byte		/////////////////////


		///////////////////		For ADC		////////////////////////////////

		while(!(ADC1->SR & ADC1_SR_EOC));		//Wait until EOC bit is set after ADC conversion is complete
		/*
		 * Read the ADC result and keep only the lowest 8 bits.
		 *
		 * ADC1->DR is a 16-bit register, but I set the ADC to 8-bit resolution,
		 * so the result is always 0-255 and already fits in the low byte.
		 * The "& 0xFF" masks off the upper bits, keeping just one byte -
		 * which is all UART can send at a time.
		 *
		 * Note: This mask is safe ONLY because I'm in 8-bit mode. In 12-bit
		 * mode (0-4095) it would discard the top 4 bits and corrupt the value.
		 */
		adc_value = ADC1->DR & 0xFF;

		///////////////////		For UART		//////////////////////////////

		while (!(USART2->SR & USART2_TXE));		//Wait until TXE bit is set (TX buffer is empty and ready for new data)
		USART2->DR = (adc_value)-12;				//Add the value to transmit via UART (ex.- 255 for 3.3V) (sends the RAW byte, not digits)
		while (!(USART2->SR & USART2_TXE));
		USART2->DR = '\r';
		while (!(USART2->SR & USART2_TXE));
		USART2->DR = '\n';
		for(int i=0; i<100000; i++);				//To provide some random delay
	}
}
