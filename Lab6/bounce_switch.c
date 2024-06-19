// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

// CMPE13 Support Library
#include "BOARD.h"
#include "Leds_Lab06.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// **** Declare any datatypes here ****

struct Timer {
    uint8_t event;        // Flag indicating a timer event
    int16_t timeRemaining; // Time remaining for the next event
};

// **** Define global, module-level, or external variables here ****
// LED bit mask
#define TRUE 1
#define FALSE 0
#define LEFT -1
#define RIGHT 1

static struct Timer TimerA = {.event = FALSE}; // Initialize a Timer structure with event set to FALSE

// **** Declare function prototypes ****

int main(void)
{
    BOARD_Init(); // Initialize the board

    // Configure Timer 1 using PBCLK as input.
    T1CON = 0;               // Clear the T1CON register
    T1CONbits.TCKPS = 1;      // Set the prescaler to 1:8
    PR1 = 0xFFFF;             // Set the period register for maximum interval
    T1CONbits.ON = 1;         // Turn on Timer 1

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T1IF = 0;        // Clear the Timer 1 interrupt flag
    IPC1bits.T1IP = 4;        // Set the priority of Timer 1 interrupt to 4
    IPC1bits.T1IS = 0;        // Set the subpriority of Timer 1 interrupt to 0 arbitrarily
    IEC0bits.T1IE = 1;        // Enable Timer 1 interrupt

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    printf("Welcome to pramolia937's lab6 part2 (bounce_switch). Compiled on %s %s.\n", __TIME__, __DATE__);

    // Set everything to initial states
    LEDS_INIT(); // Initialize the LEDs
    TimerA.timeRemaining = 2 * SWITCH_STATES() + 10; // Set initial time remaining based on switch states
    LEDS_SET(0x01); // Set the initial LED pattern
    int state = 0;
    uint8_t LED1 = 0x01;
    uint8_t LED8 = 0x80;
    uint8_t CurrentLED = 0x01;
    
    // Infinite loop of bouncing lights
    while (1) {
        // Leftmost light is 0x01, rightmost is 0x80
        if (TimerA.event == TRUE) { // Check if a timer event has occurred
            if (CurrentLED == LED1) { // Check if the leftmost LED is reached
                state = LEFT;
            }
            // If going left and hits the rightmost LED, go back right
            if (CurrentLED == LED8) {
                state = RIGHT;
            }
            if (state == RIGHT){
                CurrentLED = CurrentLED >> 1; // Shift the LED pattern right
            }
            if (state == LEFT){
                CurrentLED = CurrentLED << 1; // Shift the LED pattern left
            }
            LEDS_SET(CurrentLED); // Set the LEDs based on the current pattern
            TimerA.event = FALSE; // Reset the timer event flag
        }
    }
}
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/


/**
 * This is the interrupt for the Timer1 peripheral. It will trigger at the frequency of the peripheral
 * clock, divided by the timer 1 prescaler and the interrupt interval.
 * 
 * It should not be called, and should communicate with main code only by using module-level variables.
 */
void __ISR(_TIMER_1_VECTOR, ipl4auto) Timer1Handler(void)
{
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    
    TimerA.timeRemaining--; // Decrement the time remaining for the timer event
    if(TimerA.timeRemaining == 0){ // Check if the time remaining has reached zero
        TimerA.event = TRUE; // Set the timer event flag
        TimerA.timeRemaining = 2 * SWITCH_STATES() + 10; // Reset the time remaining based on switch states
    }

    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/
}
