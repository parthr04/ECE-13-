// Leds_Lab06.c
#include "Leds_Lab06.h"
#include <xc.h>


void LEDS_INIT(void) {
    // Initialize LED pins as outputs and set them to low
    TRISE = 0x00; // 0000 0000 
    LATE = 0x00; // 0000 0000
}

void LEDS_SET(char newPattern) {
    // Toggle LEDs based on the new pattern
    LATE = newPattern;
  
}

char LEDS_GET(void) {
    // Return the current state of the LED pins
    return LATE;
}
