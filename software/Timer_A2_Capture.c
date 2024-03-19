/**
 * @file Timer_A2_Capture.c
 * @brief Source code for the Timer_A2_Capture driver.
 *
 * This file contains the function definitions for configuring Timer A2 in Capture mode.
 *
 * @author Aaron Nanas
 *
 */

#include "../inc/Timer_A2_Capture.h"

// User-defined function
void (*Timer_A2_Capture_Task_1)(uint16_t time);

// Initialize Timer A2 in Capture mode to request interrupts on the rising edge
// of the external signal. The ISR will acknowledge the interrupt and calls a user-defined function
void Timer_A2_Capture_Init(void(*task)(uint16_t time))
{
    // Assign task 0 to user function
    Timer_A2_Capture_Task_1 = task;

    // Configure pin P5.6 (TA2.1) to peripheral function mode
    P5->SEL0 |= 0x40;
    P5->SEL1 &= ~0x40;

    // Set the direction of P5.6 as input
    P5->DIR &= ~0x40;

    // Halt Timer A2
    TIMER_A2->CTL &= ~0x0030;

    // Select SMCLK = 12 MHz as timer clock source
    // Set ID = 0 (Divide timer clock by 1)
    TIMER_A2->CTL = 0x0200;

    // Capture on Rising Edge (Bit 15-14 = 01)
    // Synchronous Capture Source (Bit 11 = 1)
    // Capture Mode (Bit 8 = 1)
    // Capture / Compare Interrupt Enable (Bit 4 = 1)
    // Clear Capture / Compare Interrupt Flag (Bit 0 = 0)
    TIMER_A2->CCTL[1] = 0x4910;

    // Configure the Timer A2 expansion register to divide the clock frequency by 1
    TIMER_A2->EX0 = 0x0000;

    // Set Interrupt Priority Level to 3
    NVIC->IP[3] = (NVIC->IP[3] & 0xFFFF0FFF) | 0x00006000;

    // Enable Interrupt 13 in NVIC
    NVIC->ISER[0] = 0x00002000;

    // Reset and start Timer A2 in Continuous mode
    TIMER_A2->CTL |= 0x0024;
}

void TA2_N_IRQHandler(void)
{
    // Acknowledge Capture/Compare interrupt and clear it
    TIMER_A2->CCTL[1] &= ~0x0001;

    // Execute the user-defined task
    (*Timer_A2_Capture_Task_1)(TIMER_A2->CCR[1]);
}
