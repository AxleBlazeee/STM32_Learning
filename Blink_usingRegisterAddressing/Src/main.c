#include <stdint.h>

#define RCC (volatile uint32_t *)0x40023830  // For enabling clock to GPIOC
#define GPIO_baseAddress (volatile uint32_t *)0x40020800	 // For setting mode of pin 13 as output and controlling pin 13
#define GPIOC_MODER GPIO_baseAddress
#define GPIOC_ODR (volatile uint32_t *)0x40020814

volatile uint32_t * ptr1 = RCC;
volatile uint32_t * ptr2 = GPIOC_MODER;
volatile uint32_t * ptr3 = GPIOC_ODR;

//#define RCC *(volatile uint32_t *)0x40023830  // For enabling clock to GPIOC
//#define GPIO_baseAddress *(volatile uint32_t *)0x40020800	 // For setting mode of pin 13 as output and controlling pin 13
//#define GPIOC_MODER GPIO_baseAddress
//#define GPIOC_ODR *(volatile uint32_t *)0x40020814

//#define RCC *(volatile unsigned int *)0x40023830  // For enabling clock to GPIOC
//#define GPIO_baseAddress *(volatile unsigned int *)0x40020800	 // For setting mode of pin 13 as output and controlling pin 13
//#define GPIOC_MODER GPIO_baseAddress
//#define GPIOC_ODR *(volatile unsigned int *)0x40020814

int main(void)
{
	*ptr1 |= (1<<2);
	*ptr2 |= (1<<26);
	*ptr2 &= ~(1<<27);
	while(1)
	{
		*ptr3 |= (1<<13);
		for(int i=0; i<100000; i++){}
		*ptr3 &= ~(1<<13);
		for(int i=0; i<100000; i++){}
	}
}

//int main(void)
//{
//	RCC |= (1<<2);
//	GPIOC_MODER |= (1<<26);
//	GPIOC_MODER &= ~(1<<27);
//	while(1)
//	{
//		GPIOC_ODR |= (1<<13);
//		for(int i=0; i<1000000; i++){}
//		GPIOC_ODR &= ~(1<<13);
//		for(int i=0; i<1000000; i++){}
//	}
//}
