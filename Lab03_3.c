//----------------------------------
// Lab 3 - Part 3: SPI - Lab03_spi.c
//----------------------------------
//

#include "init.h"
#include "uart.h"

// If needed:
//#include <stdio.h>
//#include <stdlib.h>

/*
 * For convenience, configure the SPI handler here
 */
SPI_HandleTypeDef SPI2_handler;


// See 769 Description of HAL drivers.pdf, Ch. 58.1 or stm32f7xx_hal_spi.c
void configureSPI()
{
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

/*
 * This is called upon SPI initialization. It handles the configuration
 * of the GPIO pins for SPI.
 */
 // Do NOT change the name of an MspInit function; it needs to override a
 // __weak function of the same name. It does not need a prototype in the header.
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
char tx_value;
char rx_value;

int main(void)
{
	Sys_Init();

	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	asm ("nop");
	asm ("nop");
	uart_print(&USB_UART,"\033[2J\033[;H");

	// For convenience
	configureSPI();

	while(1){

		tx_value = 0;
		HAL_UART_Receive(&USB_UART,(uint8_t *)&tx_value,1,10);

		if(tx_value== 0)
			continue;
		else{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
			HAL_SPI_TransmitReceive(&SPI2_handler,(uint8_t *)&tx_value, (uint8_t *)&rx_value,1,10);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);


			printf("\033[1;0H%c\r\n",tx_value);
			printf("\033[12;0H%c\r\n",rx_value);
		}
	}
// See 769 Description of HAL drivers.pdf, Ch. 58.2.3 or stm32f7xx_hal_spi.c
//
//	HAL_StatusTypeDef HAL_SPI_TransmitReceive(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size, uint32_t Timeout)
//
}
