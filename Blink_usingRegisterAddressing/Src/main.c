#define RCC *(volatile unsigned int *)0x40023830  // For enabling clock to GPIOC
#define GPIO_baseAddress *(volatile unsigned int *)0x40020800	 // For setting mode of pin 13 as output and controlling pin 13
#define GPIOC_MODER GPIO_baseAddress
#define GPIOC_ODR *(volatile unsigned int *)0x40020814

int main(void)
{
	RCC |= (1<<2);
	GPIOC_MODER |= (1<<26);
	GPIOC_MODER &= ~(1<<27);
	while(1)
	{
		GPIOC_ODR |= (1<<13);
		for(int i=0; i<1000000; i++){}
		GPIOC_ODR &= ~(1<<13);
		for(int i=0; i<1000000; i++){}
	}
}
