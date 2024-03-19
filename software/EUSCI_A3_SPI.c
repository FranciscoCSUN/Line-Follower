/**
 * @file EUSCI_A3_SPI.c
 * @brief Source code for the EUSCI_A3_SPI driver.
 *
 * This file contains the function definitions for the EUSCI_A3_SPI driver.
 *
 * @note This function assumes that the necessary pin configurations for SPI communication have been performed
 *       on the corresponding pins. The output from the pins will be observed using an oscilloscope.
 *       - P9.4 (SCE, Chip Enable)
 *       - P9.5 (SCLK)
 *       - P9.7 (MOSI, Master Out Slave In)
 *
 * For more information regarding the Enhanced Universal Serial Communication Interface (eUSCI),
 * refer to the MSP432Pxx Microcontrollers Technical Reference Manual
 *
 * @author Aaron Nanas
 *
 */

#include "../inc/EUSCI_A3_SPI.h"

void EUSCI_A3_SPI_Init()
{
    // Hold the EUSCI_A3 module in reset mode
    EUSCI_A3->CTLW0 |= 0x01;

//     CTWL0 Register Configuration
//
//      Bit(s)      Field       Value       Description
//      -----       -----       -----       -----------
//       15         UCCKPH       0x1        Data is captured on the first edge and changed on the following edge
//       14         UCCKPL       0x0        Clock is low when inactive
//       13         UCMSB        0x1        MSB first
//       12         UC7BIT       0x0        8-bit data
//       11         UCMST        0x1        Master mode is selected
//       10-9       UCMODEx      0x2        4-pin SPI with active low UCSTE
//       8          UCSYNC       0x1        Synchronous mode
//       7-6        UCSSELx      0x2        eUSCI clock source is SMCLK
//       5-2        Reserved     0x0        Reserved
//       1          UCSTEM       0x1        UCSTE pin is used to generate signal for 4-wire slave
//       0          UCSWRST      0x1        eUSCI logic held in reset state
    EUSCI_A3->CTLW0 |= 0xAD83;

    // Set the baud rate. The clock frequency used is 1 MHz.
    // N = (Clock Frequency) / (Baud Rate) = (12,000,000 / 1,000,000)
    // N = 3
    EUSCI_A3->BRW |= 12;

    // Configure P9.4, P9.5, and P9.7 pins as primary module function
    P9->SEL0 |= 0xB0;
    P9->SEL1 &= ~0xB0;

    // Clear the software reset bit to enable the EUSCI_A3 module
    EUSCI_A3->CTLW0 &= ~0x01;

    // Ensure that the following interrupts are disabled:
    // - Receive Interrupt
    // - Transmit Interrupt
    EUSCI_A3->IE &= ~0x03;
}

void EUSCI_A3_SPI_Command_Write(uint8_t command)
{
    // UCBUSY - Wait until SPI is not busy
    while((EUSCI_A3->STATW & 0x0001) == 0x0001);

    // Write the command byte to the transmit buffer
    EUSCI_A3->TXBUF = command;

    // UCBUSY - Wait until SPI is not busy
    while((EUSCI_A3->STATW & 0x0001) == 0x0001);
}

void EUSCI_A3_SPI_Data_Write(uint8_t data)
{
    // Wait until UCA3TXBUF is empty
    while((EUSCI_A3->IFG & 0x0002) == 0x0000);

    // Write the data byte to the transmit buffer
    EUSCI_A3->TXBUF = data;
}
