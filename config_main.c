#include "init.h"

/*
 * (4) 10k ohm resistors for switches
 * (blue) 100 ohm
 * (red & yellow) 220 Ohm
 * (green) 330 ohm
 * V = 5
 */

//pins to use
//PJ0 D4
//PC8 D5
ADC_HandleTypeDef hadc1;

void configureADC();

int main(void){
	Sys_Init();
	configureADC();
	HAL_ADC_Start(&hadc1);

	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	HAL_NVIC_EnableIRQ(EXTI1_IRQn);
	HAL_NVIC_EnableIRQ(EXTI3_IRQn);
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);


	// Read the README in the base directory of this project.

	printf("\033[2J\033[;H");

	while (1){
		/*
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
			printf("testing\r\n");
			HAL_Delay(500);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
		}*/

	}
}


void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc){
    GPIO_InitTypeDef pins_config;

	asm ("nop");
	asm ("nop");

	//sets 4 GPIO inputs
	pins_config.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_4;
	pins_config.Mode = GPIO_MODE_INPUT;
    pins_config.Pull = GPIO_PULLDOWN;

    HAL_GPIO_Init(GPIOJ, &pins_config);

	__HAL_RCC_GPIOJ_CLK_ENABLE();	//INPUTS: PJ0 -> D2	// PJ1 -> D4	// PJ3 -> D7	// PJ4 -> D8

	__HAL_RCC_SYSCFG_CLK_ENABLE();
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

void EXTI0_IRQHandler(){
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
}
void EXTI1_IRQHandler(){
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);
}
void EXTI3_IRQHandler(void) {

	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);
}
void EXTI4_IRQHandler(){
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_4);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

	if(GPIO_Pin == GPIO_PIN_0){
		HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_0, GPIO_PIN_SET);
		printf("testing 0\r\n");
		HAL_Delay(500);
		HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_0, GPIO_PIN_RESET);
	}

	if(GPIO_Pin == GPIO_PIN_1){
			HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_1, GPIO_PIN_SET);
			printf("testing 1\r\n");
			HAL_Delay(500);
			HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_1, GPIO_PIN_RESET);
		}

	if(GPIO_Pin == GPIO_PIN_3){
			HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_3, GPIO_PIN_SET);
			printf("testing 3\r\n");
			HAL_Delay(500);
			HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_3, GPIO_PIN_RESET);
		}

	if(GPIO_Pin == GPIO_PIN_4){
			HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_4, GPIO_PIN_SET);
			printf("testing 4\r\n");
			HAL_Delay(500);
			HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_4, GPIO_PIN_RESET);
		}

}


