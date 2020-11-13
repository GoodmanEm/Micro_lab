#include "init.h"
#include "uart.h"

SPI_HandleTypeDef SPI2_handler;
DMA_HandleTypeDef DMA_hand_rx;
DMA_HandleTypeDef tx_DMA_hand;

void DMACallback(DMA_HandleTypeDef *hdma);
void DMA_config();
void configureSPI();

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
	DMA_config();

	while(1){
		rx_value = 0;
		tx_value = getchar();
		printf("1. %c\r\n",tx_value);

		if(tx_value == 0)
			continue;
		else{

			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
			HAL_SPI_TransmitReceive_DMA(&SPI2_handler,(uint8_t *)&tx_value, (uint8_t *)&rx_value,1);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);

			printf("2. %c\r\n",tx_value);

			while(!rx_value);
			printf("3. %c\r\n\r\n",rx_value);
		}
	}
}

void DMA_config(void){

	__HAL_RCC_DMA1_CLK_ENABLE();

	//direction, data size, address incrementation, mode, and FIFO buffers
	DMA_hand_rx.Instance = DMA1_Stream3;
	DMA_hand_rx.Init.Channel = DMA_CHANNEL_0;

	tx_DMA_hand.Instance = DMA1_Stream4;
	tx_DMA_hand.Init.Channel = DMA_CHANNEL_0;

	tx_DMA_hand.Init.Direction = DMA_PERIPH_TO_MEMORY;
	DMA_hand_rx.Init.Direction = DMA_MEMORY_TO_PERIPH;

	DMA_hand_rx.Init.PeriphInc = DMA_PINC_DISABLE;
	DMA_hand_rx.Init.MemInc = DMA_MINC_ENABLE;
	tx_DMA_hand.Init.PeriphInc = DMA_PINC_DISABLE;
	tx_DMA_hand.Init.MemInc = DMA_MINC_ENABLE;

	DMA_hand_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	DMA_hand_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	DMA_hand_rx.Init.Mode = DMA_NORMAL;
	tx_DMA_hand.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	tx_DMA_hand.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	tx_DMA_hand.Init.Mode = DMA_NORMAL;

	DMA_hand_rx.Init.Priority = DMA_PRIORITY_HIGH;
	tx_DMA_hand.Init.Priority = DMA_PRIORITY_HIGH;

	DMA_hand_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	tx_DMA_hand.Init.FIFOMode = DMA_FIFOMODE_DISABLE;

	HAL_DMA_Init(&tx_DMA_hand);
	HAL_DMA_Init(&DMA_hand_rx);


	HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 0x1, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);

	HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);

	// curious about this
	//HAL_DMA_Start(&DMA_hand_rx, tx_value , &tx_DMA_hand, 1);
	//HAL_DMA_Start(&tx_DMA_hand, rx_value , &DMA_hand_rx, 1);

}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef * hspi){

}

void DMA1_Stream3_IRQHandler(void) {

	HAL_DMA_IRQHandler(&DMA_hand_rx); //This will automatically call the HAL_UART_RxCpltCallback()
}

void DMA1_Stream4_IRQHandler(void) {

	HAL_DMA_IRQHandler(&tx_DMA_hand); //This will automatically call the HAL_UART_RxCpltCallback()
}

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

		//__HAL_LINKDMA(__HANDLE__, __PPP_DMA_FIELD__, __DMA_HANDLE__) also (or hdmarx)
		__HAL_LINKDMA(hspi, hdmarx , DMA_hand_rx); //HERE?
		__HAL_LINKDMA(hspi, hdmatx , tx_DMA_hand); //HERE?

	}
}
