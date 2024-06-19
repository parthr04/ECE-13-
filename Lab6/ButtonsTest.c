// Standard libraries
#include <stdio.h>

// CMPE13 Support Library
#include "BOARD.h"
#include "Buttons.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries

// **** Set macros and preprocessor directives ****

// **** Declare any datatypes here ****
static uint8_t timerEvent; // Variable to store button events

// **** Define global, module-level, or external variables here ****

// **** Declare function prototypes ****

int main(void)
{
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
    // pretty reasonable rate to start.
    T1CON = 0; // everything should be off
    T1CONbits.TCKPS = 2; // 1:64 prescaler
    PR1 = 0xFFFF; // interrupt at max interval
    T1CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T1IF = 0; // clear the interrupt flag before configuring
    IPC1bits.T1IP = 4; // priority of 4
    IPC1bits.T1IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T1IE = 1; // turn the interrupt on

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    printf("Welcome to pramolia937's lab6 part4 (ButtonsTest). Compiled on %s %s.\n", __TIME__, __DATE__);

    printf("Please press some buttons!\n");

    ButtonsInit(); // Initialize buttons
    char *dash1 = "----";
    char *dash2 = "----";
    char *dash3 = "----";
    char *dash4 = "----";
    
    while (1) {
        dash1 = "----";
        dash2 = "----";
        dash3 = "----";
        dash4 = "----";    
        
        if (timerEvent != BUTTON_EVENT_NONE) {
            
            // Use a bitwise AND to narrow down which buttons have been pressed or released
            if (timerEvent & BUTTON_EVENT_1DOWN) {
                dash1 = "DOWN";
            }
            if (timerEvent & BUTTON_EVENT_1UP) {
                dash1 = " UP ";
            }
            if (timerEvent & BUTTON_EVENT_2DOWN) {
                dash2 = "DOWN";
            }
            if (timerEvent & BUTTON_EVENT_2UP) {
                dash2 = " UP ";
            }
            if (timerEvent & BUTTON_EVENT_3DOWN) {
                dash3 = "DOWN";
            }
            if (timerEvent & BUTTON_EVENT_3UP) {
                dash3 = " UP ";
            }
            if (timerEvent & BUTTON_EVENT_4DOWN) {
                dash4 = "DOWN";
            }
            if (timerEvent & BUTTON_EVENT_4UP) {
                dash4 = " UP ";
            }
            
            timerEvent = BUTTON_EVENT_NONE; // Reset the timerEvent to no event
            
            // Print the event and what happened
            printf("Event: 4:%s 3:%s 2:%s 1:%s\n", dash4, dash3, dash2, dash1);
        }
    }
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}

/**
 * This is the interrupt for the Timer1 peripheral. It should check for button events and stores them in a
 * module-level variable.
 * 
 * You should not modify this function for ButtonsTest.c or bounce_buttons.c!
 */
void __ISR(_TIMER_1_VECTOR, ipl4auto) Timer1Handler(void)
{
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    // Every time the ISR runs, check for events
    timerEvent = ButtonsCheckEvents();
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}
