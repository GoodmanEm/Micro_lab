#include "init.h"
#include "stdio.h"

//function definitions
void Print_Line(int row[8], int row_number);
void Clock_Setup(void);
void Scoring(void);
void Print_Game(void);
void Check_Buttons(void);
void configureADC();
//handle type definitions
ADC_HandleTypeDef hadc1;

//predifined strum bar markers, one for normal, one to indicate whammy bar active
char strum_line[]="-----------------";
char strum_line_whammy[]="~~~~~~~~~~~~~~~~~";
//12 lines of blank in beginning for padding
//12 lines of blank at end for padding
int grid[][8]={   //2D array containing game board
		{0,0,0,0,0,0,0,0}, // any number of arrays may be inserted
		{0,0,0,0,0,0,0,0}, // 0 = blank space
		{0,0,0,0,0,0,0,0}, // 1 = note
		{0,0,0,0,0,0,0,0}, // 2 = long note (must trail a 1, is on programmer to remember this)
		{0,0,0,0,0,0,0,0}, //     a long note will not be able to be hit reguardless if there is no note leading it
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,1,0,0},
		{0,0,0,0,0,2,0,0},
		{0,0,0,0,0,2,0,0},
		{0,0,0,0,0,2,0,0},
		{0,0,0,0,0,2,0,0},
		{0,0,0,0,0,2,0,0},
		{0,1,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,1,0,1},
		{0,0,0,0,0,1,0,1},
		{0,0,0,0,0,1,0,1},
		{0,1,0,1,0,0,0,0},
		{0,1,0,1,0,0,0,0},
		{0,1,0,1,0,0,0,0},
		{0,1,0,1,0,0,0,0},
		{0,1,0,1,0,0,0,0},
		{0,1,0,1,0,0,0,0},
		{0,0,0,0,0,1,0,1},
		{0,0,0,0,0,1,0,1},
		{0,0,0,0,0,1,0,1},
		{0,0,0,0,0,1,0,1},
		{0,0,0,0,0,1,0,1},
		{0,1,0,1,0,1,0,1},
		{0,1,0,1,0,1,0,1},
		{0,1,0,1,0,1,0,1},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0}};
//TIM6 counter value
volatile uint32_t counter = 0;
//previous value of conter
uint32_t prev_counter = 0;
//size of grid used to determing amount of looping
int array_size = sizeof grid / sizeof grid[0];
//chars representing if the buttons are held down or not
char green_hold = 0;
char red_hold = 0;
char yellow_hold = 0;
char blue_hold = 0;
//chars representing if the buttone needs to be held down for a long note
char green_long = 0;
char red_long = 0;
char yellow_long = 0;
char blue_long = 0;
//indicator if whammy bar is active
char whammy_time = 0;
//used to determine if strum was done
uint8_t space_press;
char strum = 0;
//used to keep track of score and number of notes hit
uint32_t score = 0;
uint32_t note_total = 0;
//variable to help determine if player hit a note or not when they were supposed to
char miss = 1;
//for poll/ADC
int value;
int cut_off = 41;		//200mv out of 5V = 25	1023/25
int original_value;	//range 0 - 1023
//array to hold notes that were hit correctly
int notes_hit[8] = {0,0,0,0,0,0,0,0};

int main(void){
	Sys_Init();
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	asm ("nop");
	asm ("nop");
	printf("\033[2J\033[;H"); // Erase screen & move cursor to home position
	fflush(stdout); // Need to flush stdout after using printf that doesn't end in \n
	configureADC(); // Configure ADC and GPIO 
	HAL_ADC_Start(&hadc1); //Start ADC
	HAL_NVIC_EnableIRQ(USART1_IRQn); //enable UART interupts
    HAL_UART_Receive_IT (&USB_UART, &space_press, 1); // initilize UART so it will actually trigger interupt
	Clock_Setup(); // set up clock 

 	//setting value to compare the ADC to in order to determine if whammy bar is ative
	HAL_ADC_PollForConversion(&hadc1, 0);
	original_value = HAL_ADC_GetValue(&hadc1);

	//main loop that goes while there are still notes to hit
	//condition is to ensure not going out of bounds of array
	while(counter < (array_size-12)){

		//check to see what buttons are being pushed
		Check_Buttons();

		//get ADC value
  		HAL_ADC_PollForConversion(&hadc1,0);
		value = HAL_ADC_GetValue(&hadc1);			 //From PIN A0  (PA6)
		if (abs(value - original_value)> cut_off){ //If difference is great enough, i.e. potentiometer was turned enough, whammy bar is active
			whammy_time = 1;
		}
		else { //otherwise whammy bar is not active
  			whammy_time = 0;
		}

		//if counter increases then print board again (and at counter =0)
		if(counter == 0 || (counter > prev_counter)){
			prev_counter = counter;
			Print_Game( );
			//clear notes hit
			for(int i = 0; i < 8; ++i)
				notes_hit[i] = 0;

		}
		//if space bar is hit then score 
		if(strum)
			Scoring();

		// following portion will check to see if the note is supposed to be held
		//if it is then it will go to scoring and add points appropriately
		//else if it was to be held for long not then reset long note flag
		if(green_long && green_hold)
			Scoring();
		else if(green_long)
			green_long = 0;
		if(red_long && red_hold)
			Scoring();
		else if(red_long)
			red_long = 0;
		if(yellow_long && yellow_hold)
			Scoring();
		else if(yellow_long)
			yellow_long = 0;
		if(blue_long && blue_hold)
			Scoring();
		else if(blue_long)
			blue_long = 0;

		//will reset holds to 0 reguardless
		//if they are being held they will go back to 1 before anything checks
		green_hold = 0;
		red_hold = 0;
		yellow_hold = 0;
		blue_hold = 0;
	}
}

//scoring function
void Scoring(void){
	//function was called because space bar was pressed or long note
	//if called otherwise will not make any changes
	if(strum == 1){ // space bar was pressed
		strum = 0; // reset strum
		//will be 0 if miss
		miss = 1;
		//go through to see if buttons pressed while strumming are correct
		for (int i = 0; i < 8; i++){
			if(i==1){
				//if button is in grid and button was held the give points
				if(grid[1+counter][i] == 1 && green_hold){
					//adding to the notes hit
					notes_hit[i] =1;
					score += 100;
					//if it is a long note, hit flag for specific color
					if(grid[2+counter][i] == 2)
						green_long = 1;
				}
				//else strummed and there was no note there
				else if(grid[1+counter][i] == 0 && green_hold) {
					miss = 0;
					//make beep noise
					printf("\a");
				}
			}
			if(i==3){
				if(grid[1+counter][i] == 1 && red_hold){
					notes_hit[i] =1;
					score += 100;
					if(grid[2+counter][i] == 2)
						red_long = 1;
				}
				else if(grid[1+counter][i] == 0 && red_hold) {
					miss = 0;
					//make beep noise
					printf("\a");

				}
			}
			if(i==5){
				if(grid[1+counter][i] == 1 && yellow_hold){
					notes_hit[i] =1;
					score += 100;
					if(grid[2+counter][i] == 2)
							blue_long = 1;
				}
				else if(grid[1+counter][i] == 0 && yellow_hold) {
					miss = 0;
					//make beep noise
					printf("\a");

				}
			}
			if(i==7){
				if(grid[1+counter][i] == 1 && blue_hold){
					notes_hit[i] =1;
					score += 100;
					if(grid[2+counter][i] == 2)
						blue_long = 1;
				}
				else if(grid[1+counter][i] == 0 && blue_hold) {
					miss = 0;
					//make beep noise
					printf("\a");
				}
			}
		}
		//if did not miss then add one to note total
		if (miss != 0)
			note_total +=1;
	}
	//else function was called to give points for long note
	else{
		// if whammy time the 2x points
		if(green_long && whammy_time)
			score += 2;
		else if(green_long)
			score +=1;
		if(red_long && whammy_time)
			score += 2;
		else if(red_long)
			score +=1;
		if(yellow_long && whammy_time)
			score += 2;
		else if(yellow_long)
			score +=1;
		if(blue_long && whammy_time)
			score += 2;
		else if(blue_long)
			score +=1;

	}
}


//function that prints out all the lines needed on the screen
void Print_Game(void){
	//prints out 11 rows at a time + strum line
	//will increment with timer to scroll
	Print_Line(grid[11+counter],1);
	Print_Line(grid[10+counter],2);
	Print_Line(grid[9+counter],3);
	Print_Line(grid[8+counter],4);
	Print_Line(grid[7+counter],5);
	Print_Line(grid[6+counter],6);
	Print_Line(grid[5+counter],7);
	Print_Line(grid[4+counter],8);
	Print_Line(grid[3+counter],9);
	Print_Line(grid[2+counter],10);
	Print_Line(grid[1+counter],11);	//this is line where you hit buttons and strum

	printf("\r\n\033[12;0H"); // moving cursor to the line where the strum bar goes
	if(whammy_time)
		//print special line if whammy
		printf(strum_line_whammy);
	else// otherwise print normal strum line
		printf(strum_line);
	fflush(stdout);
	//prints notes that were hit
	Print_Line(notes_hit,13);
	//prints score
	printf("\r\n\033[14;0H score: %d", score);
	//prints out the number of notes that were hit (double, triple, and quadruple notes counts a 1 note)
	printf("\r\n\033[15;0H Notes hit: %d", notes_hit);
}


//clock set up function
void Clock_Setup(void){
	//char difficuty = 2;

    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
	asm ( "nop" );
	asm ( "nop" );
	NVIC_EnableIRQ(54); //enabling the interupt for TIM6

//note that this comented out section controls the sped selection
//was atempted using getchar and scanf but was not halting to accept input from terminal
//can be re inserted when issue fixed
	// printf("choose difficulty\r\n");
	// printf("a = baby\r\n");
	// printf("s = normal\r\n");
	// printf("d = nightmare\r\n");
	// //difficuty = getchar();

	// if(difficuty == 'a') {
	// 	TIM6->PSC = 4999;
	// }
	// else if(difficuty == 's'){
	// 	TIM6->PSC = 16669;
	// }
	// else if(difficuty == 'd'){
	// 	TIM6->PSC = 24999;
	// }
	// else{
	// 	TIM6->PSC = 16669;
	// }
	// printf("\033[2J\033[;H"); // Erase screen & move cursor to home position
	// fflush(stdout); // Need to flush stdout after using printf that doesn't end in \n

	// Set pre-scaler to slow down ticks
	//4999-> .1s
	//12499 -> .25s
	//16669-> .33s
	TIM6->PSC = 16669; //commented out if speed select is active
	//Set the Auto-reload Value
	TIM6->ARR = 2159;
	// Generate update events to auto reload.
	TIM6->EGR  |= 0x1U;
	// Enable Update Interrupts.
	TIM6->DIER |= TIM_DIER_UIE;
	// Start the timer.
	TIM6->CR1 |= TIM_CR1_CEN;
	}

///modify this function to print our a line for GH
void Print_Line(int grid[8], int row_number){

	//making string to print to specific line
	char row_num_string[3];
	snprintf(row_num_string, 3, "%d" , row_number);

	// declare and initialize string
	char str1[6] = "\033[";
	char str2[] = ";0H";

	// append ch to str
	strncat(str1, row_num_string, 2);
	strncat(str1, str2, 3);

	// print string
	printf(str1);
    fflush(stdout);
    //now should be at line that needs to be printed


    //string that will contain final line to print
	char row_string[54] = {0};
	//strings that will be inserted into row_string
	char col_note = '0';
	char col_note_long = '|';
	char col_note_long_odd = '/';
	char col_note_long_even = '\\';
	char col_note_empty = ' ';
	char end_string[1] = "'";
	// pre-defined strings to make specific columns a color and other stuff
	char green_string[] = "\033[32m'";
	char red_string[] = "\033[31m'";
	char blue_string[] = "\033[33m'";
	char yellow_string[] = "\033[34m'";
	char clear_string[] = "\033[0m";

	for (int i = 0; i < 8; i++){
		char color_string[6] = {0};
		//checks color
		if(i == 1){
			strncat(color_string, &green_string, 8);
		}
		else if(i == 3){
			strncat(color_string, &red_string, 8);
		}
		else if(i == 5){
			strncat(color_string, &blue_string, 8);
		}
		else if(i == 7){
			strncat(color_string, &yellow_string, 8);
		}

		//it is a note
		if(grid[i] == 1){
			//adding color
			strncat(row_string, &color_string, 8);
			//adding char
			strncat(row_string, &col_note, 1);
			//closing '
			strncat(row_string, &end_string, 1);
		}
		//it is a long note
		else if(grid[i] == 2){
			//adding color
			strncat(row_string, &color_string, 8);
			//if it is whammy bar time, print the diagonal lines
			if(whammy_time){
				if (row_number % 2)
					strncat(row_string, &col_note_long_odd, 1);
				else
					strncat(row_string, &col_note_long_even, 1);
			}
			else
				strncat(row_string, &col_note_long, 1);
			//closing '
			strncat(row_string, &end_string, 1);
		}
		//condition where note column is emtpy
		else if(i == 1 || i == 3 || i == 5|| i == 7){
			//adding color
			strncat(row_string, &color_string, 8);
			//adding char
			strncat(row_string, &col_note_empty, 1);
			//closing '
			strncat(row_string, &end_string, 1);
		}
		else{
			strncat(row_string, &col_note_empty, 1);
		}
		//if last row, add sequence to erase all color formatting
		if(i == 7)
			strncat(row_string, &clear_string, 6);
	}
	//print out line
	printf(row_string);
	fflush(stdout);
	return;
}
//Timer interupt handler
void TIM6_DAC_IRQHandler() {
	// Clear Interrupt Bit
	TIM6->SR &= ~(TIM_SR_UIF);
	counter += 1;
}
//inililiziing ADC and GPIO
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc){
    GPIO_InitTypeDef pins_config;
    GPIO_InitTypeDef GPIOInit;

	__HAL_RCC_GPIOJ_CLK_ENABLE();	//INPUTS: PJ0 -> D2	// PJ1 -> D4	// PJ3 -> D7	// PJ4 -> D8

	__HAL_RCC_GPIOA_CLK_ENABLE();	//for ADC

	__HAL_RCC_SYSCFG_CLK_ENABLE();

	asm ("nop");
	asm ("nop");

	//sets 4 GPIO inputs
	pins_config.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_4;
	pins_config.Mode = GPIO_MODE_INPUT;
    pins_config.Pull = GPIO_PULLDOWN;

    HAL_GPIO_Init(GPIOJ, &pins_config);

    //ADC PIN A0  (PA6)
    GPIOInit.Pin = GPIO_PIN_6;
    GPIOInit.Mode = GPIO_MODE_ANALOG;
    GPIOInit.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOA, &GPIOInit);

}
//configureing ADC
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
//function that is used for polling buttons
void Check_Buttons(void) {

	//green
	if(HAL_GPIO_ReadPin(GPIOJ, GPIO_PIN_0))
		green_hold =1;
	else
		green_hold = 0;
	//red
	if(HAL_GPIO_ReadPin(GPIOJ, GPIO_PIN_1))
		red_hold =1;
	else
		red_hold = 0;
	//yellow
	if(HAL_GPIO_ReadPin(GPIOJ, GPIO_PIN_3))
		yellow_hold =1;
	else
		yellow_hold = 0;
	//blue
	if(HAL_GPIO_ReadPin(GPIOJ, GPIO_PIN_4))
		blue_hold =1;
	else
		blue_hold = 0;
}
//uart interupt handler
void USART1_IRQHandler() {
	HAL_UART_IRQHandler(&USB_UART);
    HAL_UART_Receive_IT (&USB_UART, &space_press, 1);
}
//uart callback
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
 //if you hit the space bar
  if (huart->Instance == USART1 && space_press == 32)
  {
	  strum = 1;
  }

}