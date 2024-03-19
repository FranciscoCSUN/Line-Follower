/**
 * @file Timer_A3_Capture.c
 * @brief Source code for the Timer_A3_Capture driver.
 *
 * This file contains the function definitions for configuring Timer A3 in Capture mode.
 * with interrupts enabled. External signals are connected to P10.4 and P10.5.
 * The interrupts are triggered when the rising edges of P10.4 and P10.5 are detected.
 *
 * Timer A3 is used as a base driver for the Tachometer driver.
 *
 * @author Jonathan W. Valvano, Aaron Nanas
 *
 * @note Original Timer A3 driver written by Jonathan W. Valvano
 *
 * @note This driver has been modified by Aaron Nanas to clean up the code,
 *       remove unnecessary logic, and update documentation
 */

#include "../inc/Timer_A3_Capture.h"

// Placeholder function
void User_Function(uint16_t t) {};

// Capture task 0 pointer to user function
void (*Capture_Task_0)(uint16_t time) = User_Function;

// Capture task 1 pointer to user function
void (*Capture_Task_1)(uint16_t time) = User_Function;

void Timer_A3_Capture_Init(void(*task0)(uint16_t time), void(*task1)(uint16_t time))
{
    // Assign task 0 to user function
    Capture_Task_0 = task0;

    // Assign task 1 to user function
    Capture_Task_1 = task1;

    // Configure pins P10.4 (TA3.0) and P10.5 (TA3.1) to peripheral function mode
    P10->SEL0 |= 0x30;
    P10->SEL1 &= ~0x30;

    // Set the direction of P10.4 and P10.5 as input
    P10->DIR &= ~0x30;

    // Halt Timer A3
    TIMER_A3->CTL &= ~0x0030;

    // Set the timer clock source to SMCLK = 12 MHz
    TIMER_A3->CTL = 0x0200;

    // Capture on Rising Edge (Bit 15-14 = 01)
    // Synchronous Capture Source (Bit 11 = 1)
    // Capture Mode (Bit 8 = 1)
    // Capture / Compare Interrupt Enable (Bit 4 = 1)
    // Clear Capture / Compare Interrupt Flag (Bit 0 = 0)
    TIMER_A3->CCTL[0] = 0x4910;

    // Capture on Rising Edge (Bit 15-14 = 01)
    // Synchronous Capture Source (Bit 11 = 1)
    // Capture Mode (Bit 8 = 1)
    // Capture / Compare Interrupt Enable (Bit 4 = 1)
    // Clear Capture / Compare Interrupt Flag (Bit 0 = 0)
    TIMER_A3->CCTL[1] = 0x4910;

    // Configure the Timer A3 expansion register to divide the clock frequency by 1
    TIMER_A3->EX0 &= ~0x0007;

    // Set Interrupt Priority Level to 2
    NVIC->IP[14] = 0x40;

    // Set Interrupt Priority Level to 2
    NVIC->IP[15] = 0x40;

    // Enable Interrupt 14 and 15 in NVIC
    NVIC->ISER[0] = 0x0000C000;

    // Reset and start Timer A3 in Continuous mode
    TIMER_A3->CTL |= 0x0024;
}

void TA3_0_IRQHandler(void)
{
    // Acknowledge Capture/Compare interrupt and clear it
    TIMER_A3->CCTL[0] &= ~0x0001;

    // Execute the user-defined task
    (*Capture_Task_0)(TIMER_A3->CCR[0]);
}

void TA3_N_IRQHandler(void)
{
    // Acknowledge Capture/Compare interrupt and clear it
    TIMER_A3->CCTL[1] &= ~0x0001;

    // Execute the user-defined task
    (*Capture_Task_1)(TIMER_A3->CCR[1]);
}
