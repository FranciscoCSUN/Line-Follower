/**
 * @file PMOD_BTN_Interrupt.c
 * @brief Source code for the PMOD_BTN_Interrupt driver.
 *
 * This file contains the function definitions for the PMOD_BTN_Interrupt driver.
 * It interfaces with the PMOD BTN module and it is used to demonstrate external I/O-triggered interrupts.
 *
 * For more information regarding the PMODs used in this lab, visit the following links:
 *  - PMOD BTN: https://digilent.com/reference/pmod/pmodbtn/reference-manual
 *
 * The following pins are used when the Bumper Sensors are connected to the TI MSP432 LaunchPad:
 *  - PMOD BTN0         <-->    MSP432 LaunchPad Pin P6.0
 *  - PMOD BTN1         <-->    MSP432 LaunchPad Pin P6.1
*   - PMOD BTN2         <-->    MSP432 LaunchPad Pin P6.2
 *  - PMOD BTN3         <-->    MSP432 LaunchPad Pin P6.3
 *  - PMOD GND          <-->    MSP432 LaunchPad GND
 *  - PMOD VCC          <-->    MSP432 LaunchPad VCC (3.3V)
 *
 * @note The PMOD 8LD module operates in an active high configuration.
 *
 * @author Aaron Nanas
 *
 */

#include "../inc/PMOD_BTN_Interrupt.h"

// PMOD BTN should not be used if OPT3101 is being used
// to avoid any interrupt conflict
#define OPT3101_ACTIVE 1

void PMOD_BTN_Interrupt_Init(void(*task)(uint8_t))
{
    // Store the user-defined task function for use during interrupt handling
    PMOD_BTN_Task = task;

    // Configure the following pins as GPIO pins: P6.0, P6.1, P6.2, and P6.3
    P6->SEL0 &= ~0x0F;
    P6->SEL1 &= ~0x0F;

    // Set the following pins as input: P6.0, P6.1, P6.2, and P6.3
    P6->DIR &= ~0x0F;

    // Enable pull-down resistors on the following pins: P6.0, P6.1, P6.2, and P6.3
    P6->REN |= 0x0F;

    // Ensure that the pins are pulled down: P6.0, P6.1, P6.2, and P6.3
    P6->OUT &= ~0x0F;

#ifndef OPT3101_ACTIVE
    // Interrupt Edge Select: Low-to-High Transition
    // Configure the pins to use rising edge event triggers: P6.0, P6.1, P6.2, and P6.3
    P6->IES &= ~0x0F;

    // Clear any existing interrupt flags
    P6->IFG &= ~0x0F;

    // Enable interrupts on the following pins: P6.0, P6.1, P6.2, and P6.3
    P6->IE |= 0x0F;

    // Set the priority of the interrupts (IRQ 40) to 0 (section 2.4.3.21)
    NVIC->IP[10] = (NVIC->IP[10] & 0xFFFFFF0F);

    // Enable Interrupt 40 in NVIC (section 2.4.3.2)
    // Bit 8 corresponds to IRQ 40
    NVIC->ISER[1] = 0x00000100;
#endif
}

uint8_t PMOD_BTN_Read(void)
{
    // Declare a local variable to store the input register value
    // Then, read the input register P6->IN
    // The PMOD BTN operates in an active high configuration
    // A "0x0F" bit mask is used to capture only the relevant bits
    uint8_t pmod_btn_state = P6->IN & 0x0F;

    // Return the state of the PMOD BTN
    return pmod_btn_state;
}

#ifndef OPT3101_ACTIVE
void PORT6_IRQHandler(void)
{
    // Clear the interrupt flags for P6.0 - P6.3
    P6->IFG &= ~0x0F;

    // Execute the user-defined task
    (*PMOD_BTN_Task)(PMOD_BTN_Read());
}
#endif
