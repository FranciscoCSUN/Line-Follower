/**
 * @file Reflectance_Sensor.c
 * @brief Source code for the Reflectance_Sensor driver.
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

#include "../inc/Reflectance_Sensor.h"

/**
 * @brief Weight values used for sensor integration in Reflectance_Sensor_Position().
 *
 * The Weight array contains pre-defined values corresponding to the influence of each sensor
 * in the 8-Channel QTRX Sensor Array module on the calculated position relative to the center of the line.
 * Positive values indicate sensors on the right side, while negative values indicate sensors on the left side.
 * The values are in units of 0.1mm.
 *
 * Weight[0]: Weight for the rightmost sensor (P7.0)
 * Weight[1]: Weight for the second-rightmost sensor (P7.1)
 * Weight[2]: Weight for the third-rightmost sensor (P7.2)
 * Weight[3]: Weight for the fourth-rightmost sensor (P7.3)
 * Weight[4]: Weight for the fourth-leftmost sensor (P7.4)
 * Weight[5]: Weight for the third-leftmost sensor (P7.5)
 * Weight[6]: Weight for the second-leftmost sensor (P7.6)
 * Weight[7]: Weight for the leftmost sensor (P7.7)
 */
const int32_t Weight[8] = {334, 238, 142, 48, -48, -142, -238, -334};

/**
 * @brief Bit masks used for sensor integration in Reflectance_Sensor_Position().
 *
 * The Mask array contains bit masks corresponding to each sensor in the 8-Channel QTRX Sensor Array module.
 * These masks are used to isolate individual sensor readings when performing sensor integration.
 *
 * Mask[0]: Bit mask for the rightmost sensor (P7.0)
 * Mask[1]: Bit mask for the second-rightmost sensor (P7.1)
 * Mask[2]: Bit mask for the third-rightmost sensor (P7.2)
 * Mask[3]: Bit mask for the fourth-rightmost sensor (P7.3)
 * Mask[4]: Bit mask for the fourth-leftmost sensor (P7.4)
 * Mask[5]: Bit mask for the third-leftmost sensor (P7.5)
 * Mask[6]: Bit mask for the second-leftmost sensor (P7.6)
 * Mask[7]: Bit mask for the leftmost sensor (P7.7)
 */
const int32_t Mask[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

void Reflectance_Sensor_Init()
{
    // Configure P5.3 as output GPIO
    P5->SEL0 &= ~0x08;
    P5->SEL1 &= ~0x08;
    P5->DIR |= 0x08;

    // Turn off even-numbered IR LEDs
    P5->OUT &= ~0x08;

    // Configure P9.2 as output GPIO
    P9->SEL0 &= ~0x04;
    P9->SEL1 &= ~0x04;
    P9->DIR |= 0x04;

    // Turn off odd-numbered IR LEDs
    P9->OUT &= ~0x04;

    // Configure P7.0 - P7.7 as input GPIO
    P7->SEL0 &= ~0xFF;
    P7->SEL1 &= ~0xFF;
    P7->DIR &= ~0xFF;
}

uint8_t Reflectance_Sensor_Read(uint32_t time)
{
    // Turn on even-numbered IR LEDs
    P5->OUT |= 0x08;

    // Turn on odd-numbered IR LEDs
    P9->OUT |= 0x04;

    // Configure P7.0 - P7.7 as output GPIO
    P7->DIR |= 0xFF;

    // Set P7.0 - P7.7 to high
    P7->OUT |= 0xFF;

    // Wait for 10 us
    Clock_Delay1us(10);

    // After waiting 10 us, configure P7.0 - P7.7 as input GPIO
    P7->DIR &= ~0xFF;

    // Wait for a specified amount of time
    Clock_Delay1us(time);

    // Retrieve the value of the line sensor array
    // Note: "1" indicates black while "0" indicates white
    uint8_t reflectance_value = P7->IN;

    // Turn off even-numbered IR LEDs
    P5->OUT &= ~0x08;

    // Turn off odd-numbered IR LEDs
    P9->OUT &= ~0x04;

    // Return the value of the line sensor array
    return reflectance_value;
}

uint8_t Reflectance_Sensor_Center(uint32_t time)
{
    return (Reflectance_Sensor_Read(time) & 0x18) >> 3;
}

int check_string = 0;
int32_t Reflectance_Sensor_Position(uint8_t data)
{
    int sum = 0;
    int count = 0;

    // Check if any reflectance sensors are active (i.e. data is non-zero)
    if (data)
    {
        // Iterate through each reflectance sensor
        for(int i = 0; i < 8; i++)
        {
            // Check if the sensor is active based on the bit position in the data
            if (data & Mask[i])
            {
                // Sum the weighted position value of the active sensor
                sum = sum + Weight[i];
                count++;
            }
        }

        // Calculate the average position based on the active sensors
        return sum/count;
    }
    else
    {
        // If no sensors are active, return a default position value
        return Weight[0] + 1;
    }
}

void Reflectance_Sensor_Start()
{
    // Turn on even-numbered IR LEDs
    P5->OUT |= 0x08;

    // Turn on odd-numbered IR LEDs
    P9->OUT |= 0x04;

    // Configure P7.0 - P7.7 as output GPIO
    P7->DIR |= 0xFF;

    // Set P7.0 - P7.7 to high
    P7->OUT = 0xFF;

    // Wait for 10 us
    Clock_Delay1us(10);

    // Configure P7.0 - P7.7 as input GPIO
    P7->DIR &= ~0xFF;
}

uint8_t Reflectance_Sensor_End()
{
    // Retrieve the value of the line sensor array
    // Note: "1" indicates black while "0" indicates white
    uint8_t reflectance_value = P7->IN;

    // Turn off even-numbered IR LEDs
    P5->OUT &= ~0x08;

    // Turn off odd-numbered IR LEDs
    P9->OUT &= ~0x04;

    // Return the value of the line sensor array
    return reflectance_value;
}
