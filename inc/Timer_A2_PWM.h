/**
 * @file Timer_A2_PWM.h
 * @brief Header file for the Timer_A2_PWM driver.
 *
 * This file contains the function definitions for configuring Timer A2 in Pulse Width Modulation (PWM) mode.
 * It provides functions for initializing the PWM timer and the updating duty cycle for PWM signals.
 *
 * @author Aaron Nanas
 *
 */

#ifndef TIMER_A2_PWM_H_
#define TIMER_A2_PWM_H_

#include <stdint.h>
#include "msp.h"

/**
 * @brief Initialize Timer A2 for PWM operation.
 *
 * This function initializes Timer A2 to generate PWM signals on specific pins. It configures the timer's clock source,
 * prescale value, and PWM mode settings.
 *
 * @param period The PWM signal period, in timer ticks.
 * @param duty_cycle_1 The duty cycle for PWM signal, P5.6 (PM_TA2.1)
 * @param duty_cycle_2 The duty cycle for PWM signal, P5.7 (PM_TA2.2)
 *
 * @return None
 */
void Timer_A2_PWM_Init(uint16_t period, uint16_t duty_cycle_1, uint16_t duty_cycle_2);

/**
 * @brief Update the Timer A2 duty cycle for PWM signal, P5.6 (PM_TA2.1)
 *
 * @param duty_cycle_1 The new duty cycle for PWM signal, P5.6 (PM_TA2.1)
 *
 * @return None
 */
void Timer_A2_Update_Duty_Cycle_1(uint16_t duty_cycle_1);

/**
 * @brief Update the Timer A2 duty cycle for PWM signal, P5.7 (PM_TA2.2)
 *
 * @param duty_cycle_2 The new duty cycle for PWM signal, P5.7 (PM_TA2.2)
 *
 * @return None
 */
void Timer_A2_Update_Duty_Cycle_2(uint16_t duty_cycle_2);

#endif /* TIMER_A2_PWM_H_ */
