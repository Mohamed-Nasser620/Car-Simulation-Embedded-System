/*
 * timer1_compare.c
 *
 *  Created on: Sep 11, 2022
 *      Author: Mohamed Nasser
 */

#include "timer1_compare.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
static volatile void (*timer1A_interrupt)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/
ISR(TIMER1_COMPA_vect)
{
	if(timer1A_interrupt != NULL_PTR)
		(*timer1A_interrupt)();
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/* Description :
 * 1. Select Compare mode based on the configuration
 * 2. Select the required pre_scaler
 * 3. Set the compare value
 */
void timer1_compare_init(const Timer1Comp_Config* config_ptr)
{
	SREG |= (1 << 7);
	TIMSK |= (1 << OCIE1A);
	TCCR1A = (1 << FOC1A) | (1 << FOC1B);
	TCCR1B = (1 << WGM12);
	TCCR1B = (TCCR1B & 0xF8) | (config_ptr->prescaler & 0x07);
	OCR1A = config_ptr->compare_value;
}

/*Description :
 * we store in this function the address of the function we want to execute when we get an interrupt
 */
void timer1_func_callback (void (*func_ptr)(void))
{
	timer1A_interrupt = func_ptr;
}

/*Description :
 * function to disable the timer by cutting the frequency
 */
void timer1_deinit (void)
{
	TIMSK &= ~(1<<OCIE1A);
	TCCR1B &= 0xF8;
}
