#include "init.h"


volatile uint32_t elapsed = 0;
float temp_float = 0;

int main(){
    Sys_Init();



	printf("\033[2J\033[;H"); // Erase screen & move cursor to home position
	fflush(stdout); // Need to flush stdout after using printf that doesn't end in \n


	// Enable TIM6 clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
	asm ( "nop" );
	asm ( "nop" );

	//enable interupt
	NVIC_EnableIRQ(54);


	// Set pre-scaler to slow down ticks
	TIM6->PSC = 4999;

	//Set the Auto-reload Value for 10Hz overflow
    TIM6->ARR = 2159;

	// Generate update events to auto reload.
    TIM6->EGR  |= 0x1U;

	// Enable Update Interrupts.
	TIM6->DIER |= TIM_DIER_UIE;

	// Start the timer.
	TIM6->CR1 |= TIM_CR1_CEN;
	while(1){
		//printf("hello\r\n");
	   // HAL_Delay(1000);
	}

}

void TIM6_DAC_IRQHandler() {
	// Clear Interrupt Bit
	TIM6->SR &= ~(TIM_SR_UIF);
	elapsed += 1;
	temp_float = (double)elapsed / 10;
	printf("time elapsed %.1f\r\n", temp_float);

	// Other code here:

}
