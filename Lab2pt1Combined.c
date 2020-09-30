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


    GPIO_InitTypeDef pin_config;

    //HAL_NVIC_EnableIRQ();
    pin_config.Pin = GPIO_PIN_0;
    pin_config.Mode = GPIO_MODE_IT_RISING;
    pin_config.Pull = GPIO_PULLUP;

    HAL_GPIO_Init(GPIOJ, &pin_config);

    //D5
	GPIOC->MODER &= ~GPIO_MODER_MODER8_0;
	GPIOC->MODER &= ~GPIO_MODER_MODER8_1;
	//set as pull down
    GPIOC->PUPDR |= GPIO_PUPDR_PUPDR8_0;
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR8_1;


    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

//	//EXTI0
//	NVIC_EnableIRQ(EXTI0_IRQn);
//	//setting pin
//	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PJ;
//	//unmask
//	EXTI->IMR |= EXTI_IMR_MR0;
//	//make trigger on rising edge
//	EXTI->RTSR |= EXTI_RTSR_TR0;

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

void EXTI0_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);	//timer interupt
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_PIN){
	if (GPIO_PIN == GPIO_PIN_0) {
		printf("if 0\r\n");

	}
}
//void EXTI0_IRQHandler(){
//		EXTI->PR |= EXTI_PR_PR0;
//		printf("EXTI0 Triggered\r\n");
//
//
//}

void EXTI9_5_IRQHandler(){
	EXTI->PR |= EXTI_PR_PR8;
	printf("EXTI8 Triggered\r\n");
}
