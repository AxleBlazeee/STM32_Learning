#define RCC *(volatile unsigned int *)0x40023830  		    // For enabling clock to GPIOC
#define GPIOC_MODER *(volatile unsigned int *)0x40020800	// For setting mode of pin13 as output
#define GPIOC_ODR *(volatile unsigned int *)0x40020814		// For controlling pin13 output

int main(void)
{
	RCC |= (1<<2);
	GPIOC_MODER |= (1<<26);
	GPIOC_MODER &= ~(1<<27);
	while(1)
	{
		GPIOC_ODR |= (1<<13);
		for(int i=0; i<100000; i++){}
		GPIOC_ODR &= ~(1<<13);
		for(int i=0; i<100000; i++){}
	}
}
