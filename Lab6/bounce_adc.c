#include <stdio.h>                  // Include standard input/output library
#include "BOARD.h"                  // Include CMPE13 Support Library - BOARD
#include "Oled.h"                   // Include CMPE13 Support Library - OLED
#include "Ascii.h"                  // Include CMPE13 Support Library - ASCII

#include <xc.h>                     // Include Microchip libraries - XC compiler
#include <sys/attribs.h>            // Include Microchip libraries - attributes
#include <stdlib.h>                 // Include standard library for abs function

#define WINDOW_SIZE 5               // Set window size for ADC comparison
#define TRUE 1                      // Define TRUE as 1
#define FALSE 0                     // Define FALSE as 0

struct AdcResult {                  // Define structure for ADC result
    uint8_t event;                  // Flag indicating ADC event
    uint16_t voltage;               // Voltage value from ADC
};

static struct AdcResult voltA = {.event = FALSE}; // Initialize ADC result structure

void AdcHandler(void);              // Declare function prototype for ADC interrupt handler

int main(void)
{
    BOARD_Init();                   // Initialize hardware

    IPC6bits.AD1IP = 2;             // Set ADC interrupt priority
    IPC6bits.AD1IS = 0;             // Set ADC interrupt sub-priority
    IEC1bits.AD1IE = 1;             // Enable ADC interrupts

    TRISBbits.TRISB2 = 1;           // Set B2 as input for ADC

    AD1CHSbits.CH0SA = 2;           // Add B2 to ADC mux
    AD1PCFGbits.PCFG2 = 0;          // Add B2 to ADC
    AD1CSSLbits.CSSL2 = 1;          // Add B2 to ADC scanner

    AD1CON1 = 0;                    // Initialize ADC configuration
    AD1CON1bits.SSRC = 0b111;       // Set auto conversion
    AD1CON1bits.ASAM = 1;           // Set auto sample

    AD1CON2 = 0;                    // Initialize ADC configuration
    AD1CON2bits.SMPI = 7;           // Set interrupt per 8 samples

    AD1CON3 = 0;                    // Initialize ADC configuration
    AD1CON3bits.SAMC = 29;          // Set long sample time
    AD1CON3bits.ADCS = 50;          // Set long conversion time

    AD1CON1bits.ADON = 1;           // Turn on ADC

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    printf("Welcome to pramolia937's lab6 part3 (bounce_adc). Compiled on %s %s.\n", __TIME__, __DATE__);
    OledInit();                     // Initialize OLED display

    while (1) {
        if (voltA.event == TRUE) {  // Check for ADC event
            char buffer[100];
            int percentage = (int) ((float)voltA.voltage * 100) / 1023; // Calculate percentage for OLED display
            sprintf(buffer, "Potentiometer value: \n%4d\n%3d%% ", voltA.voltage, percentage);
            OledDrawString(buffer);  // Draw string on OLED display
            OledUpdate();            // Update OLED display
            voltA.event = FALSE;     // Reset ADC event flag
        }
    }
}

/***************************************************************************************************
 * Your code goes in between this comment and the preceding one with asterisks
 **************************************************************************************************/

/**
 * This is the interrupt service routine (ISR) for the ADC1 peripheral.
 * It will trigger whenever a new ADC reading is available in the ADC buffer SFRs, ADC1BUF0 - ADC1BUF7.
 * It should not be called directly and should communicate with the main code only by using module-level variables.
 */
void __ISR(_ADC_VECTOR, ipl2auto) AdcHandler(void)
{
    IFS1bits.AD1IF = 0;             // Clear ADC interrupt flag

    uint16_t AdcSum = 0;            // Variable to store sum of ADC values

    
    AdcSum += ADC1BUF0;
    AdcSum += ADC1BUF1;
    AdcSum += ADC1BUF2;
    AdcSum += ADC1BUF3;
    AdcSum += ADC1BUF4;
    AdcSum += ADC1BUF5;
    AdcSum += ADC1BUF6;
    AdcSum += ADC1BUF7;
        
    

    uint16_t AdcAverage = AdcSum / 8; // Calculate average ADC value

    if (abs(AdcAverage - voltA.voltage) > WINDOW_SIZE) {
        voltA.event = TRUE;           // Set ADC event flag
        voltA.voltage = AdcAverage;   // Update voltage value
    }
}
