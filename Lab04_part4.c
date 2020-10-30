/*
 * Lab04_4.c
 *
 *  Created on: Oct 28, 2020
 *      Author: goodme
 */
/*
#include "init.h"
#include "uart.h"

DAC_HandleTypeDef hdac;
ADC_HandleTypeDef hadc1;


static void DAC_Init(void);
void configureADC();
float value;
uint16_t raw_temp;

double current;
double past_x1;
double past_x2;
double current_y;
double past_y;

// Main Execution Loop
int main(void)
{
	unsigned int N_dac = 0;
	int counter = 0;

	//Initialize the system
	Sys_Init();

	__HAL_RCC_GPIOA_CLK_ENABLE();

	HAL_DAC_MspInit(&hdac);

	configureADC();
	HAL_ADC_Start(&hadc1);

	DAC_Init();
	HAL_DAC_Init(&hdac);
	HAL_DAC_Start(&hdac, DAC_CHANNEL_1);

	printf("\033[2J\033[;H");

	while (1) {

		if (counter >= 3){
			HAL_ADC_PollForConversion(&hadc1, 1);
			raw_temp = HAL_ADC_GetValue(&hadc1);
			past_x2 = past_x1;
			past_x1 = current;
			current = raw_temp ;

			//y(k) = 0.312500x(k) + 0.240385x(k − 1) + 0.312500x(k − 2) + 0.296875y(k − 1)
			current_y = 0.312500*current + 0.240385*past_x1 + 0.312500*past_x2  + 0.296875*past_y;
			past_y = current_y;

			//*HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, current_y);
			//HAL_Delay(1);
			//printf("number %u\r\n", current_y);
		}

		else if (counter == 2){
			HAL_ADC_PollForConversion(&hadc1, 1);
			raw_temp = HAL_ADC_GetValue(&hadc1);
			past_x2 = past_x1;
			past_x1 = current;
			current = raw_temp;

			past_y = 0.312500*current + 0.240385*past_x1 + 0.312500*past_x2;
		}

		else if (counter == 1){
			HAL_ADC_PollForConversion(&hadc1, 1);
			raw_temp = HAL_ADC_GetValue(&hadc1);
			past_x1 = current;
			current = raw_temp;
		}

		else if (counter == 0){
			HAL_ADC_PollForConversion(&hadc1, 1);
			raw_temp = HAL_ADC_GetValue(&hadc1);
			current = raw_temp;
		}
		counter++;
	}
}

void DAC_Init(void) {

	DAC_ChannelConfTypeDef sConfig;

	RCC->APB1ENR |= RCC_APB1ENR_DACEN;
		asm ("nop");
		asm ("nop");

	// Initialization
	hdac.Instance = DAC;
	HAL_DAC_Init(&hdac);

	//**DAC channel OUT1 config
	sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
	HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1);

}

void HAL_DAC_MspInit(DAC_HandleTypeDef *hadc) {

	GPIO_InitTypeDef GPIO_InitStruct;

	//* DAC GPIO Configuration, PA5 ------> DAC_OUT1 or PA4
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
*/
#include "init.h"
#include "uart.h"

DAC_HandleTypeDef hdac;
ADC_HandleTypeDef hadc1;


static void DAC_Init(void);
void configureADC();
float value;
double raw_temp = 0;
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
	HAL_ADC_Start(&hadc1);

	DAC_Init();
	HAL_DAC_Init(&hdac);
	HAL_DAC_Start(&hdac, DAC_CHANNEL_1);

	printf("\033[2J\033[;H");

	while (1) {

		raw_temp_minus2 = raw_temp_minus1;
		raw_temp_minus1 = raw_temp;

		HAL_ADC_PollForConversion(&hadc1, 1);
		raw_temp = HAL_ADC_GetValue(&hadc1);


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
