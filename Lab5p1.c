#include "init.h" // Always need init.h, otherwise nothing will work.
#include<stdint.h>
#include<stdlib.h>

 uint8_t CinitBuffer[1000];
 uint8_t CdestBuffer[1000];


 uint8_t initBuffer[1000];
 uint8_t destBuffer[1000];
 DMA_HandleTypeDef DMA2_Handler;

 void DMACallback(DMA_HandleTypeDef *hdma);
 uint32_t cycles =0;

int main(void)
{
	Sys_Init();
	HAL_Init();
	__DMA2_CLK_ENABLE();
	int i = 0;
	for (i = 0; i < 999; ++i)
	{
		CinitBuffer[i]= i;
	}
	for (i = 0; i < 999; ++i)
	{
		initBuffer[i]= i;
	}

	printf("\033[0m\033[44;33m\033[2J\033[;H"); // Erase screen & move cursor to home position
	fflush(stdout);

	// Enable the DWT_CYCCNT register
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->LAR = 0xC5ACCE55;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;


	DWT->CYCCNT = 0; // Clear the cycle counter
	//do in for loop
	for (i = 0; i < 999; ++i)
	{
		CdestBuffer[i]= CinitBuffer[i];
	}

	//memcpy(CdestBuffer, CinitBuffer, sizeof(CdestBuffer));

	cycles = DWT->CYCCNT; // Store the cycle counter

	//DWT->CYCCNT = 0; // Clear the cycle counter
	printf("Setting the array in C took %lu CPU cycles\r\n",cycles);
	fflush(stdout);
//	cycles = DWT->CYCCNT; // Store the cycle counter


	//printf("And printing the above took %lu CPU cycles\r\n",cycles);
	//fflush(stdout);


	 DMA2_Handler.Instance = DMA2_Stream0;
	 DMA2_Handler.XferCpltCallback = &DMACallback;

	 DMA2_Handler.Init.Channel = DMA_CHANNEL_0;
	 DMA2_Handler.Init.Direction = DMA_MEMORY_TO_MEMORY;
	 DMA2_Handler.Init.PeriphInc = DMA_PINC_ENABLE;
	 DMA2_Handler.Init.MemInc = DMA_MINC_ENABLE;
	 DMA2_Handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	 DMA2_Handler.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	 DMA2_Handler.Init.Mode = DMA_NORMAL;
	 DMA2_Handler.Init.Priority = DMA_PRIORITY_LOW;
	 DMA2_Handler.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	 DMA2_Handler.Init.MemBurst = DMA_MBURST_SINGLE;
	 DMA2_Handler.Init.PeriphBurst = DMA_MBURST_SINGLE;
	 HAL_DMA_Init(&DMA2_Handler);

	 HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
	 HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);


	 DWT->CYCCNT = 0; // Clear the cycle counter
	 HAL_DMA_Start_IT(&DMA2_Handler, (uint32_t)&initBuffer, (uint32_t)&destBuffer, 1000);


	while(1);
}

//This callback is automatically called by the HAL when the DMA transfer is completed
void DMACallback(DMA_HandleTypeDef *hdma) {

	if(hdma->Instance == DMA2_Stream0) {
		cycles = DWT->CYCCNT; // Store the cycle counter
		printf("DMA took %lu CPU cycles\r\n",cycles);
		fflush(stdout);

	}

}


void DMA2_Stream0_IRQHandler(void) {

	HAL_DMA_IRQHandler(&DMA2_Handler); //This will automatically call the HAL_UART_RxCpltCallback()


}

