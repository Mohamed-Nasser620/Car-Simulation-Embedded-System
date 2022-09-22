/*
 * stepper_motor.h
 *
 *  Created on: Apr 8, 2022
 *      Author: Mohamed Nasser
 */

#ifndef STEPPER_MOTOR_H_
#define STEPPER_MOTOR_H_

#include <avr/io.h>
#include <util/delay.h>
#include "std_types.h"
#include "gpio.h"


/*******************************************************************************
 *                              Important Definitions                          *
 *******************************************************************************/
#define HALF_WAVE 8
#define FULL_WAVE 4
#define STEPPER_MOTOR_PORT_ID           PORTB_ID
#define MOTOR_FIRST_PIN_ID              PIN4_ID

/*******************************************************************************
 *                         Types Declarations                                  *
 *******************************************************************************/
typedef enum
{
	full_wave_step
}Stepper_Motor_Step_Size;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description:
 * 1. Takes the angle and mode step size mode (Half/Full)
 * 2. Rotates the motor to the right with the given angle
 */
void stepper_to_right (uint8 angle , Stepper_Motor_Step_Size mode);

/*
 * Description:
 * 1. Takes the angle and mode step size mode (Half/Full)
 * 2. Rotates the motor to the left with the given angle
 */
void stepper_to_left (uint8 angle , Stepper_Motor_Step_Size mode);

#endif /* STEPPER_MOTOR_H_ */
