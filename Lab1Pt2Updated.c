#include "stm32f769xx.h"
#include "init.h"
#include<stdint.h>



int main(void){


	Sys_Init(); // This always goes at the top of main (defined in init.c)

	char KeyChar;
	char int_value;

	//stuff to make title centered
	const char title_str[] ="PRESS <ESC> or <CTL>+[ TO QUIT \r\n\n";
	int length = sizeof(title_str) - 1;
	int padding = (length >= 80) ? 0 : (80 - length) / 2;

	//for keeping track of row
	int counter = 12;


    printf("\033[2J\033[;H"); // Erase screen & move cursor to home position
    //initializing setting
	printf("\033[0m");//clear any previous settings
    printf("\033[44m");// background blue
    printf("\033[33m");//foreground yellow

    fflush(stdout); // Need to flush stdout after using printf that doesn't end in \n

    printf("\033[2;0H");//moving to row 2
	printf("%*.*s%s", padding, padding, " ", title_str);
    printf("\033[12;24r"); //scroll section

    while(1){
    	//reverting to this to prevent erasure of key color attributes
        printf("\033[44m\033[33m\033");// background blue, text yellow
    	KeyChar = getchar();
        //convert char to int value
        int_value = KeyChar;

        //27 is <ESC>
       	 if( int_value == 27){
       	     printf("\033[2J\033[;H");
             fflush(stdout);
             break;
       	 }
       	 //all non printable characters
    	 else if(int_value <= 32 || int_value == 127){
//    		 //if first line of non-printables then go to line 12, save position
       	     if(counter == 12){
       	    	printf("\033[12;0H\033[s");
       	     	counter= counter + 1;
       	     }
       	     if(counter != 12){
       	    	 printf("\r\n");
       	     }

    		 printf("\a"); //blink
        	 printf("\033[DThe keyboard character $%2.2x", int_value); //%2.2 prints 2 characters for hex
        	 printf(" is \033[4m\033[5m'not printable'\033[0m"); //underscores, blink, erase respectively
        	 fflush(stdout); // Need to flush stdout after using printf that doesn't end in \n

      	 }
       	 //everything else is printable
      	 else{
      	     printf("\033[s\033[6;0H");//saving last position, moving to row 6
			 printf("The Keyboard character is ");
			 printf("\033[31m");//making output red
				 putchar(KeyChar);
			 printf("\r\n\033[u"); // goes back to most recent position
      	 }
    }
}




