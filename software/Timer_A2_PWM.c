/**
 * @file Timer_A2_PWM.h
 * @brief Source code for the Timer_A2_PWM driver.
 *
 * This file contains the function definitions for configuring Timer A2 in Pulse Width Modulation (PWM) mode.
 * It provides functions for initializing the PWM timer and the updating duty cycle for PWM signals.
 *
 * @author Aaron Nanas
 *
 */

#include "../inc/Timer_A2_PWM.h"

void Timer_A2_PWM_Init(uint16_t period, uint16_t duty_cycle_1, uint16_t duty_cycle_2)
{
    // Return immediately if either duty cycle values are greater than
    // or equal to the given period
    if (duty_cycle_1 >= period) return;
    if (duty_cycle_2 >= period) return;

    // Configure pins P5.6 (PM_TA2.1) and P5.7 (PM_TA2.2) to peripheral function mode
    P5->SEL0 |= 0xC0;
    P5->SEL1 &= ~0xC0;

    // Configure pins P5.6 and P5.7 as output to drive the PWM signals
    P5->DIR |= 0xC0;

    // Set the Timer A2 Capture/Compare register to the specified period
    // CCR[0] is primarily used as the "period" register
    // Actual formula: Period = (2*period) / (12 MHz / Prescale Value)
    // In this case: Period = (2*60000) / (12 MHz / 2) = 20 ms
    TIMER_A2->CCR[0] = period;

    // Configure the Timer A2 expansion register to divide the clock frequency by 1
    TIMER_A2->EX0 = 0x0000;

    // Configure CCR1 as Toggle / Reset
    TIMER_A2->CCTL[1] = 0x0040;

    // Set duty cycle in CCR[1]
    // Duty Cycle %: duty_cycle_1 / period
    TIMER_A2->CCR[1] = duty_cycle_1;

    // Configure CCR2 as Toggle / Reset
    TIMER_A2->CCTL[2] = 0x0040;

    // Set duty cycle in CCR[2]
    // Duty Cycle %: duty_cycle_1 / period
    TIMER_A2->CCR[2] = duty_cycle_2;

    // Select SMCLK = 12 MHz as timer clock source
    // Set ID = 1 (Divide timer clock by 2)
    // Set MC = 3 (Up/Down Mode)
    TIMER_A2->CTL = 0x0270;
}

void Timer_A2_Update_Duty_Cycle_1(uint16_t duty_cycle_1)
{
    // Immediately return if duty cycle is greater than the given period
    if (duty_cycle_1 >= TIMER_A2->CCR[0]) return;

    // Otherwise, update the duty cycle
    TIMER_A2->CCR[1] = duty_cycle_1;
}

void Timer_A2_Update_Duty_Cycle_2(uint16_t duty_cycle_2)
{
    // Immediately return if duty cycle is greater than the given period
    if (duty_cycle_2 >= TIMER_A2->CCR[0]) return;

    // Otherwise, update the duty cycle
    TIMER_A2->CCR[2] = duty_cycle_2;
}
