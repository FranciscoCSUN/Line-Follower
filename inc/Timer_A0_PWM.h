/**
 * @file Timer_A0_PWM.h
 * @brief Header file for the Timer_A0_PWM driver.
 *
 * This file contains the function definitions for configuring Timer A0 in Pulse Width Modulation (PWM) mode.
 * It provides functions for initializing the PWM timer, updating duty cycles for PWM signals, and controlling
 * the PWM output on specific pins.
 *
 * @note The Motor driver uses Timer_A0_PWM as a base driver.
 *
 * @author Aaron Nanas
 *
 */

#ifndef TIMER_A0_PWM_H_
#define TIMER_A0_PWM_H_

#include <stdint.h>
#include "msp.h"

/**
 * @brief Initialize Timer A0 for PWM signal generation.
 *
 * This function initializes Timer A0 to generate PWM signals on pins P2.6 (PM_TA0.3) and P2.7 (PM_TA0.4).
 * It configures the PWM period, duty cycle values for two channels (CCR3 and CCR4), and the clock source.
 * Additionally, it sets up pins P2.6 and P2.7 for peripheral function mode and configures them as outputs.
 *
 * @param period The PWM period in timer ticks.
 * @param duty_cycle_1 The duty cycle for CCR3, in timer ticks.
 * @param duty_cycle_2 The duty cycle for CCR4, in timer ticks.
 *
 * @note The Timer_A0_PWM_Init function should be called before using Timer A0 PWM control.
 * @note Ensure that Timer A0 peripheral and pins are properly configured in the system.
 *
 * @return None
 */
void Timer_A0_PWM_Init(uint16_t period, uint16_t duty_cycle_1, uint16_t duty_cycle_2);

/**
 * @brief Update the duty cycle for CCR3 of Timer A0.
 *
 * This function updates the duty cycle value for CCR3 of Timer A0 without affecting other settings.
 * It immediately returns if the provided duty cycle value is greater than or equal to the configured PWM period.
 *
 * @param duty_cycle_1 The new duty cycle value for CCR3, in timer ticks.
 *
 * @note The Timer_A0_PWM_Init function should be called before using this function.
 *
 * @return None
 */
void Timer_A0_Update_Duty_Cycle_1(uint16_t duty_cycle_1);

/**
 * @brief Update the duty cycle for CCR4 of Timer A0.
 *
 * This function updates the duty cycle value for CCR4 of Timer A0 without affecting other settings.
 * It immediately returns if the provided duty cycle value is greater than or equal to the configured PWM period.
 *
 * @param duty_cycle_2 The new duty cycle value for CCR4, in timer ticks.
 *
 * @note The Timer_A0_PWM_Init function should be called before using this function.
 *
 * @return None
 */
void Timer_A0_Update_Duty_Cycle_2(uint16_t duty_cycle_2);

#endif /* TIMER_A0_PWM_H_ */
