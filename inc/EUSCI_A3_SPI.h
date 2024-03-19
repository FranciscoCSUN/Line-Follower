/**
 * @file EUSCI_A3_SPI.h
 * @brief Header file for the EUSCI_A3_SPI driver.
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

#ifndef EUSCI_A3_SPI_H_
#define EUSCI_A3_SPI_H_

#include <stdio.h>
#include <stdint.h>
#include "msp.h"

/**
 * @brief Initializes the SPI module EUSCI_A3 for communication.
 *
 * This function configures the EUSCI_A3 module to enable SPI communication
 * with the following configuration:
 *
 * - CTLW0 Register Configuration:
 *
 *  Bit(s)      Field       Value       Description
 *  -----       -----       -----       -----------
 *   15         UCCKPH       0x1        Data is captured on the first edge and changed on the following edge
 *   14         UCCKPL       0x0        Clock is low when inactive
 *   13         UCMSB        0x1        MSB first
 *   12         UC7BIT       0x0        8-bit data
 *   11         UCMST        0x1        Master mode is selected
 *   10-9       UCMODEx      0x2        4-pin SPI with active low UCSTE
 *   8          UCSYNC       0x1        Synchronous mode
 *   7-6        UCSSELx      0x2        eUSCI clock source is SMCLK
 *   5-2        Reserved     0x0        Reserved
 *   1          UCSTEM       0x1        UCSTE pin is used to generate signal for 4-wire slave
 *   0          UCSWRST      0x1        eUSCI logic held in reset state
 *
 * - Clock Frequency: 1 MHz (refer to Page 20 of Nokia 5110 LCD datasheet)
 * - Interrupts disabled
 *
 * For more information regarding the registers used, refer to the eUSCI_A SPI Registers section (25.4)
 * of the MSP432Pxx Microcontrollers Technical Reference Manual
 *
 * @note This function assumes that the necessary pin configurations for SPI communication have been performed
 *       on the corresponding pins.
 *       - P9.4 (SCE, Chip Enable)
 *       - P9.5 (SCLK)
 *       - P9.7 (MOSI, Master Out Slave In)
 *
 * @return None
 */
void EUSCI_A3_SPI_Init();

/**
 * @brief The EUSCI_A3_SPI_Command_Write function writes a command byte to the SPI transmit buffer.
 *
 * This function writes a command byte to the transmit buffer of the EUSCI_A3 SPI module.
 * It waits until the SPI is not busy before writing the command byte.
 *
 * @param command The command byte to be written.
 *
 * @return None
 */
void EUSCI_A3_SPI_Command_Write(uint8_t command);

/**
 * @brief The EUSCI_A3_SPI_Data_Write function writes a data byte to the SPI transmit buffer.
 *
 * This function writes a data byte to the transmit buffer of the EUSCI_A3 SPI module.
 * It waits until the transmit buffer is empty before writing the data byte.
 *
 * @param data The data byte to be written.
 *
 * @return None
 */
void EUSCI_A3_SPI_Data_Write(uint8_t data);

#endif /* EUSCI_A3_SPI_H_ */
