/**
 * @file PMOD_BTN_Interrupt.h
 * @brief Header file for the PMOD_BTN_Interrupt driver.
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

#ifndef PMOD_BTN_INTERRUPT_H_
#define PMOD_BTN_INTERRUPT_H_

#include <stdint.h>
#include "msp.h"

/**
 * @brief User-defined task function for handling PMOD BTN interrupt events.
 *
 * This is a user-defined function that can be assigned to the PMOD_BTN_Task pointer during initialization.
 * When a rising edge event is detected on any of the PMOD BTN pins (P6.0, P6.1, P6.2, and P6.3),
 * this function will be called, and the pmod_btn_state parameter will indicate which specific bumper switch triggered the interrupt.
 *
 * @param bumper_sensor An 8-bit unsigned integer representing the Bump Sensor that triggered the interrupt.
 *                   The bit positions correspond to the following sensors:
 *                   - Bit 0: P6.0 (PMOD BTN0)
 *                   - Bit 1: P6.1 (PMOD BTN1)
 *                   - Bit 2: P6.2 (PMOD BTN2)
 *                   - Bit 3: P6.3 (PMOD BTN3)
 *
 * @return None
 */
void (*PMOD_BTN_Task)(uint8_t pmod_btn_state);

/**
 * @brief Initialize the PMOD BTN module and set up interrupt handling.
 *
 * This function initializes the PMOD BTN module and sets up the necessary configurations for interrupt handling.
 * The specified task function will be called when a rising edge event is detected on any of the following pins
 * used by the PMOD BTN module.
 *
 * The specified task function should take a single uint8_t parameter.
 *
 * @param task A pointer to the user-defined function that will be called on a rising edge event.
 *
 * @return None
 */
void PMOD_BTN_Interrupt_Init(void(*task)(uint8_t));

/**
 * @brief Read the current state of the 4 push buttons on the PMOD BTN module which operate in an active high configuration.
 *
 * This function reads the state of the 4 push buttons on the PMOD BTN module (BTN0 to BTN3) connected to the TI MSP432 LaunchPad's P6 port.
 * The switches are expected to have positive logic behavior. Each bit of the result corresponds to one push button:
 *      - Bit 0: PMOD BTN0
 *      - Bit 1: PMOD BTN1
 *      - Bit 2: PMOD BTN2
 *      - Bit 3: PMOD BTN3
 *
 * @return uint8_t The 8-bit result representing the current state of the push buttons (0 to 15).
 */
uint8_t PMOD_BTN_Read(void);

#endif /* PMOD_BTN_INTERRUPT_H_ */
