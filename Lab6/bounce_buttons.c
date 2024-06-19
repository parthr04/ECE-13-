// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

// CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "Buttons.h"
#include "Leds_Lab06.h"

// **** Set macros and preprocessor directives ****
enum LEDRegisters {
    LED_0 = 0x00,
    LED_1 = 0x01,
    LED_2 = 0x02,
    LED_3 = 0x04,
    LED_4 = 0x08,
    LED_5 = 0x10,
    LED_6 = 0x20,
    LED_7 = 0x40,
    LED_8 = 0x80
};

// **** Declare any datatypes here ****
static uint8_t eventTimer; // Timer to store button events

// **** Define global, module-level, or external variables here ****

// Function prototypes
void InitializeSystem(void);

int main(void) {
    InitializeSystem();

    printf("Welcome to jwang456's lab6 part5 (bounce_buttons). Compiled on %s %s.\n", __TIME__, __DATE__);

    LEDS_INIT();      // Initialize LEDs
    ButtonsInit();    // Initialize buttons

    while (1) {
        if (eventTimer != BUTTON_EVENT_NONE) {
            char currentLED = LEDS_GET();       // Get the current LED pattern
            uint8_t switchesState = SWITCH_STATES();  // Get the state of the switches

            // Define macros for LED and switch mappings
            #define LED_MAP(sw, up, down) (currentLED ^= (sw) ? (up) : (down))
            #define PROCESS_SWITCH(sw, up, down) LED_MAP(switchesState & sw, up, down)

            // Process each switch and update LED pattern accordingly
            PROCESS_SWITCH(SWITCH_STATE_SW1, LED_1 | LED_2, LED_1 | LED_2);
            PROCESS_SWITCH(SWITCH_STATE_SW2, LED_3 | LED_4, LED_3 | LED_4);
            PROCESS_SWITCH(SWITCH_STATE_SW3, LED_5 | LED_6, LED_5 | LED_6);
            PROCESS_SWITCH(SWITCH_STATE_SW4, LED_7 | LED_8, LED_7 | LED_8);

            LEDS_SET(currentLED);       // Set the LEDs with the updated pattern
            eventTimer = BUTTON_EVENT_NONE;  // Reset the timer event
        }
    }

    while (1);
}

/**
 * Initialize the system including Timer 1 and interrupts.
 */
void InitializeSystem(void) {
    BOARD_Init();  // Initialize the system

    T1CON = 0;  // Clear the Timer 1 configuration
    T1CONbits.TCKPS = 1;  // 1:8 prescaler
    PR1 = 0xFFFF;  // Set the period for the interrupt
    T1CONbits.ON = 1;  // Turn on Timer 1

    IFS0bits.T1IF = 0;  // Clear the Timer 1 interrupt flag
    IPC1bits.T1IP = 4;  // Set the interrupt priority to 4
    IPC1bits.T1IS = 0;  // Set the interrupt subpriority to 0
    IEC0bits.T1IE = 1;  // Enable Timer 1 interrupt
}

/**
 * This is the interrupt for the Timer1 peripheral. It should check for button events and store them in a
 * module-level variable.
 * You should not modify this function for ButtonsTest.c or bounce_buttons.c!
 */
void __ISR(_TIMER_1_VECTOR, ipl4auto) Timer1Handler(void) {
    IFS0bits.T1IF = 0;  // Clear the Timer 1 interrupt flag
    eventTimer = ButtonsCheckEvents();  // Get the button events
}
