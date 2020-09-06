#include "stm32f769xx.h"
#include "init.h"
#include<stdint.h>



int main(void){


	Sys_Init(); // This always goes at the top of main (defined in init.c)

	char KeyChar;
	int int_value;


    printf("\033[2J\033[;H"); // Erase screen & move cursor to home position
    fflush(stdout); // Need to flush stdout after using printf that doesn't end in \n

	printf("PRESS <ESC> or <CTL>+[ TO QUIT \r\n\n");

    while(1){

    	KeyChar = getchar();
   	   // putchar(KeyChar);
        printf("\r\n");

        //convert char to int value
        int_value = (int)KeyChar;

        //27 is <ESC>
       	 if( int_value == 27){
       	     printf("\033[2J\033[;H");
             fflush(stdout);
             break;
       	 }
       	 //all non printable characters
    	 else if(int_value <= 32 || int_value == 127){
        	 printf("Invalid Character\r\n\n");
      	 }
       	 //everything else is printable
      	 else{
       	 printf("The Keyboard character is ");
     		 putchar(KeyChar);
       	 printf("\r\n\n");

      	 }
    }
}
