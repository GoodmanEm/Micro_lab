//------------------------------------
// Lab 3 - Part 1: UART - Lab03_uart.c
//------------------------------------
//

#include "init.h"
// Main Execution Loop
//UART_HandleTypeDef huart1;
UART_HandleTypeDef huart6;
char Letter1_6[1];
char Letter6_1[1];

int main(void) {
	//Initialize the system
	Sys_Init();



	SystemClock_Config();


	/* Enable GPIO port for transmitting and receiving
	Populating the UART_HandleTypeDef fields Instance and Init.*/

	initUart(&huart6, 38400, USART6);

	printf("Ready to Start\r\n");

	while (1){
		//Words[1] = HAL_UART_RECIEVE();
		if (!(HAL_UART_Receive(&huart6,(uint8_t*)Letter1_6, 1, 10))) {

		HAL_UART_Transmit(&USB_UART,(uint8_t*)Letter1_6, strlen(Letter1_6), 10);
		printf("\r\n");

		}
		if (!(HAL_UART_Receive(&USB_UART,(uint8_t*)Letter6_1, 1, 10))) {

			HAL_UART_Transmit(&huart6,(uint8_t*)Letter6_1, strlen(Letter6_1), 10);

		}
		if (((int)Letter6_1[0] == 27) || ((int)Letter1_6[0] == 27) ) {	//if esc
			//printf("Goodbye\r\n");
			break;
		}
		else {
			//printf("hello\r\n");
		}

	}
}
// Handle USB/UART Interrupts with HAL
void USART1_IRQHandler() {
	HAL_UART_IRQHandler(&USB_UART);
    HAL_UART_Receive_IT (&USB_UART, (uint8_t*)Letter6_1, 1);

}

void USART6_IRQHandler() {
	HAL_UART_IRQHandler(&huart6);
    HAL_UART_Receive_IT (&huart6, (uint8_t*)Letter1_6, 1);
}
