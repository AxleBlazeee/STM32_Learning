#define RCC *(volatile unsigned int *)0x40023830  		    // For enabling clock to GPIOC
#define GPIOC_MODER *(volatile unsigned int *)0x40020800	// For setting mode of pin13 as output
#define GPIOC_ODR *(volatile unsigned int *)0x40020814		// For controlling pin13 output

int main(void)
{
	RCC |= (1<<2);               //Enables clock to GPIOC
	GPIOC_MODER |= (1<<26);      //To set pin13 in OUTPUT mode
	GPIOC_MODER &= ~(1<<27);     //To set pin13 in OUTPUT mode
	while(1)
	{
		GPIOC_ODR |= (1<<13);			//To make pin13 state HIGH
		for(int i=0; i<100000; i++){}	//To give some delay
		GPIOC_ODR &= ~(1<<13);			//To make pin13 state LOW
		for(int i=0; i<100000; i++){}	//To give some delay
	}
}
