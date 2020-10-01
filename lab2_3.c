/*
 * lab2_1.c
 *
 *  Created on: Sep 20, 2020
 *      Author: goodme
 */

#include "init.h"
TIM_HandleTypeDef htim6;

volatile uint32_t tenth_sec = 0;
//------------------------------------------------------------------------------------
// MAIN Routine
//------------------------------------------------------------------------------------
int main(void)
{
    Sys_Init();

    NVIC_EnableIRQ(54);

    htim6.Instance = TIM6;
    htim6.Init.Prescaler = 47999;
    htim6.Init.Period = 249;

    __HAL_RCC_TIM6_CLK_ENABLE();

   	HAL_TIM_Base_Init(&htim6);		//Configure the timer
   	HAL_TIM_Base_Start_IT(&htim6);	//start it

    printf("\033[2J\033[;H"); // Erase screen & move cursor to home position
	fflush(stdout); // Need to flush stdout after using printf that doesn't end in \n


	while (1);

}


void TIM6_DAC_IRQHandler() {	// Pass the control to HAL, which processes the IRQ
	HAL_TIM_IRQHandler(&htim6);
	tenth_sec ++;
	printf("time elapsed in 1/10 sec: %i\n\r", tenth_sec);

}
/*
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	// Callback function contents
	if (htim->Instance == TIM6) {
		// Do stuff pertaining to TIM0 interrupt
	}
}
*/
