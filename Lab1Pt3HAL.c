#include "stm32f769xx.h"
#include "init.h"
#include "hello.h"
#include<stdint.h>
//#include "stm32f7xx_hal.h"



int main(void){

	Sys_Init();


    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOJ_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();


    //input set up
    GPIO_InitTypeDef pin_config;
    // D0 PC7
    // D1 PC6
    // D2 PJ1
    // D3 PF6
    pin_config.Pin = GPIO_PIN_7 | GPIO_PIN_6 | GPIO_PIN_1;
    pin_config.Mode = GPIO_MODE_INPUT;
    pin_config.Pull = GPIO_PULLUP;

    /*
    HAL_GPIO_Init(GPIOC, &pin_config);
    HAL_GPIO_Init(GPIOJ, &pin_config);
    HAL_GPIO_Init(GPIOF, &pin_config);
    */

    //need to mess around with J to go to the LEDs and mess with GPIO_PIN_6

    // output set up
    GPIO_InitTypeDef LED_config;
    LED_config.Pin = GPIO_PIN_13 | GPIO_PIN_5 | GPIO_PIN_12 |GPIO_PIN_4;
    LED_config.Mode = GPIO_MODE_OUTPUT_PP;
    LED_config.Pull = GPIO_PULLDOWN;

    /*
    HAL_GPIO_Init(GPIOJ, &LED_config);
    HAL_GPIO_Init(GPIOA, &LED_config);
    HAL_GPIO_Init(GPIOD, &LED_config);
    */

    /*
    LD1 RED USER1 PJ13
    LD2 GREEN USER2 PJ5
    LD3 GREEN Arduino PA12
    LD4 RED OTG overcurrent PD4
	*/
    //initialize all Inputs
	 HAL_GPIO_Init(GPIOC, &pin_config);
	 HAL_GPIO_Init(GPIOJ, &pin_config);
	 HAL_GPIO_Init(GPIOF, &pin_config);
	 HAL_GPIO_Init(GPIOJ, &LED_config);
	 HAL_GPIO_Init(GPIOA, &LED_config);
	 HAL_GPIO_Init(GPIOD, &LED_config);
	while(1){


		// if D0 then send LD1 high
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7)){
			// Turn on LED 1
			HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_13, GPIO_PIN_SET);    // Turn green LED on (GPIO_PIN_SET == 1)
		}
		else{ //else do same as turning on but off this time
			HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_13, GPIO_PIN_RESET);    // Turn green LED on (GPIO_PIN_RESET == 0)
		}

		// if D1 then send LD2 high
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6)){
			// Turn on LED 2
			HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_5, GPIO_PIN_SET);
		}
		else{ //else do same as turning on but off this time
			HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_5, GPIO_PIN_RESET);
		}

		// if D2 then send LD3 high
		if(HAL_GPIO_ReadPin(GPIOJ, GPIO_PIN_1)){
			// Turn on LED 3
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
		}
		else{ //else do same as turning on but off this time
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
		}

		// if D3 then send LD4 low
		if(HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_6)){
			// Turn on LED 4
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_RESET);
		}
		else{ //else do same as turning on but off this time
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET);
		}

	}

}
