/*
 * stepper_motor.c
 *
 *  Created on: Apr 8, 2022
 *      Author: Mohamed Nasser
 */

#include "stepper_motor.h"

/*******************************************************************************
 *                      Global Variables                                       *
 *******************************************************************************/
uint8 previous = 0;
uint8 idx_right = 0;
uint8 idx_left = 0;

/*******************************************************************************
 *                          Functions Definitions                              *
 *******************************************************************************/

/*
 * Description:
 * 1. Takes the angle and mode step size mode (Half/Full)
 * 2. Rotates the motor to the right with the given angle
 */
void stepper_to_right(uint8 angle , Stepper_Motor_Step_Size mode)
{

	uint8 motor_steps[FULL_WAVE] = {3,9,12,6};

	// Configure first 4 pins in stepper motor's PORT as output pins
	GPIO_setupPinDirection(STEPPER_MOTOR_PORT_ID,MOTOR_FIRST_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(STEPPER_MOTOR_PORT_ID,MOTOR_FIRST_PIN_ID+1,PIN_OUTPUT);
	GPIO_setupPinDirection(STEPPER_MOTOR_PORT_ID,MOTOR_FIRST_PIN_ID+2,PIN_OUTPUT);
	GPIO_setupPinDirection(STEPPER_MOTOR_PORT_ID,MOTOR_FIRST_PIN_ID+3,PIN_OUTPUT);

	if(mode == full_wave_step)
	{
		for (unsigned char i = 0; i < angle; i++)
		{
			// move the motor to the next step
			switch (STEPPER_MOTOR_PORT_ID)
			{
			case PORTA_ID : PORTA = ((motor_steps[idx_right] & 0x0F)<<PIN4_ID) | (PORTA & 0x0F);
			break;
			case PORTB_ID : PORTB = ((motor_steps[idx_right] & 0x0F)<<PIN4_ID) | (PORTB & 0x0F);
			break;
			case PORTC_ID : PORTC = ((motor_steps[idx_right] & 0x0F)<<PIN4_ID) | (PORTC & 0x0F);
			break;
			case PORTD_ID : PORTD = ((motor_steps[idx_right] & 0x0F)<<PIN4_ID) | (PORTD & 0x0F);
			break;
			}
			idx_right++;
			if(idx_right == FULL_WAVE)
			{
				//start from the beginning after finish the full motor rotation.
				idx_right = 0;
			}
			if(idx_left == 0)
			{
				//start from the beginning after finish the full motor rotation.
				idx_left = FULL_WAVE;
			}
			idx_left--;
			_delay_ms(35);
		}
		previous = motor_steps[idx_right];
	}
}

/*
 * Description:
 * 1. Takes the angle and mode step size mode (Half/Full)
 * 2. Rotates the motor to the left with the given angle
 */
void stepper_to_left(uint8 angle , Stepper_Motor_Step_Size mode)
{
	uint8 motor_steps[FULL_WAVE] = {12,9,3,6};

	// Configure first 4 pins in stepper motor's PORT as output pins
	GPIO_setupPinDirection(STEPPER_MOTOR_PORT_ID,MOTOR_FIRST_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(STEPPER_MOTOR_PORT_ID,MOTOR_FIRST_PIN_ID+1,PIN_OUTPUT);
	GPIO_setupPinDirection(STEPPER_MOTOR_PORT_ID,MOTOR_FIRST_PIN_ID+2,PIN_OUTPUT);
	GPIO_setupPinDirection(STEPPER_MOTOR_PORT_ID,MOTOR_FIRST_PIN_ID+3,PIN_OUTPUT);

	if(mode == full_wave_step)
	{
		for (unsigned char i = 0; i < angle; i++)
		{
			// move the motor to the next step
			switch (STEPPER_MOTOR_PORT_ID)
			{
			case PORTA_ID : PORTA = ((motor_steps[idx_left] & 0x0F)<<PIN4_ID) | (PORTA & 0x0F);
			break;
			case PORTB_ID : PORTB = ((motor_steps[idx_left] & 0x0F)<<PIN4_ID) | (PORTB & 0x0F);
			break;
			case PORTC_ID : PORTC = ((motor_steps[idx_left] & 0x0F)<<PIN4_ID) | (PORTC & 0x0F);
			break;
			case PORTD_ID : PORTD = ((motor_steps[idx_left] & 0x0F)<<PIN4_ID) | (PORTD & 0x0F);
			break;
			}
			idx_left++;
			if(idx_left == FULL_WAVE)
			{
				//start from the beginning after finish the full motor rotation.
				idx_left = 0;
			}
			if(idx_right == 0)
			{
				//start from the beginning after finish the full motor rotation.
				idx_right = FULL_WAVE;
			}
			idx_right--;
			_delay_ms(35);
		}
		previous = motor_steps[idx_left];
	}
}
