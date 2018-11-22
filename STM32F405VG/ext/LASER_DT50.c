/**
*	Laser Sensor for stm32f103
*	@Desciption This is the library for the SICK DT50 laser sensor, which mesaures the distance from 200mm to 5000mm with 3.5% error from the board
*	@Reminder 	A 220nF capacitor must be presented at the adc port in order to filter out the noise, default ADC used are ADC_1, ADC_2 and ADC_3
*	@Author 		James Mok
**/

#include "LASER_DT50.h"

