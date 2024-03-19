/**
 * @file Timer_A1_Interrupt.c
 * @brief Source code for the Timer_A1_Interrupt driver.
 *
 * This file contains the function definitions for the Timer_A1_Interrupt driver.
 * It uses the Timer_A1 timer to generate periodic interrupts at a specified rate.
 *
 * @note The interrupt rate has been modified to 2 kHz for the Tachometer / ADC Lab.
 *
 * @note The interrupt rate has been modified to 1 kHz for the I2C Lab.
 *
 * @author Aaron Nanas
 *
 */

#include "../inc/Timer_A1_Interrupt.h"

void Timer_A1_Interrupt_Init(void(*task)(void), uint16_t period)
{
    // Store the user-defined task function for use during interrupt handling
    Timer_A1_Task = task;

    // Halt Timer A1 by clearing MC bits
    TIMER_A1->CTL &= ~0x0030;

    // Choose SMCLK as timer clock source (TASSEL = 10b)
    // Choose prescale value of 1 (ID = 0)
    TIMER_A1->CTL |= 0x0200;

    // Enable interrupt request of the
    // corresponding Capture/Compare interrupt flag
    TIMER_A1->CCTL[0] |= 0x0010;

    // Store the period in the CCR0 register
    // Note: Timer starts counting from 0
    TIMER_A1->CCR[0] = (period - 1);

    // Divide the SMCLK frequency by 1
    TIMER_A1->EX0 = 0x0000;

    // Set interrupt priority level to 2
    NVIC->IP[2] = (NVIC->IP[2]&0xFF00FFFF) | 0x00600000;

    // Enable Interrupt 10 in NVIC
    NVIC->ISER[0] |= 0x00000400;

    // Set the TACLR bit and enable Timer A1 in up mode
    TIMER_A1->CTL |= 0x0014;
}

void TimerA1_Stop(void)
{
    // Halt Timer A1 by clearing MC bits
    TIMER_A1->CTL &= ~0x0030;

    // Disable Interrupt 10 in NVIC
    NVIC->ICER[0] = 0x00000400;
}

void TA1_0_IRQHandler(void)
{
    // Acknowledge Capture/Compare interrupt and clear it
    TIMER_A1->CCTL[0] &= ~0x0001;

    // Execute the user-defined task
    (*Timer_A1_Task)();
}
