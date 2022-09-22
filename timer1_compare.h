/*
 * timer1_compare.h
 *
 *  Created on: Sep 11, 2022
 *      Author: Mohamed Nasser
 */

#ifndef TIMER1_COMPARE_H_
#define TIMER1_COMPARE_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "std_types.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	OFF, NO_PRESCALING, FCPU_8 , FCPU_64 , FCPU_256 , FCPU_1024
} Prescaler ;

/*
 * Configuration structure that defines the compare value and prescaler
 */
typedef struct
{
	uint16 compare_value;
	Prescaler prescaler;
}Timer1Comp_Config;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/* Description :
 * 1. Select Compare mode based on the configuration
 * 2. Select the required pre_scaler
 * 3. Set the compare value
 */
void timer1_compare_init(const Timer1Comp_Config* config_ptr);

/*Description :
 * we store in this function the address of the function we want to execute when we get an interrupt
 */
void timer1_func_callback (void (*func_ptr)(void));

/*Description :
 * function to disable the timer by cutting the frequency
 */
void timer1_deinit (void);

#endif /* TIMER1_COMPARE_H_ */
