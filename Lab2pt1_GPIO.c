#include "init.h"


//EXTI0
//EXTI9_5


//pins to use
//PJ0 D4
//PC8 D5

int main(void)
{

    Sys_Init();



    printf("\033[2J\033[;H"); // Erase screen & move cursor to home position
    fflush(stdout); // Need to flush stdout after using printf that doesn't end in \n

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOJEN;

	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	asm ("nop");
	asm ("nop");



    //D4
    GPIOJ->MODER &= ~GPIO_MODER_MODER0_0;
    GPIOJ->MODER &= ~GPIO_MODER_MODER0_1;
    //set as pull down
    GPIOJ->PUPDR |= GPIO_PUPDR_PUPDR0_0;
    GPIOJ->PUPDR &= ~GPIO_PUPDR_PUPDR0_1;

    //D5
	GPIOC->MODER &= ~GPIO_MODER_MODER8_0;
	GPIOC->MODER &= ~GPIO_MODER_MODER8_1;
	//set as pull down
    GPIOC->PUPDR |= GPIO_PUPDR_PUPDR8_0;
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR8_1;


	//EXTI0
	NVIC_EnableIRQ(EXTI0_IRQn);
	//setting pin
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PJ;
	//unmask
	EXTI->IMR |= EXTI_IMR_MR0;
	//make trigger on rising edge
	EXTI->RTSR |= EXTI_RTSR_TR0;

	//EXTI8
    NVIC_EnableIRQ(EXTI9_5_IRQn);
	SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI8_PC;
 	EXTI->IMR |= EXTI_IMR_MR8;
	EXTI->RTSR |= EXTI_RTSR_TR8;

    while(1){
    	printf("hello\r\n");
    	HAL_Delay(1000);

    }


}

void EXTI0_IRQHandler(){
		EXTI->PR |= EXTI_PR_PR0;
		printf("EXTI0 Triggered\r\n");


}

void EXTI9_5_IRQHandler(){
	EXTI->PR |= EXTI_PR_PR8;
	printf("EXTI8 Triggered\r\n");
}
