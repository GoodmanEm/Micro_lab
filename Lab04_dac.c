//--------------------------------
// Lab 4 - Sample - Lab04_sample.c
//--------------------------------
//
//

#include "init.h"
#include "uart.h"

DAC_HandleTypeDef hdac;
ADC_HandleTypeDef hadc1;


static void DAC_Init(void);
void configureADC();

// Main Execution Loop
int main(void)
{
	unsigned int N_dac = 0;
	unsigned int prev_N_dac = 100;
	int V_DAC;
	float V_ref = 3.3;
	const int frequency = 20000;

	// value = (uint16_t) rint((sinf(((2*PI)/SAMPLES)*i)+1)*2048);


	//Initialize the system
	Sys_Init();

	__HAL_RCC_GPIOA_CLK_ENABLE();

	//HAL_DAC_MspInit(&hdac);
	//HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1);
	configureADC();
	HAL_ADC_Start(&hadc1);

	DAC_Init();
	HAL_DAC_Init(&hdac);
	HAL_DAC_Start(&hdac, DAC_CHANNEL_1);

	printf("\033[2J\033[;H");

	while (1) {

		if (N_dac == prev_N_dac){
			N_dac = 0;
			V_DAC = ((V_ref*N_dac)/(4096));
			HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, V_DAC);
			HAL_Delay(1);
			printf("number %u\r\n", V_DAC);

		}
		else {
		prev_N_dac = N_dac;
		N_dac++;
		V_DAC = (V_ref*N_dac)/(2^8);

		// Not sure about 3rd variable
		HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, V_DAC);
		HAL_Delay(1);
		printf("number %u\r\n", V_DAC);
		}
	}
}

void DAC_Init(void) {

	DAC_ChannelConfTypeDef sConfig;

	GPIO_InitTypeDef GPIO_InitStruct;

	// Initialization
	hdac.Instance = DAC;
	HAL_DAC_Init(&hdac);

	/**DAC channel OUT1 config */
	sConfig.DAC_Trigger = DAC_TRIGGER_T6_TRGO;
	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
	HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1);

	/* DAC GPIO Configuration
	PA5 ------> DAC_OUT1 or PA4*/
	GPIO_InitStruct.Pin = GPIO_PIN_5;
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

	HAL_ADC_MspInit(&hadc1);


	 hadc1.Instance = ADC1;
	 hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	 hadc1.Init.ScanConvMode = DISABLE;
	 hadc1.Init.ContinuousConvMode = ENABLE;
	 hadc1.Init.DiscontinuousConvMode = DISABLE;
	 hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	 hadc1.Init.NbrOfConversion = 1;
	 hadc1.Init.DMAContinuousRequests = DISABLE;
	 hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
	HAL_ADC_Init(&hadc1); // Initialize the ADC


	// Configure the ADC channel
	sConfig.Channel = ADC_CHANNEL_6;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);
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
