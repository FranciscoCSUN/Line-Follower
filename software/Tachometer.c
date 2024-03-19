/**
 * @file Tachometer.c
 * @brief Source code for the Tachometer driver.
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

#include "../inc/Tachometer.h"

uint16_t Tachometer_FirstRightTime;
uint16_t Tachometer_SecondRightTime;

uint16_t Tachometer_FirstLeftTime;
uint16_t Tachometer_SecondLeftTime;

// Incremented with every step forward. Decremented with every step backward
int Tachometer_RightSteps = 0;

// Incremented with every step forward. Decremented with every step backward
int Tachometer_LeftSteps = 0;

enum Tachometer_Direction Tachometer_RightDir = STOPPED;
enum Tachometer_Direction Tachometer_LeftDir = STOPPED;

void Tachometer_Right_Int(uint16_t current_time)
{
    // Store the time of the previous rising edge for the right wheel
    Tachometer_FirstRightTime = Tachometer_SecondRightTime;

    // Store the time of the current rising edge for the right wheel
    Tachometer_SecondRightTime = current_time;

    if ((P5->IN & 0x01) == 0)
    {
        // If Encoder B is low, then the motor is moving backward
        Tachometer_RightSteps = Tachometer_RightSteps - 1;
        Tachometer_RightDir = REVERSE;
    }
    else
    {
        // If Encoder B is high, then the motor is moving forward
        Tachometer_RightSteps = Tachometer_RightSteps + 1;
        Tachometer_RightDir = FORWARD;
    }
}

void Tachometer_Left_Int(uint16_t current_time)
{
    // Store the time of the previous rising edge for the left wheel
    Tachometer_FirstLeftTime = Tachometer_SecondLeftTime;

    // Store the time of the current rising edge for the left wheel
    Tachometer_SecondLeftTime = current_time;

    if ((P5->IN & 0x04) == 0)
    {
        // If Encoder B is low, then the motor is moving backward
        Tachometer_LeftSteps = Tachometer_LeftSteps - 1;
        Tachometer_LeftDir = REVERSE;
    }

    else
    {
        // If Encoder B is high, then the motor is moving forward
        Tachometer_LeftSteps = Tachometer_LeftSteps + 1;
        Tachometer_LeftDir = FORWARD;
    }
}

void Tachometer_Init()
{
    // Configure pins P5.0 and P5.2 as GPIO inputs
    P5->SEL0 &= ~0x05;
    P5->SEL1 &= ~0x05;
    P5->DIR &= ~0x05;

    // Initialize Timer A3 Capture
    Timer_A3_Capture_Init(&Tachometer_Right_Int, &Tachometer_Left_Int);
}

void Tachometer_Get(uint16_t *left_tach,
                    enum Tachometer_Direction *left_dir,
                    int32_t *left_steps,
                    uint16_t *right_tach,
                    enum Tachometer_Direction *right_dir,
                    int32_t *right_steps)
{
    *left_tach = (Tachometer_SecondLeftTime - Tachometer_FirstLeftTime);
    *left_dir = Tachometer_LeftDir;
    *left_steps = Tachometer_LeftSteps;
    *right_tach = (Tachometer_SecondRightTime - Tachometer_FirstRightTime);
    *right_dir = Tachometer_RightDir;
    *right_steps = Tachometer_RightSteps;
}
