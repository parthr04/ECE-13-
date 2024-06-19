// **** Include libraries here ****
// old bounce
// Standard libraries
#include <stdio.h>
#include <GenericTypeDefs.h>

// CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "Leds_Lab06.h"


// **** Declare any datatypes here ****

struct Timer {
    uint8_t event;
    int16_t timeRemaining;
};

// **** Define global, module-level, or external variables here ****

#define TIMER_A_COUNTDOWN 2*38
#define TIMER_B_COUNTDOWN 3*38
#define TIMER_C_COUNTDOWN 5*38

static struct Timer TimerA = { .timeRemaining = TIMER_A_COUNTDOWN, .event = 0 };
static struct Timer TimerB = { .timeRemaining = TIMER_B_COUNTDOWN, .event = 0 };
static struct Timer TimerC = { .timeRemaining = TIMER_C_COUNTDOWN, .event = 0 };


// **** Declare function prototypes ****

int main(void)
{
    BOARD_Init();
    
    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
    // pretty reasonable rate to start.
    T1CON = 0; // everything should be off
    T1CONbits.TCKPS = 1; // 1:8 prescaler
    PR1 = 0xFFFF; // interrupt at max interval
    T1CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T1IF = 0; // clear the interrupt flag before configuring
    IPC1bits.T1IP = 4; // priority of  4
    IPC1bits.T1IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T1IE = 1; // turn the interrupt on
    
    LEDS_INIT();

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    printf("Welcome to pramolia937's lab6 part1 (timers).  Compiled on %s %s.\n", __TIME__, __DATE__);

    while (1) {
        char ledAssign = 0x00;
        // poll timer A
        if (TimerA.event) {
            printf("A\n");
            ledAssign += 0x01;
            TimerA.event = FALSE;
        }
        // poll timer B
        if (TimerB.event) {
            printf("B\n");
            ledAssign += 0x02;
            TimerB.event = FALSE;
        }
        // poll timer C
        if (TimerC.event) {
            printf("C\n");
            ledAssign += 0x04;
            TimerC.event = FALSE;
        }
        if(ledAssign != 0x00){
            LEDS_SET(ledAssign);
        }
    }


    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}

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
    
    // update timerA
    TimerA.timeRemaining--;
    if (TimerA.timeRemaining == 0) {
        TimerA.event = TRUE;
        TimerA.timeRemaining = TIMER_A_COUNTDOWN;
    }

    // update timerB
    TimerB.timeRemaining--;
    if (TimerB.timeRemaining == 0) {
        TimerB.event = TRUE;
        TimerB.timeRemaining = TIMER_B_COUNTDOWN;
    }

    // update timerC
    TimerC.timeRemaining--;
    if (TimerC.timeRemaining == 0) {
        TimerC.event = TRUE;
        TimerC.timeRemaining = TIMER_C_COUNTDOWN;
    }
    
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}
