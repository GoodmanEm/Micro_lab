//------------------------------------
// Lab 3 - Part 4: UART - Lab03-4_uart.c
//------------------------------------
//

#include "init.h"
#include "uart.h"

//UART_HandleTypeDef spi6;

SPI_HandleTypeDef SPI2_handler;

char Letter1_2[1];
char Letter2_1[1];
char choice;

int main(void) {

	Sys_Init();

	configureSPI();

	//2. Read the slave’s firmware version upon startup.
	// V_MAJ (Reg#: 0) and V_MIN (Reg#: 1):

	HAL_SPI_Receive(&SPI2_handler,(uint8_t*)Letter1_2, 1, 10);
	uart_print(&USB_UART,"type esc on main terminal for options\r\n");

	while (1){

		//CTK_REG (Reg 2)  RDTMP (Bit 1) & ( set ULKDID (bit 7) to 1) & RST (bit 0)
		//TMP_AVG (Reg 4)
		//TMP_LO (Reg 5) & TMP_HI (Reg 6)
		//DEVID (9) (Reset Value: 0x1a)

		HAL_UART_Receive(&USB_UART,(uint8_t*)Letter2_1, 1, 10);
		choice = 0;
/*		6. Pressing ESC in the master’s terminal should present a menu where the user can trigger items 2-5.
		When this menu is active, it is acceptable to ignore characters inputted from the slave’s terminal.*/
		if ((int)Letter2_1[0] == 27) {	//if esc in master terminal
			uart_print(&USB_UART, "type 1 for _____, 2 for ______, 3 for ____, 4 for _____, and 5 for ______\r\n");

			while(1){
				if (!(HAL_UART_Receive(&USB_UART,(uint8_t*)Letter2_1, 1, 10))) {
					choice = Letter2_1[0];
				}
				if (choice == 0) {
					continue;
				}
				else {
					break;
				}
			}
/*1.Reliably send and receive terminal characters to and from the slave device. The received characters
 should be printed on the bottom half of the master’s terminal, respectively.*/
			if (choice == '1'){
				uart_print(&USB_UART,"choice 1 selected.\r\n");

				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
				if (!(HAL_SPI_Receive(&SPI2_handler,(uint8_t*)Letter1_2, 1, 10))) {

					HAL_UART_Transmit(&USB_UART,(uint8_t*)Letter1_2, strlen(Letter1_2), 10);
					uart_print(&USB_UART,"\r\n");
				}

				else if (!(HAL_UART_Receive(&USB_UART,(uint8_t*)Letter2_1, 1, 10))) {

					HAL_SPI_Transmit(&SPI2_handler,(uint8_t*)Letter2_1, strlen(Letter2_1), 10);
				}
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);

			}

// 2. Read the slave’s firmware version upon startup.
			else if (choice == '2'){
				uart_print(&USB_UART,"choice 2 selected.\r\n");

			}

/*3. Trigger a temperature measurement and retrieve the result when it is ready. The temperature should
be printed on the right side of the terminal to avoid the transmitted and received terminal characters.*/
			else if (choice == '3'){
				uart_print(&USB_UART,"choice 3 selected.\r\n");

			}

// 4. Change the device ID of the slave.
			else if (choice == '4'){
				uart_print(&USB_UART,"choice 4 selected.\r\n");

			}

// 5. Clear or reset the slave terminal.
			else if (choice == '5'){
				uart_print(&USB_UART,"choice 5 selected.\r\n");

			}

			else {
				uart_print(&USB_UART,"no choice selected.\r\n");
			}
		}
	}
}
// Handle USB/UART Interrupts with HAL
void USART1_IRQHandler() {
	HAL_UART_IRQHandler(&USB_UART);
    HAL_UART_Receive_IT (&USB_UART, (uint8_t*)Letter2_1, 1);

}

//from lab partner
void configureSPI() {
	__SPI2_CLK_ENABLE();

	SPI2_handler.Instance = SPI2; // Please use SPI2!
	SPI2_handler.Init.Mode = SPI_MODE_MASTER; // Set master mode
	SPI2_handler.Init.TIMode = SPI_TIMODE_DISABLE; // Use Motorola mode, not TI mode
	SPI2_handler.Init.DataSize = SPI_DATASIZE_8BIT;
	SPI2_handler.Init.Direction = SPI_DIRECTION_2LINES;
	SPI2_handler.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
	SPI2_handler.Init.BaudRatePrescaler= SPI_BAUDRATEPRESCALER_256;
	SPI2_handler.Init.CLKPhase = SPI_PHASE_1EDGE;
	SPI2_handler.Init.CLKPolarity = SPI_POLARITY_LOW;
	SPI2_handler.Init.NSS = SPI_NSS_SOFT;
	SPI2_handler.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
	SPI2_handler.Init.FirstBit = SPI_FIRSTBIT_MSB;


	HAL_SPI_Init(&SPI2_handler);
	//
	// Note: HAL_StatusTypeDef HAL_SPI_Init(SPI_HandleTypeDef *hspi)
	//
	// HAL_SPI_Init() will call HAL_SPI_MspInit() after reading the properties of
	// the passed SPI_HandleTypeDef. After finishing the MspInit call, it will set
	// the SPI property bits. This is how all HAL_[peripheral]_Init() functions work.
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
	// SPI GPIO initialization structure here
    GPIO_InitTypeDef  GPIO_init;

	if (hspi->Instance == SPI2)
	{
	    __GPIOB_CLK_ENABLE();
	    __GPIOA_CLK_ENABLE();

	    GPIO_init.Pin =GPIO_PIN_15 | GPIO_PIN_14 | GPIO_PIN_11 | GPIO_PIN_12;
		GPIO_init.Mode = GPIO_MODE_AF_PP;
		GPIO_init.Pull = GPIO_PULLUP;
		GPIO_init.Speed = GPIO_SPEED_HIGH;
		GPIO_init.Alternate = GPIO_AF5_SPI2;

	    HAL_GPIO_Init(GPIOB, &GPIO_init);
	    GPIO_init.Pin = GPIO_PIN_11 | GPIO_PIN_12;

	    HAL_GPIO_Init(GPIOA, &GPIO_init);

		// Enable SPI GPIO port clocks, set HAL GPIO init structure's values for each
		// SPI-related port pin (SPI port pin configuration), enable SPI IRQs (if applicable), etc.
	}
}
