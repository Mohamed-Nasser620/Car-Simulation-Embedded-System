/*
 * DC_motor.h
 *
 *  Created on: Sep 11, 2022
 *      Author: Mohamed Nasser
 */

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include <avr/io.h>
#include <util/delay.h>
#include "std_types.h"
#include "gpio.h"
#include "timer0_PWM.h"

/*******************************************************************************
 *                              Important Definitions                          *
 *******************************************************************************/
#define MIN_VOLTAGE 0  // Minimum CPU Voltage
#define MAX_VOLTAGE 5  // Maximum CPU Voltage

/*******************************************************************************
 *                         Types Declarations                                  *
 *******************************************************************************/

/*
 * ENUM that contains the direction of the motor's rotation
 */
typedef enum
{
	CW, CCW
} motorDirection;

/*
 * Configuration structure
 * 1. Contains the port of the motor and the selected pins
 * 2. Contains the maximum range of speed to make the necessary mapping for PWM signal
 */
typedef struct
{
	uint8 port;
	uint8 in1;
	uint8 in2;
	uint8 max_range;
} DCMotorConfig;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 *Description:
 *1. Set the port and pins selected for the motor
 *2. Set the maximum range for the motor to make the significant mapping into voltage
 */
void DC_init (const DCMotorConfig* config_ptr);

/*
 * Description:
 * 1. Takes the direction of motor's rotation and set the output pins to meet this direction
 * 2. Map the speed of the motor using the previously given maximum speed
 * 3. generates the PWM signal to control the speed
 */
void DC_speed_config (uint8 speed, motorDirection dir);

/*
 * Description:
 * 1. Stops the motor rotation
 */
void DC_stop (void);

#endif /* DC_MOTOR_H_ */
