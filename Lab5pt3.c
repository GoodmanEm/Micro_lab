#include "init.h"
#include "uart.h"

DAC_HandleTypeDef hdac;
ADC_HandleTypeDef hadc;
DMA_HandleTypeDef hdma_adc1;



static void DAC_Init(void);
void configureADC();
void configureDMA();
void DMACallback(DMA_HandleTypeDef *hdma);



float value;
double raw_temp = 0;
uint32_t raw_value = 0;
double raw_temp_minus1 = 0;
double raw_temp_minus2 = 0;





// Main Execution Loop
int main(void)
{
	double N_dac = 0;
	double prev_N_dac = 0;
	double a = 0.312500;
	double b = 0.240385;
	double c = 0.312500;
	double d = 0.296875;

	//x(k) is ADC

	//current ADC
	//most recent ADC
	//Second most recent ADC


	//Initialize the system
	Sys_Init();

	__HAL_RCC_GPIOA_CLK_ENABLE();


	HAL_DAC_MspInit(&hdac);

	configureADC();
	HAL_ADC_Start(&hadc);

	DAC_Init();
	HAL_DAC_Init(&hdac);
	HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
	configureDMA();

	printf("\033[2J\033[;H");

	while (1) {

		raw_temp_minus2 = raw_temp_minus1;
		raw_temp_minus1 = raw_temp;

		//HAL_ADC_PollForConversion(&hadc, 1);
		//raw_temp = HAL_ADC_GetValue(&hadc);

		HAL_ADC_Start_DMA(&hadc, &raw_value, 1);//start the DMA collecting the data

		raw_temp = raw_value;

		prev_N_dac = N_dac;
		N_dac = 0;
//		N_dac = (0.312500 * raw_temp)+ (0.240385 * raw_temp_minus1)
//				+ (0.312500 * raw_temp_minus2) + (0.296875 * prev_N_dac);

		asm volatile ("VMLA.F64 %P[dest], %P[a], %P[k]"
				: [dest] "+w" (N_dac)
				: [a] "w" (a),[k] "w" (raw_temp));

		asm volatile ("VMLA.F64 %P[dest], %P[b], %P[k1]"
				:[dest] "+w" (N_dac)
				: [b] "w" (b),[k1] "w" (raw_temp_minus1));

		asm volatile ("VMLA.F64 %P[dest], %P[c], %P[k2]"
				: [dest] "+w" (N_dac)
				:   [c] "w" (c), [k2] "w" (raw_temp_minus2));

		asm volatile ("VMLA.F64 %P[dest], %P[d], %P[yk]"
				: [dest] "+w" (N_dac)
				:[yk] "w" (prev_N_dac),   [d] "w" (d));



		HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, N_dac);
	}
}
void configureDMA(){
	__DMA2_CLK_ENABLE();




	 hdma_adc1.Instance = DMA2_Stream0;
	 hdma_adc1.XferCpltCallback = &DMACallback;

	 hdma_adc1.Init.Channel = DMA_CHANNEL_0;
	 hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
	 hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
	 hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
	 hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	 hdma_adc1.Init.MemDataAlignment = DMA_PDATAALIGN_WORD;
	 hdma_adc1.Init.Mode = DMA_CIRCULAR;
	 hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;
	 hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	 hdma_adc1.Init.MemBurst = DMA_MBURST_SINGLE;
	 hdma_adc1.Init.PeriphBurst = DMA_MBURST_SINGLE;
	 HAL_DMA_Init(&hdma_adc1);

	 __HAL_LINKDMA(&hadc, DMA_Handle, hdma_adc1);


	 HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
	 HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);


}
void DMACallback(DMA_HandleTypeDef *hdma) {

	if(hdma->Instance == DMA2_Stream0) {

	}

}


void DMA2_Stream0_IRQHandler(void) {

	HAL_DMA_IRQHandler(&hdma_adc1); //This will automatically call the HAL_UART_RxCpltCallback()

}
void DAC_Init(void) {

	DAC_ChannelConfTypeDef sConfig;

	RCC->APB1ENR |= RCC_APB1ENR_DACEN;
		asm ("nop");
		asm ("nop");

	// Initialization
	hdac.Instance = DAC;
	HAL_DAC_Init(&hdac);

	/**DAC channel OUT1 config */
	sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
	HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1);

}

void HAL_DAC_MspInit(DAC_HandleTypeDef *hadc) {

	GPIO_InitTypeDef GPIO_InitStruct;


	/* DAC GPIO Configuration
	PA5 ------> DAC_OUT1 or PA4*/
	GPIO_InitStruct.Pin = GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

// ---------------------------------------------
void configureADC() {
	ADC_ChannelConfTypeDef sConfig;
	// Enable the ADC Clock.
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	asm ("nop");
	asm ("nop");

	HAL_ADC_MspInit(&hadc);


	 hadc.Instance = ADC1;
	 hadc.Init.Resolution = ADC_RESOLUTION_12B;
	 hadc.Init.ScanConvMode = ENABLE;
	 hadc.Init.ContinuousConvMode = DISABLE;
	 hadc.Init.DiscontinuousConvMode = DISABLE;
	 hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	 hadc.Init.NbrOfConversion = 1;
	 hadc.Init.DMAContinuousRequests = ENABLE;
	 hadc.Init.EOCSelection = ADC_EOC_SEQ_CONV;

	HAL_ADC_Init(&hadc); // Initialize the ADC


	// Configure the ADC channel
	sConfig.Channel = ADC_CHANNEL_6;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
	HAL_ADC_ConfigChannel(&hadc, &sConfig);
}

void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc) {
    GPIO_InitTypeDef but_config;
    //Blue button (PA0)
    but_config.Pin = GPIO_PIN_0;
    but_config.Mode = GPIO_MODE_INPUT;
    but_config.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &but_config);


    GPIO_InitTypeDef GPIOInit;
    //ADC PIN A0  (PA6)
    GPIOInit.Pin = GPIO_PIN_6;
    GPIOInit.Mode = GPIO_MODE_ANALOG;
    GPIOInit.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOA, &GPIOInit);
}
