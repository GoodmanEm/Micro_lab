/*
 * lab2_1.c
 *
 *  Created on: Sep 20, 2020
 *      Author: goodme
 */

#include "init.h"

//------------------------------------------------------------------------------------
// MAIN Routine
//------------------------------------------------------------------------------------
int main(void)
{
// two External Interrupts via the signals EXTI0 and EXTI8. Both
// external sources for EXTI0 and EXTI8 should be Analog Discovery
//	pushbuttons as connected in Lab 1
    Sys_Init();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOJ_CLK_ENABLE();

    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    // or __HAL_RCC_SYSCFG_CLK_ENABLE();
    // Delay after an RCC peripheral clock enabling
    asm ("nop");
   	asm ("nop");

    GPIO_InitTypeDef pin_config;

    //HAL_NVIC_EnableIRQ();
    pin_config.Pin = GPIO_PIN_7 | GPIO_PIN_0;
    pin_config.Mode = GPIO_MODE_INPUT;
    pin_config.Pull = GPIO_PULLDOWN;

    HAL_GPIO_Init(GPIOC, &pin_config);
    HAL_GPIO_Init(GPIOJ, &pin_config);

    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    printf("\033[2J\033[;H"); // Erase screen & move cursor to home position
	fflush(stdout); // Need to flush stdout after using printf that doesn't end in \n

	while (1){
		printf("hello\n\r");
		HAL_Delay(1000);
    }
}

//HAL_NVIC_EnableIRQ()


void EXTI0_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);	//timer interupt
}

void EXTI9_5_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_PIN){
	if (GPIO_PIN == GPIO_PIN_0) {
		printf("if 0/n/r");

	}
	else if (GPIO_PIN == GPIO_PIN_7) {
		printf("if 8/n/r");

	}
}
/*
void HAL_EXTI9_5_ConvCpItCallback(EXTI9_5_HandleTypeDef * hexti9_5_){

}
void HAL_EXTI0_ConvCpItCallback(EXTI0_HandleTypeDef * hexti0_){}

*/
