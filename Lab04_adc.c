//--------------------------------
// Lab 4 - Sample - Lab04_sample.c
//--------------------------------
//
//

#include "init.h"

ADC_HandleTypeDef hadc1;

void configureADC();



uint16_t raw_temp;
uint16_t conversions = 0;
float high = 0;
float low = 0;
float average = 0;
float value;




// Main Execution Loop
int main(void)
{
	//Initialize the system
	Sys_Init();
	configureADC();
	// Code goes here
	HAL_ADC_Start(&hadc1);
    printf("\033[2J\033[;H"); // Erase screen & move cursor to home position

	while(1){

		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)){
			 HAL_ADC_PollForConversion(&hadc1, 100);

			 raw_temp = HAL_ADC_GetValue(&hadc1);
			 value = ((float)(raw_temp) *  3300 / 4095 );
			 if(high == 0 || value > high)
				 high = value;
			 if(low == 0 || value < low)
				 low = value;
			 conversions+=1;
			 average += value;


			 printf("\033[1;0H current value: %f\r\n", value);
			 printf("\033[2;0H high: %f\r\n",high);
			 printf("\033[3;0H low: %f\r\n",low);
			 printf("\033[4;0H average: %f\r\n",(float)(average)/conversions);


			 HAL_Delay(1000);
		}
	}



}

void configureADC()
{
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


void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{

    __HAL_RCC_GPIOA_CLK_ENABLE();

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
