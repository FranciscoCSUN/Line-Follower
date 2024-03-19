/**
 * @file Tachometer.h
 * @brief Header file for the Tachometer driver.
 *
 * This file contains the function definitions for interfacing with the tachometers.
 * The Tachometer driver initializes pins P5.0 and P5.2 as GPIO inputs which are used to determine
 * the direction of the motor rotation. In addition, it also initializes the Timer A3 module
 * to measure the speed of the motor rotation.
 *
 * @author Jonathan W. Valvano, Aaron Nanas
 *
 * @note Original Tachometer driver written by Jonathan W. Valvano
 *
 * @note This driver has been modified by Aaron Nanas to clean up the code
 *       and update documentation. Most of the logic has been preserved.
 */

#ifndef TACHOMETER_H_
#define TACHOMETER_H_

#include <stdint.h>
#include "msp.h"
#include "../inc/Clock.h"
#include "../inc/Timer_A3_Capture.h"

/**
 * @brief Indicates the direction of the motor rotation relative to the front of the robot
 */
enum Tachometer_Direction
{
    FORWARD,
    STOPPED,
    REVERSE
};

/**
 * @brief Initialize tachometer interface.
 *
 * This function initializes pins P5.0 and P5.2 as GPIO inputs, which will
 * be used to determine the direction of the motor rotation. It also
 * initializes the Timer A3 module which will be used to measure the speed
 * of the motor rotation.
 *
 * @param None
 *
 * @return None
 */
void Tachometer_Init();

/**
 * @brief Get tachometer measurements.
 *
 * @param left_tach:    Pointer to store the last measured tachometer period of the left wheel (units of 83.3 ns)
 * @param left_dir:     Pointer to store the enumerated direction of last movement of the left wheel
 * @param left_steps:   Pointer to store the total number of forward steps measured for the left wheel (360 steps per ~220 mm circumference)
 * @param right_tach:   Pointer to store the last measured tachometer period of the right wheel (units of 83.3 ns)
 * @param right_dir:    Pointer to store the enumerated direction of last movement of the right wheel
 * @param right_steps:  Pointer to store the total number of forward steps measured for the right wheel (360 steps per ~220 mm circumference)
 *
 * @note Assumes Tachometer_Init() has been called
 *
 * @note Assumes Clock_Init48MHz() has been called
 *
 * @return None
 */
void Tachometer_Get(uint16_t *left_tach,
                    enum Tachometer_Direction *left_dir,
                    int32_t *left_steps,
                    uint16_t *right_tach,
                    enum Tachometer_Direction *right_dir,
                    int32_t *right_steps);

#endif /* TACHOMETER_H_ */
