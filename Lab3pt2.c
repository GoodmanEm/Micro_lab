#include "init.h"
#include "uart.h"


UART_HandleTypeDef UART6;

uint8_t stuff;


// Main Execution Loop
int main(void) {
	printf("\033[2J\033[;H"); // Erase screen & move cursor to home position
	fflush(stdout); // Need to flush stdout after using printf that doesn't end in \n
	//Initialize the system
	Sys_Init();

	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	asm ("nop");
	asm ("nop");

	initUart(&UART6, 38400, USART6);


	HAL_NVIC_EnableIRQ(USART1_IRQn);
	HAL_NVIC_EnableIRQ(USART6_IRQn);

    HAL_UART_Receive_IT (&USB_UART, &stuff, 1);

	HAL_UART_Receive_IT(&UART6, &stuff, 1);
	uart_print(&USB_UART,"\033[2J\033[;H");

	while(1);

	// Lost? Check out uart.c!

}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(stuff == 27){
	    uart_putchar(&UART6, &stuff);
		uart_print(&USB_UART,"\r\nprogram halted hit reset to continue, otherwise just sit back and watch the world burn\r\n");
		while(1);
	}
  if (huart->Instance == USART1)
  {
      uart_putchar(&USB_UART, &stuff);
      uart_putchar(&UART6, &stuff);
  }
  if (huart->Instance == USART6)
  {
      uart_putchar(&USB_UART, &stuff);
      uart_putchar(&UART6, &stuff);

  }
}


// Handle USB/UART Interrupts with HAL
void USART1_IRQHandler() {
	HAL_UART_IRQHandler(&USB_UART);
    HAL_UART_Receive_IT (&USB_UART, &stuff, 1);
}

void USART6_IRQHandler() {
	HAL_UART_IRQHandler(&UART6);
    HAL_UART_Receive_IT (&UART6, &stuff, 1);

}

