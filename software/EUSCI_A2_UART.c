/**
 * @file EUSCI_A2_UART.c
 * @brief Source code for the EUSCI_A2_UART driver.
 *
 * This file contains the function definitions for the EUSCI_A2_UART driver.
 *
 * @note Assumes that the necessary pin configurations for UART communication have been performed
 *       on the corresponding pins. P3.2 is used for UART RX while P3.3 is used for UART TX.
 *       For this lab, the P3.3 pin (UART TX) will be used to observe the transmitted data using an oscilloscope.
 *
 * For more information regarding the Enhanced Universal Serial Communication Interface (eUSCI),
 * refer to the MSP432Pxx Microcontrollers Technical Reference Manual
 *
 * @author Aaron Nanas
 *
 */

#include "../inc/EUSCI_A2_UART.h"

void EUSCI_A2_UART_Init()
{
    // Hold the EUSCI_A2 module in reset mode
    EUSCI_A2->CTLW0 |= 0x01;

    // Set all the bits of the modulation control register to zero
    EUSCI_A2->MCTLW &= ~0xFF;

    // Hold the EUSCI_A2 module in reset mode
    // Set the clock source to SMCLK
    // MSB first
    EUSCI_A2->CTLW0 |= 0x20C1;

    // Set the baud rate
    // N = (Clock Frequency) / (Baud Rate) = (12,000,000 / 115,200) = 104.1667
    // Use only the integer part, so N = 104
    EUSCI_A2->BRW |= 104;

    // Configure P3.2 and P3.3 as primary module function
    P3->SEL0 |= 0x0C;
    P3->SEL1 &= ~0x0C;

    // Clear the software reset bit to enable the EUSCI_A2 module
    EUSCI_A2->CTLW0 &= ~0x01;

    // Ensure that the following interrupts are disabled:
    // - Receive Interrupt
    // - Transmit Interrupt
    // - Start Bit Interrupt
    // - Transmit Complete Interrupt
    EUSCI_A2->IE &= ~0x0F;
}

void EUSCI_A2_UART_OutChar(uint8_t data)
{
    while((EUSCI_A2->IFG & 0x02) == 0);
    EUSCI_A2->TXBUF = data;
}

uint8_t EUSCI_A2_UART_InChar()
{
    while((EUSCI_A2->IFG & 0x01) == 0);
    return EUSCI_A2->RXBUF;
}
