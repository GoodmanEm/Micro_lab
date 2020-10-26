

#include "init.h"

// Main Execution Loop
int main(void)
{
	//Initialize the system
	Sys_Init();
	// Code goes here
    printf("\033[2J\033[;H"); // Erase screen & move cursor to home position

    // 1. In basic inline assembly only: loads (into registers) and adds two integer numbers (hard-coded). Use
    	//extended assembly to pass the resultant value to a C variable and print it.

      int print1;

      asm("LDR r1, =2");
      asm("LDR r2, =9");
      asm("ADD r4, r1, r2");
      asm("STR r4,%0" : "=m" (print1));

      printf("part 1: %i\r\n" ,print1);


    //2. In extended assembly: multiply 2 int32_t variables (signed long).

      int32_t first = 45;
      int32_t second = 76;
      volatile int32_t result = 0;

      //SMULL is a thing

      asm volatile ("MUL %[final] , %[fac1], %[fac2]"
    		  	  	: [final] "=&r" (result)
					: [fac1] "r" (first) , [fac2] "r" (second));

      printf("part 2: %li\r\n" ,result);

    //3. In extended assembly: evaluate the equation 2x/3 + 5 through 32-bit integer math: addition (ADD),
    	//multiplication (MUL), and division (DIV). See Note #1

      volatile int32_t eq_result = 0;
      int32_t x = 783;

      asm volatile ("LDR r1, =2 \n\t"
    		  	  	"LDR r2, =3 \n\t"
    		  	  	"LDR r3, =5 \n\t"
    		  	  	"MUL %[result],  %[x], r1 \n\t"
    		  	  	"SDIV %[result], r2 \n\t"
    		  	  	"ADD %[result], %[result], r3"
    		  	  	: [result] "+&r" (eq_result)
					: [x] "r" (x)
					: "r1", "r2","r3");


      printf("part 3: %li\r\n" ,eq_result);


    //4. In extended assembly: evaluate the previous equation using integer MAC commands where applicable.

      volatile int32_t eq_result2 = 0;

		asm volatile ("LDR r1, =2 \n\t"
			          "LDR r2, =3 \n\t"
			          "LDR r3, =5 \n\t"
		              "SDIV %[result], %[x], r2 \n\t"
					  "MLA %[result], %[result], r1, r3"
		              : [result] "+&r" (eq_result2)
	                  : [x] "r" (x)
		        	  : "r1", "r2","r3");


       printf("part 4: %li\r\n" ,eq_result2);



}

