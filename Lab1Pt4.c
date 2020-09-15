#include "stm32f769xx.h"
#include "init.h"
#include<stdint.h>

//#include<string.h>

void Print_Line(int row[][10], int row_number);


int main(void){

	Sys_Init();
	char KeyChar;
	char int_value;


	 // This always goes at the top of main (defined in init.c)

    printf("\033[2J\033[;H"); // Erase screen & move cursor to home position
    fflush(stdout); // Need to flush stdout after using printf that doesn't end in \n


    __HAL_RCC_GPIOJ_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef but_config;

    but_config.Pin = GPIO_PIN_0;
    but_config.Mode = GPIO_MODE_INPUT;
    but_config.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &but_config);



    GPIO_InitTypeDef LED_config;
    LED_config.Pin = GPIO_PIN_13;
    LED_config.Mode = GPIO_MODE_OUTPUT_PP;
    LED_config.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOJ, &LED_config);



	int grid[10][10]={{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,1,1},
			{0,0,0,0,0,0,0,0,1,0},
			{0,0,1,1,1,0,0,0,1,0},
			{0,0,1,0,1,1,1,0,1,0},
			{0,1,1,0,0,0,1,0,1,0},
			{2,1,0,0,0,0,1,0,1,0},
			{0,0,0,0,0,0,1,1,1,0},
			{0,0,0,0,0,0,0,0,0,0}};


	//counting which row currently at
	int row_counter = 7;
	//counting which column currently at
	int col_counter = 0;


	Print_Line(grid,1);
	Print_Line(grid,2);
	Print_Line(grid,3);
	Print_Line(grid,4);
	Print_Line(grid,5);
	Print_Line(grid,6);
	Print_Line(grid,7);
	Print_Line(grid,8);
	Print_Line(grid,9);
	Print_Line(grid,10);
	fflush(stdout);



	while(1){
		//must hold down blue button then press any key
		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)){
			grid[row_counter][col_counter] = 1;
    		Print_Line(grid,row_counter+1);
    		row_counter = 7;
    		col_counter = 0;
			grid[row_counter][col_counter] = 2;
    		Print_Line(grid,row_counter+1);
		}

		KeyChar = getchar();
        int_value = KeyChar;
        //if w
        if( int_value == 119){
        	//check to see if can god up
        	if(row_counter == 0)
        		continue;
        	//check to see if wall
        	if(grid[row_counter - 1][col_counter] == 0){
        		continue;
        	}
        	//else can go up
        	else{
        		//erase character and insert into new spot, then reprint both rows
        		grid[row_counter][col_counter] = 1;
        		grid[row_counter-1][col_counter] = 2;
        		Print_Line(grid,row_counter+1);
        		Print_Line(grid,row_counter);
        		row_counter -= 1;
        	}
        }
        //if a
        else if( int_value == 97){
        	//check to see if can go left
			if(col_counter == 0)
				continue;
			//check to see if wall
			if(grid[row_counter][col_counter-1] == 0){
				continue;
			}
			//else can go up
			else{
				//erase character and insert into new spot, then reprint both rows
				grid[row_counter][col_counter] = 1;
				grid[row_counter][col_counter-1] = 2;
				Print_Line(grid,row_counter+1);
				col_counter -= 1;
			}
        }
        //if s
        else if( int_value == 115){
        	//check to see if can go down
			if(row_counter == 9)
				continue;
			//check to see if wall
			if(grid[row_counter + 1][col_counter] == 0){
				continue;
			}
			//else can go up
			else{
				//erase character and insert into new spot, then reprint both rows
				grid[row_counter][col_counter] = 1;
				grid[row_counter+1][col_counter] = 2;
				Print_Line(grid,row_counter+1);
				Print_Line(grid,row_counter+2);
				row_counter += 1;
			}
        }
        //if d
        else if( int_value == 100){
        	//check to see if can go left
			if(col_counter == 9)
				continue;
			//check to see if wall
			if(grid[row_counter][col_counter+1] == 0){
				continue;
			}
			//else can go up
			else{
				//erase character and insert into new spot, then reprint both rows
				grid[row_counter][col_counter] = 1;
				grid[row_counter][col_counter+1] = 2;
				Print_Line(grid,row_counter+1);
				col_counter += 1;
			}
        }
        else
        	continue;

        //if at end turn on LED
        if(row_counter == 2 && col_counter == 9)
			HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_13, GPIO_PIN_SET);    // Turn green LED on (GPIO_PIN_SET == 1)
        else
        	HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_13, GPIO_PIN_RESET);    // Turn green LED off (GPIO_PIN_RESET == 0)

	}
}

void Print_Line(int grid[][10], int row_number){

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


	char row_string[9] = {0};
	char col_string = '#';
	char col_string_player = '0';
	char col_string_empty = ' ';

	for (int i = 0; i < 10; i++){
		if(grid[row_number-1][i] == 0){
			strncat(row_string, &col_string, 1);
		}
		else if(grid[row_number-1][i] == 2){
			strncat(row_string, &col_string_player, 1);
		}
		else{
			strncat(row_string, &col_string_empty, 1);
		}
	}
	printf(row_string);
	fflush(stdout);
	return;
}
