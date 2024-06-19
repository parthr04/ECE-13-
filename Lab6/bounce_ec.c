// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

// CMPE13 Support Library
#include "BOARD.h"
#include "Oled.h"
#include "OledDriver.h"
#include "Buttons.h"
#include "Leds_Lab06.h"
#include "Ascii.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries

// **** Set macros and preprocessor directives ****

// **** Declare any datatypes here ****

// Enumeration for bounce state
typedef enum {
    BOUNCE_FROZEN,
    BOUNCE_UNFROZEN
} BounceState;

// **** Define global, module-level, or external variables here ****
static volatile BounceState bounceState = BOUNCE_UNFROZEN;
static volatile int potValue = 0;

// **** Declare function prototypes ****
void UpdateLEDBounceSpeed(int speed);

// **** Define macros here ****
#define MAX_BOUNCE_SPEED 10

int main(void)
{
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input.
    T1CON = 0; // everything should be off
    T1CONbits.TCKPS = 1; // 1:8 prescaler
    PR1 = 0xFFFF; // interrupt at max interval
    T1CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T1IF = 0; // clear the interrupt flag before configuring
    IPC1bits.T1IP = 4; // priority of  4
    IPC1bits.T1IS = 0; // subpriority of 0 arbitrarily
    IEC0bits.T1IE = 1; // turn the interrupt on

    // Enable interrupts for the ADC
    IPC6bits.AD1IP = 2;
    IPC6bits.AD1IS = 0;
    IEC1bits.AD1IE = 1;

    // Set B2 to an input so AN2 can be used by the ADC.
    TRISBbits.TRISB2 = 1;

    // Configure and start the ADC
    AD1CHSbits.CH0SA = 2; // add B2 to the mux
    AD1PCFGbits.PCFG2 = 0; // add b2 to the ADC
    AD1CSSLbits.CSSL2 = 1; // and add b2 to the scanner

    AD1CON1 = 0; // start with 0
    AD1CON1bits.SSRC = 0b111; // auto conversion
    AD1CON1bits.ASAM = 1; // auto sample

    AD1CON2 = 0; // start with 0
    AD1CON2bits.SMPI = 7; // one interrupt per 8 samples

    AD1CON3 = 0; // start with 0
    AD1CON3bits.SAMC = 29; // long sample time
    AD1CON3bits.ADCS = 50; // long conversion time

    AD1CON1bits.ADON = 1; // and finally turn it on

    OledInit(); // Initialize the OLED display
    ButtonsInit(); // Initialize buttons

    printf("Welcome to CRUZID's lab6 part6 (Extra Credit). Compiled on %s %s.\n", __TIME__, __DATE__);

    while (1)
        ;
}

/**
 * This is the interrupt for the Timer1 peripheral.
 */
void __ISR(_TIMER_1_VECTOR, IPL4AUTO) Timer1Handler(void)
{
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;

    // Update LED bounce speed based on potentiometer value
    UpdateLEDBounceSpeed();

    // Toggle bounce state based on button events
    ButtonsCheckEvents();
    if (ButtonsEvents & BUTTON_EVENT_1DOWN) {
        if (bounceState == BOUNCE_FROZEN) {
            bounceState = BOUNCE_UNFROZEN;
        } else {
            bounceState = BOUNCE_FROZEN;
        }
    }
}

/**
 * This is the ISR for the ADC1 peripheral.
 */
void __ISR(_ADC_VECTOR, IPL2AUTO) AdcHandler(void)
{
    // Clear the interrupt flag.
    IFS1bits.AD1IF = 0;

    // Read potentiometer value
    potValue = ADC1BUF0;
}

/**
 * Function to update LED bounce speed based on potentiometer value.
 */
void UpdateLEDBounceSpeed(void)
{
    // Calculate bounce speed based on potentiometer value
    int bounceSpeed = (int)((float)potValue / 1023.0 * MAX_BOUNCE_SPEED);

    // Your code to update LED bounce speed goes here
    // Replace the following line with your actual implementation
    // UpdateLEDBounceSpeed(bounceSpeed);
}
