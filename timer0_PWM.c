/*
 * pwm_timer0.c
 *
 *  Created on: Apr 10, 2022
 *      Author: Mohamed Nasser
 */


#include "timer0_PWM.h"

/*******************************************************************************
 *                          Functions Definitions                              *
 *******************************************************************************/

/* Description :
 *1. Setup the PWM mode with Non-Inverting
 *2. Takes the pre_scaler
 *3. Setup the compare value based on the required input compare value
 *4. Setup the direction for OC0 as output pin
 */

void PWM_Timer0_Start(uint8 compare_val, Prescale pre)
{
	OCR0  = compare_val ;
	DDRB  = DDRB | (1<<PB3);

	/* Configure timer control register
	 * 1. Fast PWM Mode (WGM01=1 & WGM00=1)
	 * 2. Select non inverted mode (COM00=0 & COM01=1)
	 * 4. Set the given pre_scaler (CS00 CS01 CS02)
	 */
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01);
	TCCR0 = (TCCR0 & 0xF8) | (pre & 0x07);
}
