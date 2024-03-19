/**
 * @file EUSCI_A2_UART.h
 * @brief Header file for the EUSCI_A2_UART driver.
 *
 * This file contains the function definitions for the EUSCI_A2_UART driver.
 *
 * @note Assumes that the necessary pin configurations for UART communication have been performed
 *       on the corresponding pins. P3.2 is used for UART RX while P3.3 is used for UART TX.
 *       For this lab, only the P3.3 pin (UART TX) will be used to observe the transmitted data using an oscilloscope.
 *
 * For more information regarding the Enhanced Universal Serial Communication Interface (eUSCI),
 * refer to the MSP432Pxx Microcontrollers Technical Reference Manual
 *
 * @author Aaron Nanas
 *
 */

#ifndef EUSCI_A2_UART_H_
#define EUSCI_A2_UART_H_

#include <stdint.h>
#include "msp.h"

/**
 * @brief Initializes the UART module EUSCI_A2 for communication.
 *
 * This function configures the EUSCI_A2 module to enable UART communication
 * with the following configuration:
 *
 * - Parity: Disabled
 * - Stop bits: 1
 * - Data bits: 8
 * - Baud rate: 115200
 * - Mode: UART
 * - MSB first
 * - UART clock source: SMCLK
 * - Interrupts disabled
 *
 * For more information regarding the registers used, refer to the eUSCI_A UART Registers section (24.4)
 * of the MSP432Pxx Microcontrollers Technical Reference Manual
 *
 * @note This function assumes that the necessary pin configurations for UART communication have been performed
 *       on the corresponding pins. P3.2 is used for UART RX while P3.3 is used for UART TX.
 *
 * @return None
 */
void EUSCI_A2_UART_Init();

/**
 * @brief Transmits a single character over UART using the EUSCI_A2 module.
 *
 * This function transmits a single character over UART using the EUSCI_A2 module.
 * It waits until the UART transmit buffer is ready to accept new data and then writes the provided data
 * to the transmit buffer for transmission. A character consists of the following bits:
 *
 * - 1 Start Bit
 * - 8 Data Bits
 * - 1 Stop Bit
 *
 * @param data The unsigned 8-bit data to be transmitted over UART.
 *
 * @return None
 */
void EUSCI_A2_UART_OutChar(uint8_t data);

/**
 * @brief Receives a single character over UART using the EUSCI_A2 module.
 *
 * This function receives a single character over UART using the EUSCI_A2 module.
 * It waits until a character is available in the UART receive buffer and then reads
 * the received data. A character consists of the following bits:
 *
 * - 1 Start Bit
 * - 8 Data Bits
 * - 1 Stop Bit
 *
 * @return The received unsigned 8-bit data from UART.
 */
uint8_t EUSCI_A2_UART_InChar();

#endif /* EUSCI_A2_UART_H_ */
