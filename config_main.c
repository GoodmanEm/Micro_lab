#include "stm32f769xx.h"
#include "init.h"
#include<stdint.h>

/*
 * (4) 10k ohm resistors for switches
 * (blue) 100 ohm
 * (red & yellow) 220 Ohm
 * (green) 330 ohm
 * V = 5
 */

//pins to use
//PJ0 D4
//PC8 D5

int main(void){
	Sys_Init();
	init_pins();

	// Read the README in the base directory of this project.

	printf("\033[2J\033[;H");

	while (1){
		HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_6, GPIO_PIN_SET);
		printf("testing\r\n");
		HAL_Delay(500);
		HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_6, GPIO_PIN_RESET);

	}
}


void init_pins(void){
    GPIO_InitTypeDef pins_config;
    GPIO_InitTypeDef LEDS_config;

	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOJ_CLK_ENABLE();	//INPUTS: PJ0 -> D4	// PJ1 -> D2	// PJ3 -> D7	// PJ4 -> D8

	__HAL_RCC_SYSCFG_CLK_ENABLE();

	asm ("nop");
	asm ("nop");

	//sets 4 GPIO inputs
	pins_config.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_4;
	pins_config.Mode = GPIO_MODE_INPUT;
    pins_config.Pull = GPIO_PULLDOWN;

    HAL_GPIO_Init(GPIOJ, &pins_config);

    //TIM12_CH1,	TIM3_CH3, TIM11_CH1,	TIM12_CH1
	//sets 4 outputs		//PF6 ->D3 // PC8 -> D5 // PF7 -> D6	//PH6 -> D9

    LEDS_config.Pin = GPIO_PIN_8;
    LEDS_config.Mode = GPIO_MODE_OUTPUT_PP;
    LEDS_config.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOC, &LEDS_config);

    LEDS_config.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    LEDS_config.Mode = GPIO_MODE_OUTPUT_PP;
    LEDS_config.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOF, &LEDS_config);

    LEDS_config.Pin = GPIO_PIN_6;
    LEDS_config.Mode = GPIO_MODE_OUTPUT_PP;
    LEDS_config.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOH, &LEDS_config);
}
/*
void EXTI0_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);	//timer interupt
}

void EXTI9_5_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
}
*/
