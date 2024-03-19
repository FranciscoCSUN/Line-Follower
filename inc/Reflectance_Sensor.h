/**
 * @file Reflectance_Sensor.h
 * @brief Header file for the Reflectance_Sensor driver.
 *
 * This file contains the function definitions for the Reflectance_Sensor driver.
 * It uses GPIO to interface with the 8-Channel QTRX Sensor Array module.
 *
 * The 8-Channel QTRX Sensor Array module uses the following pinout:
 *  - Reflectance Sensor Output 0   <-->  MSP432 LaunchPad Pin P7.0
 *  - Reflectance Sensor Output 1   <-->  MSP432 LaunchPad Pin P7.1
 *  - Reflectance Sensor Output 2   <-->  MSP432 LaunchPad Pin P7.2
 *  - Reflectance Sensor Output 3   <-->  MSP432 LaunchPad Pin P7.3
 *  - Reflectance Sensor Output 4   <-->  MSP432 LaunchPad Pin P7.4
 *  - Reflectance Sensor Output 5   <-->  MSP432 LaunchPad Pin P7.5
 *  - Reflectance Sensor Output 6   <-->  MSP432 LaunchPad Pin P7.6
 *  - Reflectance Sensor Output 7   <-->  MSP432 LaunchPad Pin P7.7
 *  - Reflectance Sensor Ctrl Even  <-->  MSP432 LaunchPad Pin P5.3
 *  - Reflectance Sensor Ctrl Odd   <-->  MSP432 LaunchPad Pin P9.2
 *  - Reflectance Sensor VCC        <-->  MSP432 LaunchPad 5V
 *  - Reflectance Sensor GND        <-->  MSP432 LaunchPad GND
 *
 * @note The typical sequence for reading the reflectance sensors is as follows:
 *          1. Turn on the IR LEDs
 *          2. Configure the sensor output pins as output GPIO and drive them high
 *          3. Allow at least 10 us for the sensor output to rise
 *          4. Configure the sensor output pins as input GPIO
 *          5. Measure the time for the voltage to decay by waiting for the GPIO lines to go low
 *          6. Turn off the IR LEDs
 *
 * @note For more information regarding the 8-Channel QTRX Sensor Array module,
 * refer to the product page: https://www.pololu.com/product/3672
 *
 * @author Aaron Nanas
 *
 */

#ifndef REFLECTANCE_SENSOR_H_
#define REFLECTANCE_SENSOR_H_

#include <stdint.h>
#include "msp.h"
#include "../inc/Clock.h"

/**
 * @brief Initializes the 8-Channel QTRX Sensor Array module.
 *
 * This function configures the necessary GPIO pins and settings to initialize the 8-Channel QTRX Sensor Array module.
 * It sets up the IR LEDs to enable the detection of a reflective surface.
 *
 * @return None
 */
void Reflectance_Sensor_Init();

/**
 * @brief Reads the eight sensors from the 8-Channel QTRX Sensor Array module.
 *
 * This function performs the following steps:
 * 1. Turns on the 8 IR LEDs.
 * 2. Pulses the 8 sensors high for 10 microseconds.
 * 3. Makes the sensor pins input.
 * 4. Waits for a specified time in microseconds.
 * 5. Reads the sensors.
 * 6. Turns off the 8 IR LEDs.
 *
 * @param time The time to wait in microseconds before reading the sensors.
 *
 * @return Sensor readings (8-bit value). "1" indicates black, while "0" indicates white.
 *
 * @note Assumes that Reflectance_Init() has been called to initialize the sensor module.
 */
uint8_t Reflectance_Sensor_Read(uint32_t time);


/**
 * @brief Reads the two center sensors from the 8-Channel QTRX Sensor Array module.
 *
 * This function utilizes Reflectance_Sensor_Read() to get sensor readings and
 * determines the position relative to the center of the line based on the readings.
 *
 * @param time The time to wait in microseconds before reading the sensors.
 *
 * @return Position relative to the center of the line:
 *         - 0: Off-road
 *         - 1: Off to the left
 *         - 2: Off to the right
 *         - 3: On the road
 *
 * @note Assumes that Reflectance_Init() has been called to initialize the sensor module.
 */
uint8_t Reflectance_Sensor_Center(uint32_t time);

/**
 * @brief Performs sensor integration for the 8-Channel QTRX Sensor Array module.
 *
 * This function takes an 8-bit sensor reading and calculates the position
 * in 0.1mm relative to the center of the line based on predefined weight values.
 *
 * @param data 8-bit result from the line sensor.
 *
 * @return Position in 0.1mm relative to the center of the line.
 */
int32_t Reflectance_Sensor_Position(uint8_t data);

/**
 * @brief Begins the process of reading the eight sensors from the 8-Channel QTRX Sensor Array module.
 *
 * This function performs the following steps:
 *  1. Turns on the 8 IR LEDs.
 *  2. Pulses the 8 sensors high for 10 microseconds.
 *  3. Makes the sensor pins input.
 *
 * @note Assumes that Reflectance_Init() has been called to initialize the sensor module.
 */
void Reflectance_Sensor_Start();

/**
 * @brief Finishes reading the eight sensors from the 8-Channel QTRX Sensor Array module.
 *
 * This function reads the sensors and turns off the 8 IR LEDs.
 *
 * @return Sensor readings (8-bit value). "1" indicates black, while "0" indicates white.
 *
 * @note Assumes that Reflectance_Init() has been called to initialize the sensor module.
 *
 * @note Assumes that Reflectance_Start() was called 1 ms ago.
 */
uint8_t Reflectance_Sensor_End();

#endif /* REFLECTANCE_SENSOR_H_ */
