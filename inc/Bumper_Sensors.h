/**
 * @file Bump_Sensors.h
 * @brief Header file for the Bump_Sensors driver.
 *
 * This file contains the function definitions for the Bump_Sensors driver.
 * Using GPIO interrupts, the driver interfaces with the following:
 *  - Pololu Left Bumper Switches (https://www.pololu.com/product/3673)
 *  - Pololu Right Bumper Switches (https://www.pololu.com/product/3674)
 *
 * To verify the pinout used for the Pololu Bumper Sensors, refer to the MSP432P401R SimpleLink Microcontroller LaunchPad Development Kit User's Guide
 * Link: https://docs.rs-online.com/3934/A700000006811369.pdf
 *
 * The following pins are used when the Bumper Sensors are connected to the TI MSP432 LaunchPad:
 *  - BUMP_0                    <-->  MSP432 LaunchPad Pin P4.0
 *  - BUMP_1                    <-->  MSP432 LaunchPad Pin P4.2
 *  - BUMP_2                    <-->  MSP432 LaunchPad Pin P4.3
 *  - Right Bumper Sensor GND   <-->  MSP432 LaunchPad GND
 *  - BUMP_3                    <-->  MSP432 LaunchPad Pin P4.5
 *  - BUMP_4                    <-->  MSP432 LaunchPad Pin P4.6
 *  - BUMP_5                    <-->  MSP432 LaunchPad Pin P4.7
 *  - Left Bumper Sensor GND    <-->  MSP432 LaunchPad GND
 *
 * @note The Bumper Switches are configured with negative logic as the default setting.
 * When the switches are active, they connect to GND.
 *
 * @author Aaron Nanas
 *
 */

#ifndef BUMPER_SENSORS_H_
#define BUMPER_SENSORS_H_

#include <stdint.h>
#include "msp.h"

/**
 * @brief User-defined task function for handling Bumper Sensor interrupt events.
 *
 * This is a user-defined function that can be assigned to the Bumper_Task pointer during initialization.
 * When a falling edge event is detected on any of the Bump Sensor pins (P4.7, P4.6, P4.5, P4.3, P4.2, or P4.0),
 * this function will be called, and the bumper_sensor_state parameter will indicate which specific bumper switch triggered the interrupt.
 *
 * @param bumper_sensor_state An 8-bit unsigned integer representing the Bump Sensor that triggered the interrupt.
 *                   The bit positions correspond to the following sensors:
 *                   - Bit 7 (MSB): P4.7 Bump Sensor
 *                   - Bit 6: P4.6 Bump Sensor
 *                   - Bit 5: P4.5 Bump Sensor
 *                   - Bit 4: Reserved (unused)
 *                   - Bit 3: P4.3 Bump Sensor
 *                   - Bit 2: P4.2 Bump Sensor
 *                   - Bit 1: Reserved (unused)
 *                   - Bit 0 (LSB): P4.0 Bump Sensor
 *
 * @return None
 */
void (*Bumper_Task)(uint8_t bumper_sensor_state);

/**
 * @brief Initialize the Bumper Sensors and set up interrupt handling.
 *
 * This function initializes the bumper sensors and sets up the necessary configurations for interrupt handling.
 * The specified task function will be called when a falling edge event is detected on any of the following pins
 * used by the bumper sensors.
 *
 * The specified task function should take a single uint8_t parameter.
 *
 * @param task A pointer to the user-defined function that will be called on a falling edge event.
 *
 * @return None
 */
void Bumper_Sensors_Init(void(*task)(uint8_t));

/**
 * @brief Read the current state of the 6 Bumper Switches which have negative logic behavior.
 *
 * This function reads the state of 6 switches (BUMP_0 to BUMP_5) connected to the TI MSP432 LaunchPad's P4 port.
 * The switches are expected to have negative logic behavior, and their states are returned as a 6-bit result
 * represented as a uint8_t variable. Each bit of the result corresponds to one switch, where Bit 5 represents BUMP_5,
 * Bit 4 represents BUMP_4, and so on until Bit 0, which represents BUMP_0.
 *
 *  @note The Bumper Switches have negative logic behavior, where a logic high indicates a released (open) switch,
 *       and a logic low represents a pressed (closed) switch.
 *
 * @return uint8_t The 6-bit positive logic result representing the current state of the switches (0 to 63).
 */
uint8_t Bumper_Read(void);

void Handle_Collision(void);

#endif /* BUMPER_SENSORS_H_ */
