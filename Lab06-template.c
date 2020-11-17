//--------------------------------
// Microprocessor Systems Lab 6 - Template - Lab06_p1_sample.c
//--------------------------------
//
//

#include "init.h"
#include "usbh_conf.h"
#include "usbh_hid.h"
#include "usbh_core.h"
#include "ff_gen_drv.h"
#include "usbh_diskio.h"

USBH_HandleTypeDef husbh;
Diskio_drvTypeDef fatfsh;
void USBH_UserProcess(USBH_HandleTypeDef *, uint8_t);

char path[4];
uint8_t Appli_state = 0;


int main(void){
	 // System Initializations
	Sys_Init();
    printf("\033[2J\033[;H"); // Erase screen & move cursor to home position
	fflush(stdout); // Need to flush stdout after using printf that doesn't end in \n

	// Application Initializations
	FATFS_LinkDriver(&fatfsh,path);

	// USBH Driver Initialization
    USBH_Init(&husbh, USBH_UserProcess, 0);

	// USB Driver Class Registrations: Add device types to handle.
    USBH_RegisterClass(&husbh, USBH_MSC_CLASS);

	// Start USBH Driver
	USBH_Start(&husbh);

	while(1){
		USBH_Process(&husbh);



		// Other stuff
	}
}

void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id) {
	 switch (id){
		/* when HOST_USER_CLASS_ACTIVE event is received, application can start
		 communication with device*/
		 case HOST_USER_CLASS_ACTIVE:
			 Appli_state = 1;
			 if(phost -> pActiveClass->Name == "MSC"){
				 printf("hello\r\n");
			 }

			 break;
		 case HOST_USER_DISCONNECTION:
				 Appli_state = 2;
				 break;
		 case HOST_USER_CONNECTION:
			 Appli_state = 3;
			 break;

		 default:
			 break;
	}
}

// Interrupts and Callbacks...

//void HAL_HCD_IRQHandler(USBH_HandleTypeDef * husb){
//
//
//}
