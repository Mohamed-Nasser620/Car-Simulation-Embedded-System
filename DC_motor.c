/*
 * DC_motor.c
 *
 *  Created on: Sep 11, 2022
 *      Author: Mohamed Nasser
 */

#include "DC_motor.h"

/*******************************************************************************
 *                      Global Variables                                       *
 *******************************************************************************/
static volatile uint8 motor_max_speed = 0;
static volatile uint8 port = 0;
static volatile uint8 in1 = 0;
static volatile uint8 in2 = 0;

/*******************************************************************************
 *                          Functions Definitions                              *
 *******************************************************************************/

/*
 *Description:
 *1. Set the port and pins selected for the motor
 *2. Set the maximum range for the motor to make the significant mapping into voltage
 */
void DC_init (const DCMotorConfig* config_ptr)
{
	motor_max_speed = config_ptr -> max_range;
	port = config_ptr -> port;
	in1 = config_ptr -> in1;
	in2 = config_ptr -> in2;
	switch (port)
	{
	case PORTA_ID:
		DDRA |= (1 << in1) | (1 << in2);
		break;
	case PORTB_ID:
		DDRB |= (1 << in1) | (1 << in2);
		break;
	case PORTC_ID:
		DDRC |= (1 << in1) | (1 << in2);
		break;
	case PORTD_ID:
		DDRD |= (1 << in1) | (1 << in2);
	}
}

/*
 * Description:
 * 1. Takes the direction of motor's rotation and set the output pins to meet this direction
 * 2. Map the speed of the motor using the previously given maximum speed
 * 3. generates the PWM signal to control the speed
 */
void DC_speed_config (uint8 speed, motorDirection dir)
{
	if (dir == CW)
	{
		switch (port)
		{
		case PORTA_ID:
			PORTA |= (1 << in1);
			PORTA &= ~(1 << in2);
			break;
		case PORTB_ID:
			PORTB |= (1 << in1);
			PORTB &= ~(1 << in2);
			break;
		case PORTC_ID:
			PORTC |= (1 << in1);
			PORTC &= ~(1 << in2);
			break;
		case PORTD_ID:
			PORTD |= (1 << in1);
			PORTD &= ~(1 << in2);
		}
	}

	else if (dir == CCW)
	{
		switch (port)
		{
		case PORTA_ID:
			PORTA &= ~(1 << in1);
			PORTA |= (1 << in2);
			break;
		case PORTB_ID:
			PORTB &= ~(1 << in1);
			PORTB |= (1 << in2);
			break;
		case PORTC_ID:
			PORTC &= ~(1 << in1);
			PORTC |= (1 << in2);
			break;
		case PORTD_ID:
			PORTD &= ~(1 << in1);
			PORTD |= (1 << in2);
		}
	}

	/*--------------------------Mapping Section---------------------------*/
	float32 mapped_speed_in_volt = (speed * MAX_VOLTAGE) / (float32) motor_max_speed;
	uint8 duty_cycle = (mapped_speed_in_volt / MAX_VOLTAGE) * 100;
	if (duty_cycle == 100)
	{
		PWM_Timer0_Start(255, F_256);
	}
	else
	{
		uint8 compare_value = 256 * (duty_cycle / 100.00);
		PWM_Timer0_Start(compare_value, F_256);
	}

}

/*
 * Description:
 * 1. Stops the motor rotation
 */
void DC_stop (void)
{
	switch (port)
	{
	case PORTA_ID:
		PORTA &= ~(1 << in1) &~ (1 << in2);
		break;
	case PORTB_ID:
		PORTB &= ~(1 << in1) &~ (1 << in2);
		break;
	case PORTC_ID:
		PORTC &= ~(1 << in1) &~ (1 << in2);
		break;
	case PORTD_ID:
		PORTD &= ~(1 << in1) &~ (1 << in2);
	}
}
