/*
 * Lab04_3b.c
 *
 *  Created on: Oct 26, 2020
 *      Author: goodme
 *
Task Set 2:
1. In basic inline assembly only: loads (into registers) and adds two integer numbers (hard-coded). Use
	extended assembly to pass the resultant value to a C variable and print it.
2. In extended assembly: multiply 2 single precision floats.
3. In extended assembly: evaluate the equation 2x/3 + 5 through floating point addition (VADD),
	multiplication (VMUL), and division (VDIV).
4. In extended assembly: evaluate the previous equation using floating point MAC commands where
	applicable.
 */
//#include "arm_math.h"
#include "init.h"

int main(void)
{
	Sys_Init();
	printf("\033[2J\033[;H");

	int passed;
	//double float_a, float_b;


//	asm("ADD %[out],%[in1],%[in2]":[out] "=r" (var3):[in1] "r" (var1),[in2] "r" (var2));

	asm("LDR r1, =1");
	asm("LDR r2, =2");
	//asm("LDR r4, =3");
	//asm("LDR r5, =4");

	asm("ADD r2, r2, r1");
	asm("STR r2,%0" : "=m" (passed));	//print it



	// multiply 2 floats
	double i,j,k;
	j = 4;
	i = 2;
	asm("VMLA.F64 %P[dest_a],%P[fac_a],%P[fac_b]"
			: [dest_a] "+w" (k)
			: [fac_a] "w" (i) , [fac_b] "w" (j));

	// VLDR
	double a,c,x,b,d, y, z, final;
	x = 3;
	d = 5;
	b = 2;
	a = 3;
	c = 0;
	y = 0;
	final = 0;

	asm("VMUL.F64 %P[dest1],%P[fac1],%P[fac2]"
			: [dest1] "+w" (c)
			: [fac1] "w" (x) , [fac2] "w" (b));

	asm("VDIV.F64 %P[dest2],%P[fac3],%P[fac4]"
				: [dest2] "+w" (y)
				: [fac3] "w" (c) , [fac4] "w" (a));

	asm("VADD.F64 %P[dest3],%P[fac5],%P[fac6]"
					: [dest3] "+w" (final)
					: [fac5] "w" (y) , [fac6] "w" (d));

	printf("%f\r\n", final);

	//In extended assembly: evaluate the previous equation using floating point MAC commands where applicable.

	x = 3;
	d = 5;
	b = 2;
	a = 3;
	c = 0;
	y = 0;
	final = 0;

	asm("VMLA.F64 %P[dest1],%P[fac1],%P[fac2]"
				: [dest1] "+w" (c)
				: [fac1] "w" (x) , [fac2] "w" (b));

	asm("VDIV.F64 %P[dest2],%P[fac3],%P[fac4]"
				: [dest2] "+w" (y)
				: [fac3] "w" (c) , [fac4] "w" (a));

	asm("VADD.F64 %P[dest3],%P[fac5],%P[fac6]"
				: [dest3] "+w" (final)
				: [fac5] "w" (y) , [fac6] "w" (d));

		printf("%f\r\n", final);
}
