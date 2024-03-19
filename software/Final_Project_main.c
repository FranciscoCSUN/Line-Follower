/**
 * @file Final_Project_main.c
 *
 * @brief Main source code for the Final Project.
 *
 * @author
 *
 */

#include <stdint.h>
#include <math.h>
#include "msp.h"
#include "../inc/Clock.h"
#include "../inc/CortexM.h"
#include "../inc/GPIO.h"
#include "../inc/SysTick_Interrupt.h"
#include "../inc/EUSCI_A0_UART.h"
#include "../inc/Timer_A0_PWM.h"
#include "../inc/Timer_A1_Interrupt.h"
#include "../inc/Timer_A2_Capture.h"
#include "../inc/Timer_A3_Capture.h"
#include "../inc/Bumper_Sensors.h"
#include "../inc/Motor.h"
#include "../inc/Tachometer.h"
#include "../inc/LPF.h"
#include "../inc/Analog_Distance_Sensor.h"
#include "../inc/Reflectance_Sensor.h"

// buzzer
const int BUZZER_DURATION   = 200;
const int NOTE_INTERVAL     = 500;
// buzzer notes
const int C4_NOTE_FREQ       = 1980; // C4 (261 Hz)
const int D4_NOTE_FREQ       = 1750; // D4 (293 Hz)
const int E4_NOTE_FREQ       = 1555; // E4 (330 Hz)
const int F4_NOTE_FREQ       = 1470; // F4 (349 Hz)
const int F4S_NOTE_FREQ      = 1390; // F4# (370 Hz)
const int G4_NOTE_FREQ       = 1310; // G4 (392 Hz)
const int A4_NOTE_FREQ       = 1165; // A4 (440 Hz)
const int B4_NOTE_FREQ       = 1040; // B4 (493 Hz)
const int C5_NOTE_FREQ       = 980; // C5 (523 Hz)

// PID controller
double desired = 0.0;   // Desired value
double integral = 0.0;         // Integral term starts at 0
double previous = 0.0;      // Previous error
uint16_t PID = 0;       // PID to change PWM values
double Kp = 20.0;        // proportional constant
double Ki = 0.0;        // integral constant
double Kd = 1.0;         // derivative constant

// Initialize constant PWM duty cycle values for the motors
#define PWM_NOMINAL         3500 //3500
#define PWM_SWING           3000
#define PWM_MIN             (PWM_NOMINAL - PWM_SWING)
#define PWM_MAX             (PWM_NOMINAL + PWM_SWING)

#define LOWER_BOUND         47 // 47
#define UPPER_BOUND         332 // 332
// Declare global variables used to update PWM duty cycle values for the motors
uint16_t Duty_Cycle_Left;
uint16_t Duty_Cycle_Right;

uint16_t Edge_Counter = 0;

// Declare global variable used to store line sensor raw data
uint8_t Line_Sensor_Data;

// Declare global variable used to store line sensor position
int32_t Line_Sensor_Position;

// Global variable counter to keep track of the number of SysTick interrupts
// that have occurred. It increments in SysTick_Handler on each interrupt event.
uint32_t SysTick_counter = 0;

int dead_right = 0;
int ignore_left = 0;

// Define the states for the Line Follower FSM
typedef enum
{
    CENTER  = 0,
    L1 = 1,
    L2 = 2,
    L3 = 3,
    R1 = 4,
    R2 = 5,
    R3 = 6,
    DEAD_END = 7,
    LEFT_T = 8
} Line_Follower_State;

// Initialize the current state to CENTER
Line_Follower_State current_state = CENTER;
/** @brief This function handles the robot's collision function. It will stop then back up, 
 * turn around, then play a tune. It is designed to no longer move after handling this collision.
 */
void Handle_Collision()
{
    // Stop the motors
    Motor_Stop();
    // Make a function call to Clock_Delay1ms(2000)
    Clock_Delay1ms(1000);

    // Flash the LEDs to indicate collision event after stopping the motors
    for (int i = 0; i < 5; i++)
    {
        LED1_Output(RED_LED_OFF);
        LED2_Output(RGB_LED_OFF);
        Clock_Delay1ms(200);
        LED1_Output(RED_LED_ON);
        LED2_Output(RGB_LED_RED);
        Clock_Delay1ms(200);
    }

    // Move the motors backward with 30% duty cycle
    Motor_Backward(4500, 4500);
    // Make a function call to Clock_Delay1ms(3000)
    Clock_Delay1ms(500);

    // Stop the motors
    Motor_Stop();
    // Make a function call to Clock_Delay1ms(1000)
    Clock_Delay1ms(500);

    // Make the robot turn to the right with 10% duty cycle
    Motor_Right(4500, 4500);
    // Make a function call to Clock_Delay1ms(5000)
    Clock_Delay1ms(1300);

    // Stop the motors
    Motor_Stop();

    Note_Pattern_1();
    while(1){

    }

}
/**
 * @brief Implements the finite state machine (FSM) for a simple Line Follower robot.
 *
 * This function represents the FSM for the Line Follower robot. The robot's behavior is determined
 * by the current state, and it performs the following actions based on the state:
 * - CENTER: Moves forward and changes the RGB LED's color to green
 * - LEFT: Turns left and changes the RGB LED's color to blue
 * - RIGHT: Turns right and changes the RGB LED's color to yellow
 *
 * @return None
 */
void Line_Follower_FSM_1()
{
    switch(current_state)
    {
        case LEFT_T:
        {
            LED1_Output(RGB_LED_BLUE);
            LED2_Output(RGB_LED_BLUE);
            ignore_left = 0;
            dead_right = 0;
            Duty_Cycle_Left = PWM_NOMINAL;
            Duty_Cycle_Right = PWM_NOMINAL;
            Motor_Forward(Duty_Cycle_Left, Duty_Cycle_Right);
            break;
        }
        case R3:
        {
            LED1_Output(RGB_LED_RED);
            LED2_Output(RGB_LED_RED);
            dead_right = 1;
            ignore_left = 1;
            Motor_Right(Duty_Cycle_Left, Duty_Cycle_Right);
            break;
        }
        case R1:
        {
            LED1_Output(RGB_LED_YELLOW);
            LED2_Output(RGB_LED_OFF);
            dead_right = 1;
            ignore_left = 0;
            Motor_Forward(Duty_Cycle_Left, Duty_Cycle_Right);
            break;
        }
        case CENTER:
        {
            LED1_Output(RGB_LED_GREEN);
            LED2_Output(RGB_LED_GREEN);
            ignore_left = 0;
            Motor_Forward(Duty_Cycle_Left, Duty_Cycle_Right);
            break;
        }
        case L1:
        {
            LED1_Output(RGB_LED_OFF);
            LED2_Output(RGB_LED_YELLOW);
            dead_right = 0;
            ignore_left = 0;
            Motor_Forward(Duty_Cycle_Left, Duty_Cycle_Right);
            break;
        }
        case DEAD_END:
        {
            ignore_left = 0;
            Motor_Left(Duty_Cycle_Left, Duty_Cycle_Right);
            LED2_Output(RGB_LED_SKY_BLUE);
            break;
        }
    }
}


/* @brief PID controller function is used for self correction when following the line.
 *
 * @param double actual
 *
 *
 * @return double PID
 *
 */
double pidController(double actual) {

    double error = desired - actual;    // calculated error
    double proportional = Kp * error;              // proportional term,   Kp global
    integral += Ki * error;                    // integral term,       Ki global
    double derivative = Kd * (error - previous);     // derivative term,     Kd global
    double PID = proportional + integral + derivative;             // total PID
    previous = error;                       // set prev to current error
    return PID;
}



/**
 * @brief The Line follower program follows a line, prioritizing the right turns. The robot fully explores 
 * any intersection. Once an object has collided with its bumper sensors. It will back up and play a tune.
 *
 *
 * @return None
 */
void Line_Follower_Controller_2()
{
    // Increment SysTick_counter by 1 every time the SysTick periodic interrupt occurs
    SysTick_counter = SysTick_counter + 1;

    // Start the process of reading the reflectance sensor array every 10 ms (i.e. 11, 21, 31, ...)
    if ((SysTick_counter % 10) == 1)
    {
        Reflectance_Sensor_Start();
    }

    // Finish reading the reflectance sensor sensor array after 1 ms (i.e. 12, 22, 32, ...)
    if ((SysTick_counter % 10) == 2)
    {
        Line_Sensor_Data = Reflectance_Sensor_End();
        if(ignore_left == 1) Line_Sensor_Data = Line_Sensor_Data >> 1;
        Line_Sensor_Position = Reflectance_Sensor_Position(Line_Sensor_Data);

        PID = pidController(Line_Sensor_Position);

        if (current_state == DEAD_END){
            if(-48 < Line_Sensor_Position && Line_Sensor_Position < 48)
                current_state = CENTER;
            else
                current_state = DEAD_END;
        }
        else if (current_state == R3){
            if(Line_Sensor_Position > 48 && Line_Sensor_Position < 238)
                current_state = R1;
            else
                current_state = R3;
        }
        else if(Line_Sensor_Data == 0) {
            current_state = DEAD_END;
        }
        else if(Line_Sensor_Data & 0xF == 0xF) {
            current_state = R3;
        }
        else if(Line_Sensor_Data & 0xF0 == 0xF0) {
            current_state = LEFT_T;
        }
        else if(Line_Sensor_Position >= 48){
            current_state = R1;
        }
        else if(Line_Sensor_Position <= -48){
            current_state = L1;
        }
        else if(-48 < Line_Sensor_Position && Line_Sensor_Position < 48)
        {
            current_state = CENTER;
        }
        else{
            current_state = DEAD_END;
        }

        Duty_Cycle_Right = PWM_NOMINAL + PID;
        Duty_Cycle_Left = PWM_NOMINAL - PID;

        // Ensure that the duty cycle for the right motor does not go below the minimum PWM value
        if (Duty_Cycle_Right < PWM_MIN) Duty_Cycle_Right = PWM_MIN;

        // Ensure that the duty cycle for the right motor does not exceed the maximum PWM value
        if (Duty_Cycle_Right > PWM_MAX) Duty_Cycle_Right = PWM_MAX;

        // Ensure that the duty cycle for the left motor does not go below the minimum PWM value
        if (Duty_Cycle_Left  < PWM_MIN) Duty_Cycle_Left  = PWM_MIN;

        // Ensure that the duty cycle for the left motor does not exceed the maximum PWM value
        if (Duty_Cycle_Left  > PWM_MAX) Duty_Cycle_Left  = PWM_MAX;
    }
}

void Bumper_Sensors_Handler(uint8_t bumper_sensor_state)
{
    Handle_Collision();
}

/**
 * @brief This function is the handler for the SysTick periodic interrupt with a rate of 1 kHz.
 *
 * The SysTick_Handler generates a periodic interrupt that calls a specific controller function based on the selected
 * active configuration. Only one of the options can be defined at a time: CONTROLLER_1 or CONTROLLER_2.
 *
 * @return None
 */
void SysTick_Handler(void)
{
    Line_Follower_Controller_2();
}

void Detect_Edge(uint16_t time)
{
    // Your code for Task 1 goes here
    Edge_Counter = Edge_Counter + 1;
}
/**
 * @brief User-defined function executed by Timer A1 using a periodic interrupt at a rate of 1 kHz.
 *
 *
 * @return None
 */
uint8_t Done = 0;
void Timer_A1_Periodic_Task(void)
{
    // Your function for Task 1 goes here (Line_Follower_FSM_2)
    Line_Follower_FSM_1();
}
/**
 * @brief Used to set up piezo buzzer
 *
 *
 * @return None
 */
void Buzzer_Init()
{
    P10->SEL0 &= ~0x01;
    P10->SEL1 &= ~0x01;
    P10->DIR |= 0x01;
}
/**
 * @brief returns value of the buzzer.
 *
 *
 * @return int buzzer_value
 */
int Buzzer_Output(uint8_t buzzer_value)
{
    P10->OUT = buzzer_value;
    return ((buzzer_value != 0) ? 1 : 0);
}
/**
 * @brief Allows for a certain note to be played.
 *
 *
 * @return None
 */
void Play_Note(int note_delay_value)
{
    int Buzzer_Status = Buzzer_Output(0x00);
    Clock_Delay1us(note_delay_value);
    Buzzer_Status = Buzzer_Output(0x01);
    Clock_Delay1us(note_delay_value);
}

/* A note pattern of C,F,B is generated by the piezo buzzer
 * 
*/
void Note_Pattern_1(void) {
    int a[62] = {D4_NOTE_FREQ,  G4_NOTE_FREQ,   G4_NOTE_FREQ,   A4_NOTE_FREQ,
                G4_NOTE_FREQ,   F4S_NOTE_FREQ,  E4_NOTE_FREQ,   E4_NOTE_FREQ,
                E4_NOTE_FREQ,   A4_NOTE_FREQ,   A4_NOTE_FREQ,   B4_NOTE_FREQ,
                A4_NOTE_FREQ,   G4_NOTE_FREQ,   F4S_NOTE_FREQ,  D4_NOTE_FREQ,
                D4_NOTE_FREQ,   B4_NOTE_FREQ,   B4_NOTE_FREQ,   C5_NOTE_FREQ,
                B4_NOTE_FREQ,   A4_NOTE_FREQ,   G4_NOTE_FREQ,   E4_NOTE_FREQ,
                D4_NOTE_FREQ,   D4_NOTE_FREQ,   E4_NOTE_FREQ,   A4_NOTE_FREQ,
                F4S_NOTE_FREQ,  G4_NOTE_FREQ};
    int b[62] = {BUZZER_DURATION,   BUZZER_DURATION,    BUZZER_DURATION/2,  BUZZER_DURATION/2,
                BUZZER_DURATION/2,  BUZZER_DURATION/2,  BUZZER_DURATION,    BUZZER_DURATION,
                BUZZER_DURATION,    BUZZER_DURATION,    BUZZER_DURATION/2,  BUZZER_DURATION/2,
                BUZZER_DURATION/2,  BUZZER_DURATION/2,  BUZZER_DURATION,    BUZZER_DURATION,
                BUZZER_DURATION,    BUZZER_DURATION,    BUZZER_DURATION/2,  BUZZER_DURATION/2,
                BUZZER_DURATION/2,  BUZZER_DURATION/2,  BUZZER_DURATION,    BUZZER_DURATION,
                BUZZER_DURATION/2,  BUZZER_DURATION/2,  BUZZER_DURATION,    BUZZER_DURATION,
                BUZZER_DURATION,    BUZZER_DURATION};
    for (int i = 0; i < sizeof(a)/sizeof(a[0]); i++) {
        for (int j = 0; j < b[i]; j++)
        {
            Play_Note(a[i]);
        }
        Clock_Delay1us(BUZZER_DURATION);
    }
}

int main(void)
{
    //collision_detected = 0;
    // Ensure that interrupts are disabled during initialization
    DisableInterrupts();

    // Initialize the 48 MHz Clock
    Clock_Init48MHz();

    Buzzer_Init();

    // Initialize the built-in red LED
    LED1_Init();
    LED2_Init();

    // Initialize the buttons
    //Buttons_Init();

    // Initialize bumper sensors
    Bumper_Sensors_Init(&Bumper_Sensors_Handler);

    // Initialize EUSCI_A0_UART
    //EUSCI_A0_UART_Init_Printf();

    // Initialize Timer A1 periodic interrupt with a rate of 1 kHz
    Timer_A1_Interrupt_Init(&Timer_A1_Periodic_Task, TIMER_A1_INT_CCR0_VALUE);

    // Initialize the tachometers
    //Tachometer_Init();

    // Initialize the motors
    Motor_Init();

    // Initialize the 8-Channel QTRX Reflectance Sensor Array module
    Reflectance_Sensor_Init();

    // Initialize motor duty cycle values
    Duty_Cycle_Left  = PWM_NOMINAL;
    Duty_Cycle_Right = PWM_NOMINAL;

    // Initialize SysTick periodic interrupt with a rate of 1 kHz
    SysTick_Interrupt_Init(SYSTICK_INT_NUM_CLK_CYCLES, SYSTICK_INT_PRIORITY);

    // Enable the interrupts used by Timer A1 and other modules
    EnableInterrupts();

    while(1)
    {

    }
}
