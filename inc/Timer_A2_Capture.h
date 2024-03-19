/**
 * @file Timer_A2_Capture.h
 * @brief Header file for the Timer_A2_Capture driver.
 *
 * This file contains the function definitions for configuring Timer A2 in Capture mode.
 *
 * @author Aaron Nanas
 *
 */


#ifndef TIMER_A2_CAPTURE_H_
#define TIMER_A2_CAPTURE_H_

#include <stdint.h>
#include "msp.h"

void Timer_A2_Capture_Init(void(*task)(uint16_t time));

#endif /* TIMER_A2_CAPTURE_H_ */
