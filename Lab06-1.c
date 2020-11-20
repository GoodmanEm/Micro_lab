//--------------------------------
// Microprocessor Systems Lab 6 - Template - Lab06_p1_sample.c
//--------------------------------
//
#include "init.h"
#include "usbh_conf.h"
#include "usbh_hid.h"
#include "usbh_core.h"
#include "ff_gen_drv.h"
#include "usbh_diskio.h"

USBH_HandleTypeDef husbh;
//HID_MOUSE_Info_TypeDef mouse;
//int8_t x;
//int8_t y;
// uint8_t buttons[3];

Diskio_drvTypeDef FF;

void USBH_Process_code(USBH_HandleTypeDef *, uint8_t); 	// USING CN15
void initalize_pins();


char path[4];

uint8_t old_X = 0;
uint8_t old_Y = 0;
uint8_t clicks[3];

uint8_t value;
uint8_t Appli_state = 0;


int main(void){
	 // System Initializations
	Sys_Init();		//USB_OTG_HS	// USBD_RegisterClass ()
	initalize_pins();

    // Application Initializations
	FATFS_LinkDriver(&FF,path);

	// USBH Driver Initialization	//HID Joystick mouse usbh_hid(.c,.h HID class state handler.
    USBH_Init(&husbh, USBH_Process_code, 0);

	// USB Driver Class Registrations: Add device types to handle.//Human Interface Devices
    USBH_RegisterClass(&husbh, USBH_HID_CLASS);	//usbh_hid_mouse

    // Start USBH Driver
    USBH_Start(&husbh);

	while(1){

		USBH_Process(&husbh);
		// Other stuff
	}
}		//Do not continuously poll the mouse once connected. Wait until there is data to
		//be read (Hint: Check the device driver documentation).


void USBH_Process_code(USBH_HandleTypeDef *phost,  uint8_t id) {
	// ...
	HID_MOUSE_Info_TypeDef mouse;
	//HAL_HCD_IRQHandler(phost);
	//_HID_MOUSE_Info_ mice;

	switch(id){

		case HOST_USER_CLASS_ACTIVE:
			Appli_state = 1;
			printf("ACTIVE\r\n");
			//USBH_HID_SetIdle

			/*if(mouse -> x != old_X){
				old_X = mouse -> x;
			 }

			if(mouse -> y != old_Y){
				old_Y = mouse -> y;
			}

			if (mouse -> button[0]){
				printf("beep1\r\n");


			}
			if (mouse -> button[1]){
				printf("beep2\r\n");

			}
			if (mouse -> button[2]){
				printf("beep3\r\n");
			}*/

			break;

		case HOST_USER_CONNECTION:
			Appli_state = 2;
			printf("Connected\r\n");
			//USBH_HID_SetIdle

			break;

		case HOST_USER_DISCONNECTION:
			Appli_state = 3;
			printf("Disconnected\r\n");
			//set idle and don't poll
			// USBH_HID_SetIdle

			break;

		default:
			break;
	}
}

//  HOW TO IMPLIMENT
//HOW to access X & Y

//Callbacks...
void USB_HID_Callback(USBH_HandleTypeDef *phost, uint8_t id){
	//USBH_HID_EventCallback
	//USBH_HID_GetMouseInfo();
	HCD_HandleTypeDef callback;

	switch(id){
		case HOST_USER_DISCONNECTION:
			HAL_HCD_DisConnect_Callback(&callback);
			break;

		case HOST_USER_CONNECTION:
			HAL_HCD_Connect_Callback(&callback);
			break;

		case HOST_USER_CLASS_ACTIVE:
			//USBH_HID_EventCallback(&callback);
			//USBH_HID_GetMouseInfo
			break;

	}
}



void initalize_pins(){
	/*
	PA3 USB_OTG_HS_ULPI_D0
	PA5 USB_OTG_HS_ULPI_CK
	PB0 USB_OTG_HS_ULPI_D1
	PB1 USB_OTG_HS_ULPI_D2
	PB5 USB_OTG_HS_ULPI_D7
	PB10 USB_OTG_HS_ULPI_D3
	PB11 USB_OTG_HS_ULPI_D4
	PB12 USB_OTG_HS_ULPI_D5
	PB13 USB_OTG_HS_ULPI_D6
	PC0 USB_OTG_HS_ULPI_STP
	PH4 USB_OTG_HS_ULPI_NXT
	PI11 USB_OTG_HS_ULPI_DIR*/

	GPIO_InitTypeDef GPIOA_InitStruct;
	GPIO_InitTypeDef GPIOB_InitStruct;
	GPIO_InitTypeDef GPIOC_InitStruct;
	GPIO_InitTypeDef GPIOH_InitStruct;
	GPIO_InitTypeDef GPIOI_InitStruct;

/*	USB_OTG_HS_ULPI_D0.Pin = GPIO_PIN_3;*/

	GPIOA_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_5;
	HAL_GPIO_Init(GPIOA, &GPIOA_InitStruct);

	GPIOB_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_5|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12 |GPIO_PIN_13;
	HAL_GPIO_Init(GPIOB, &GPIOB_InitStruct);

	GPIOC_InitStruct.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOC, &GPIOC_InitStruct);

	GPIOH_InitStruct.Pin = GPIO_PIN_4;
	HAL_GPIO_Init(GPIOH, &GPIOH_InitStruct);

	GPIOI_InitStruct.Pin = GPIO_PIN_11;
	HAL_GPIO_Init(GPIOI, &GPIOI_InitStruct);
}

