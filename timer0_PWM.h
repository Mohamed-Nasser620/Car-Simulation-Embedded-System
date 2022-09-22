/*
 * pwm_timer0.h
 *
 *  Created on: Apr 10, 2022
 *      Author: Mohamed Nasser
 */

#ifndef TIMER0_PWM_H_
#define TIMER0_PWM_H_

#include <avr/io.h>
#include "std_types.h"

/*******************************************************************************
 *                         Types Declarations                                  *
 *******************************************************************************/
/*
 * Pre_scalers in the same numbering from the data sheet
 */
typedef enum
{
	NO_PRESCALE = 1, F_8 , F_64 , F_256 , F_1024
}Prescale ;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/* Description :
 *1. Setup the PWM mode with Non-Inverting
 *2. Takes the pre_scaler
 *3. Setup the compare value based on the required input compare value
 *4. Setup the direction for OC0 as output pin
 */
void PWM_Timer0_Start(uint8 compare_val, Prescale pre);

#endif /* TIMER0_PWM_H_ */
