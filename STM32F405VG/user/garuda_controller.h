#ifndef _GARUDA_CONTROLLER_H
#define _GARUDA_CONTROLLER_H

/**************************************************************************************************
** Contoller - Project Garuda
**
** Controller program.
**
** ROBOCON 2017
** H K U S T
**
** v1.1 May 2017
**
** Author:	Rex Cheng
** Contact:	hkchengad@connect.ust.hk
**
** "The Celestial City should not have been defiled with blood."
**
** NOTE: This protocol only support 127 bit ASCII. 
** Do not call gar_tft_update too frequently.
**
****************************************************************************************************/

#include "stm32f4xx.h"
#include <stdbool.h>

#include "spi_protocol.h"
#include "button.h"
#include "uart.h"
#include "gpio.h"
#include "ticks.h"

//SPI connection states
typedef enum {
	GAR_ONLY_SPI_CONNECTED,
	GAR_ONLY_BT_CONNECTED,
	GAR_BOTH_CONNECTED,
	GAR_BOTH_DISCONNECTED,
} GAR_CONNECTION_MODE;

#define SPI_GAR_PORT SPI3
#define BT_GAR_COM COM2

//Min time between interrupts triggered to count as disconnection
#define GAR_CONNECTION_TIMEOUT_MS   50

static const GPIO * gar_nss = &PC13;	//Port for SPI NSS line
#define GAR_NSS_IRQ EXTI15_10_IRQn
#define GAR_NSS_EXTILINE EXTI_Line13
#define GAR_NSS_PORTSOURCE EXTI_PortSourceGPIOC
#define GAR_NSS_PINSOURCE EXTI_PinSource13
#define GAR_NSS_HANDLER EXTI15_10_IRQHandler

typedef enum {
  GAR_SPI_FIRST,
	GAR_BT_FIRST
} GarPriority;

typedef enum{
	GAR_JOY_X,
	GAR_JOY_Y,
} GarJoy;

//This is in the order of BUTTON in the controller board.
typedef enum{
	GAR_DOWN_2, 
	GAR_UP_2,
	GAR_UP_1,
	GAR_DOWN_1,
	GAR_START_ZONE,
	GAR_SW13,
	GAR_POLE1,
	GAR_POLE2,
	GAR_POLE6,
	GAR_LOAD_ZONE,
	GAR_SW10,
	GAR_POLE7,
	GAR_RED_FIELD,
	GAR_POLE5,
	GAR_POLE3,
	GAR_POLE4,
	GAR_JOY_MID,
	GAR_SHOOT,
	GAR_IO1,
	GAR_IO2,
	GAR_IO3,
	GAR_IO4,
	GAR_IO5,
	GAR_IO6,
	GAR_MAIN_RESET
} GarButtonID;

typedef enum{
	GAR_LED_1 = 0,
	GAR_LED_2 = 1
} GarLed;

#define GAR_BTN_COUNT 25

//Init of Garuda Controller, including SPI, UART and CRC
void garuda_init(GarPriority priority);

//Return the connection status as an enum @GAR_CONNECTION_MODE
GAR_CONNECTION_MODE gar_get_connection(void);

//Return the status of the controller (as returned by the controller). 255 if disconnected.
u8 gar_get_status(void);

//Return the ticks when the last valid data is received
u32 gar_get_spi_last_recv(void);
u32 gar_get_bt_last_recv(void);

//Return the error count
u32 gar_get_spi_err(void);
u32 gar_get_bt_err(void);

//Return joystick value with deadzone calculation
s16 gar_get_joy(GarJoy j);

//To be called at a regular time interval. Button event will be triggered in this function.
void gar_button_update(void);

//Update the tft on Garuda based on the data in min_tft_protocol
void gar_tft_update(void);

//Return true if button is pressed
bool gar_button_pressed(GarButtonID buttonId);

//These functions are for registering listener for buttons
void gar_reg_OnClickListener(GarButtonID button, OnClickListener listener); //Called once when the button is pressed down
void gar_reg_OnReleaseListener(GarButtonID button, OnClickListener listener); //Called once when the button is released
/**
* Listener will be called multiple times during a long hold.
* @param threshold: Min. time required to start triggering this mechanism. The time unit is in terms of the calling frequency of update.
* @param frequency: How frequently will the function be called. The time unit is in terms of the calling frequency of update.
* @param listener: The listener function.
*/
void gar_reg_OnHoldListener(GarButtonID button_id, uint16_t threshold, uint16_t frequency, OnHoldListener listener);

//These functions are to disable registered listener
void gar_drop_OnClickListener(GarButtonID button);
void gar_drop_OnReleaseListener(GarButtonID button);
void gar_drop_OnHoldListener(GarButtonID button_id);

//Misc command
void gar_buzzer_on(void);
void gar_buzzer_off(void);
void gar_buzzer_short_beep(void);
void gar_buzzer_long_beep(void);

void gar_led_on(GarLed led);
void gar_led_off(GarLed led);
void gar_led_rapid_flash(GarLed led);
void gar_led_slow_flash(GarLed led);

#endif
