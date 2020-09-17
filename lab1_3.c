/*
 * lab1-3.c
 *
 *  Created on: Sep 8, 2020
 *      Author: goodme
 */
#include "stm32f769xx.h"
#include "hello.h"

#include<stdint.h>

//------------------------------------------------------------------------------------
// MAIN Routine
//------------------------------------------------------------------------------------
int main(void)
{
    Sys_Init();

    printf("\033[2J\033[;H"); // Erase screen & move cursor to home position
    fflush(stdout); // Need to flush stdout after using printf that doesn't end in \n


    //PC7 & PC6
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOJEN;


    GPIOC->MODER &= ~GPIO_MODER_MODER6_0;	// button 1
    GPIOC->MODER &= ~GPIO_MODER_MODER6_1;
    GPIOC->BSRR = (uint16_t)GPIO_PIN_6;

    GPIOC->PUPDR |= GPIO_PUPDR_PUPDR6_0;
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR6_1;

//------------------

    GPIOC->MODER &= ~GPIO_MODER_MODER7_0;
    GPIOC->MODER &= ~GPIO_MODER_MODER7_1;	// button 2

    GPIOC->BSRR = (uint16_t)GPIO_PIN_7;


//------------------
    //PJ1 (works)
    GPIOJ->MODER &= ~GPIO_MODER_MODER1_0;
    GPIOJ->MODER &= ~GPIO_MODER_MODER1_1;

    GPIOJ->BSRR = (uint16_t)GPIO_PIN_1;


    //PF6
    GPIOF->MODER &= ~GPIO_MODER_MODER6_0; 	// button 4
    GPIOF->MODER &= ~GPIO_MODER_MODER6_1;

    GPIOF->BSRR = (uint16_t)GPIO_PIN_6;


 //--------------------------------------------------------------- SET LEDS TO OUTPUTS
    GPIOJ->MODER |= GPIO_MODER_MODER13_0;
    GPIOJ->MODER &= ~GPIO_MODER_MODER13_1;

    GPIOJ->BSRR = (uint16_t)GPIO_PIN_13; // LED1

//--------
    GPIOJ->MODER |= GPIO_MODER_MODER5_0;
    GPIOJ->MODER &= ~GPIO_MODER_MODER5_1;

    GPIOJ->BSRR = (uint16_t)GPIO_PIN_5;	 // LED2

//-------------------------

    GPIOA->MODER |= GPIO_MODER_MODER12_0;
    GPIOA->MODER &= ~GPIO_MODER_MODER12_1;

    GPIOA->BSRR = (uint16_t)GPIO_PIN_12; 	// LED3 (works)

//--------------------------------------
    GPIOD->MODER |= GPIO_MODER_MODER4_0;
    GPIOD->MODER &= ~GPIO_MODER_MODER4_1;

    GPIOD->BSRR = (uint16_t)GPIO_PIN_4;	// LED4

//--------------------------------------


//---------------------------------------------------------------

    while (1) {

    	char D1 = ((GPIOC)->IDR & (GPIO_PIN_6));
    	char D0 = ((GPIOC)->IDR & (GPIO_PIN_7));
    	char D2 = ((GPIOJ)->IDR & (GPIO_PIN_1));
    	char D3 = ((GPIOF)->IDR & (GPIO_PIN_6));


		if (D0) {
			GPIOJ->BSRR = (uint16_t)GPIO_PIN_13; // Turn on Green LED (LED2)
		}
		else {
			GPIOJ->BSRR = (uint32_t)GPIO_PIN_13 << 16;
		}

		if (D1) {
			GPIOJ->BSRR = (uint16_t)GPIO_PIN_5;
		}
		else {
			GPIOJ->BSRR = (uint32_t)GPIO_PIN_5 << 16;
		}

		//THIS BASTARD WORKS
		if (D2) {
			GPIOA->BSRR = (uint16_t)GPIO_PIN_12;
		}
		else {
			GPIOA->BSRR = (uint32_t)GPIO_PIN_12 << 16;
		}

		if (D3) {
			GPIOD->BSRR = (uint32_t)GPIO_PIN_4 << 16;
		}
		else {
			GPIOD->BSRR = (uint16_t)GPIO_PIN_4 ;
		}
    }
}
