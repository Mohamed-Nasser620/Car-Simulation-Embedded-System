/*
 * task.c
 *
 *  Created on: Sep 11, 2022
 *      Author: Mohamed Nasser
 */

#include "common_macros.h"
#include "dc_motor.h"
#include "gpio.h"
#include "lcd.h"
#include "uart.h"
#include "stepper_motor.h"
#include "std_types.h"
#include "timer1_compare.h"
#include "timer0_PWM.h"
#include <math.h>
#include <util/delay.h>
#include <string.h>
#define UART_SIZE 9

/*******************************************************************************
 *                      Global Variables                                       *
 *******************************************************************************/

/*
 * MOTORS VARIABLES
 */
uint8 DC_speed         = 0;
uint8 DC_rotation      = 0;
uint8 stepper_rotation = 0;
uint8 stepper_angle    = 0;
uint8 last_angle       = 0;

/*
 * UART received string and its flag
 */
boolean input_error_flag           = 0;
uint8 str[UART_SIZE]  = {7,7,7,7,7,7,7,7,'\0'};

/*
 * Direction of stepper last rotation flags
 */
boolean stepper_rotation_right_flag   = FALSE;
boolean stepper_rotation_left_flag    = FALSE;

/*
 * TIME VARIABLES
 */
uint8 sec            = 0;
uint8 minute         = 0;
uint8 hour           = 0;
uint8 period         = 'A';

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description:
 * 1. Reset the string to accept a new data frame
 * 2. Start accepting the frame byte by byte
 * 3. If the 'E' indicator for end of the frame entered, the loop will break
 */
void dataFrame_receive(void)
{
	strcpy (str, "77777777");
	uint8 idx = 0;
	for(idx=0 ; str[idx]!='\0' ; idx++)
	{
		str[idx] = UART_recieveByte(); /* Receive Byte from Terminuteal1 */
		UART_sendByte(str[idx]);       /* Re-send the received byte to Terminuteal2 */
		if(str[idx]== 'E')
		{
			break;
		}
	}
}

/*
 * Description:
 * 1. Write the constant strings
 * 2. Write the speed of DC motor from the variable
 * 3. Write the angle of stepper motor from the variable
 */
void motorsParameters_print(void)
{
	LCD_moveCursor(0,0);
	LCD_displayString("SPD:   % DIR:   ");
	LCD_moveCursor(0,4);
	if(DC_speed == 100)
	{
		// For 3 Digits Input
		LCD_intgerToString(DC_speed);
	}
	else
	{
		// For 2 Digits Input
		LCD_displayCharacter(' ');
		LCD_intgerToString(DC_speed);
	}

	if(stepper_rotation_left_flag)
	{
		// For Negative Angles
		LCD_moveCursor(0,13);
		LCD_displayCharacter('-');
	}
	LCD_moveCursor(0,14);
	LCD_intgerToString(stepper_angle);
}

/*
 * Description:
 * 1. Write the constant strings
 * 2. Increment the seconds every timer firing
 * 3. Check for seconds limit (60) if exceeded increment the minutes and reset the seconds
 * 4. Check for minutes limit (60) if exceeded increment the hours and reset the minutes
 * 5. Check the period of the day to write AM or PM
 * 6. Write the updated time on LCD
 */
void time_print(void)
{
	LCD_moveCursor(1,0);
	LCD_displayString("Time:  :  :    M");

	// Time checking process
	sec++;
	if(sec == 60)
	{
		sec=0;
		minute++;
	}
	if(minute == 60)
	{
		minute=0;
		hour++;
	}
	if(hour == 12)
	{
		period = 'P';
	}
	else if(hour == 24)
	{
		period = 'A';
		hour = 0;
	}

	// Time printing Process
	LCD_moveCursor(1,5);
	if(hour < 10)
	{
		LCD_intgerToString(0);
	}
	LCD_intgerToString(hour);

	LCD_moveCursor(1,14);
	LCD_displayCharacter(period);

	LCD_moveCursor(1,8);
	if(minute < 10)
	{
		LCD_intgerToString(0);
	}
	LCD_intgerToString(minute);

	LCD_moveCursor(1,11);
	if(sec < 10)
	{
		LCD_intgerToString(0);
	}
	LCD_intgerToString(sec);
}

/*
 * Description:
 * 3. Check for the direction of stepper motor rotation
 * 4. Check for the direction of the previous stepper motor rotation
 * 5. Update the angle of rotation of stepper motor to make it goto the given angle directly
 * 6. Rotates stepper motor depending on the given direction
 * 1. Check for the direction of DC motor rotation
 * 2. Rotates DC motor depending on the given direction
 */
void rotate_motors(void)
{
	if(DC_rotation == 'F')
	{
		DC_speed_config (DC_speed, CCW);
	}
	else if(DC_rotation == 'B')
	{
		DC_speed_config (DC_speed, CW);
	}

	if(stepper_rotation == 'R')
	{
		if(stepper_rotation_right_flag == TRUE)
		{
			if (stepper_angle >= last_angle)
				stepper_to_right( stepper_angle - last_angle , full_wave_step);
			else
				stepper_to_left( last_angle - stepper_angle , full_wave_step);
		}
		else
		{
			stepper_to_right( stepper_angle + last_angle , full_wave_step);
		}

		stepper_rotation_right_flag = TRUE;
		stepper_rotation_left_flag = FALSE;
	}
	else if(stepper_rotation == 'L')
	{
		if(stepper_rotation_left_flag == TRUE)
		{
			if (stepper_angle >= last_angle)
				stepper_to_left( stepper_angle - last_angle , full_wave_step);
			else
				stepper_to_right( last_angle - stepper_angle , full_wave_step);
		}
		else
		{
			stepper_to_left( stepper_angle + last_angle , full_wave_step);
		}

		stepper_rotation_left_flag = TRUE;
		stepper_rotation_right_flag = FALSE;
	}
	motorsParameters_print();
}


int main()
{
	// Start the LCD
	LCD_init();

	/*
	 * 1. Configure the DC motor port and pins
	 * 2. Configure its range
	 */
	DCMotorConfig DC_motor = {PORTA_ID, PIN0_ID, PIN1_ID, 100};
	DC_init(&DC_motor);

	/* 1. Size of character = 8 bits
	 * 2. Disable parity check
	 * 3. Use one stop bit
	 * 4. Use 9600 baud rate
	 */
	UART_ConfigType UART_Configuration = { EIGHT_BIT , DISABLED , ONE_BIT , 9600 } ;
	UART_init(&UART_Configuration);

	/*
	 * 1. The value of compare register that counts one second
	 * 2. The pre_scaler is 1024
	 * 3. Use the function time_print as the ISR call back function
	 */
	Timer1Comp_Config Timer1_Config = { 7812 , FCPU_1024 };
	timer1_compare_init(&Timer1_Config);
	timer1_func_callback(time_print);

	/*
	 * 1. Write motors parameters on LCD
	 * 2. Start receiving the first data frame
	 */
	motorsParameters_print();
	dataFrame_receive();

	while(1)
	{
		/*
		 * In this section we check for the data frame and any error raise a flag:
		 * 1. Check the validity of the DC_motor speed input
		 * 2. If correct store in the variable DC_speed
		 */
		if(!(str[0]=='0' || str[0]=='1'))
		{
			input_error_flag =1;
		}
		else
		{
			DC_speed = ((str[0] - '0') * 100) + ((str[1] - '0') * 10) + (str[2]-'0');
			if(!(DC_speed>=0 && DC_speed<=100))
			{
				input_error_flag = 1;
			}
		}

		/*
		 * Check for the DC_rotation direction
		 */
		if(!(str[3]=='F' || str[3]=='B'))
		{
			input_error_flag = 1;
		}

		/*
		 * Check the validity of stepper motor angle
		 */
		last_angle = stepper_angle;
		stepper_angle = ((str[4] - '0') * 10) + (str[5]-'0') ;

		if(!(stepper_angle>=0 && stepper_angle<=45))
		{
			input_error_flag = 1;
		}

		/*
		 * Check the stepper rotation direction
		 */
		if(!(str[6]=='R' || str[6]=='L'))
		{
			input_error_flag = 1;
		}

		/*
		 * Check for the last terminator as it represents the data frame validity
		 */
		if(!(str[7]=='E'))
		{
			input_error_flag = 1;
		}

		/*
		 * If the input is not valid:
		 * 1. Write wrong frame on LCD
		 * 2. Reset the error flag to check again
		 * 3. Start receiving a new data frame
		 */
		if(input_error_flag)
		{
			LCD_clearScreen();
			LCD_moveCursor(0,0);
			LCD_displayString("INCORRECT FRAME");
			input_error_flag = 0;
			stepper_angle = last_angle;
			dataFrame_receive();
		}

		/*
		 * If the input is valid
		 * 1. Rotate the motors
		 * 2. Start receiving a new data frame
		 */
		else
		{
			DC_rotation = str[3];
			stepper_rotation = str[6];
			rotate_motors();
			dataFrame_receive();
		}
	}
}
