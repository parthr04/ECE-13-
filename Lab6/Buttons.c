#include <stdint.h>
#include <genericTypeDefs.h>
#include "BOARD.h"
#include "Buttons.h"

// Define custom variables for debounce and previous status
static uint8_t customDebounceCounter;
static uint8_t customPreviousStatus;

// Initialization function for custom buttons
void ButtonsInit(void) {

    // Set the buttons to input mode
    TRISD |= 0x00E0; // Set RD5, RD6, RD7 as inputs
    TRISF |= 0x0002; // Set RF1 as input
    
    // Initialize custom debounce counter and previous status to zero
    customDebounceCounter = 0;
    customPreviousStatus = 0x00;
}

// Function to check button events for custom buttons
uint8_t ButtonsCheckEvents(void) {

    // Every call, customButtonStatus is set to no event
    uint8_t customButtonStatus = BUTTON_EVENT_NONE;

    // Current button states get the states of the buttons
    uint8_t customStatus = BUTTON_STATES();

    // If the customDebounceCounter is greater than zero, we don't check for events yet and instead return no event
    if (customDebounceCounter > 0) {
        customDebounceCounter--;
        return customButtonStatus;
    }

    // If the button states haven't changed, we return no events
    if (customStatus == customPreviousStatus) {
        return BUTTON_EVENT_NONE;
    }

    // Check each button and update customButtonStatus accordingly

    // Check BUTTON_STATE_4
    if ((customStatus & BUTTON_STATE_4) && !(customPreviousStatus & BUTTON_STATE_4)) {
        customButtonStatus |= BUTTON_EVENT_4DOWN;
    } else if (!(customStatus & BUTTON_STATE_4) && customPreviousStatus & BUTTON_STATE_4) {
        customButtonStatus |= BUTTON_EVENT_4UP;
    }

    // Check BUTTON_STATE_3
    if ((customStatus & BUTTON_STATE_3) && !(customPreviousStatus & BUTTON_STATE_3)) {
        customButtonStatus |= BUTTON_EVENT_3DOWN;
    } else if (!(customStatus & BUTTON_STATE_3) && customPreviousStatus & BUTTON_STATE_3) {
        customButtonStatus |= BUTTON_EVENT_3UP;
    }

    // Check BUTTON_STATE_2
    if ((customStatus & BUTTON_STATE_2) && !(customPreviousStatus & BUTTON_STATE_2)) {
        customButtonStatus |= BUTTON_EVENT_2DOWN;
    } else if (!(customStatus & BUTTON_STATE_2) && customPreviousStatus & BUTTON_STATE_2) {
        customButtonStatus |= BUTTON_EVENT_2UP;
    }

    // Check BUTTON_STATE_1
    if ((customStatus & BUTTON_STATE_1) && !(customPreviousStatus & BUTTON_STATE_1)) {
        customButtonStatus |= BUTTON_EVENT_1DOWN;
    } else if (!(customStatus & BUTTON_STATE_1) && customPreviousStatus & BUTTON_STATE_1) {
        customButtonStatus |= BUTTON_EVENT_1UP;
    }

    // Reset customDebounceCounter to its value
    // Set the customPreviousStatus to the customStatus
    // Then return the state of the buttons
    customDebounceCounter = BUTTONS_DEBOUNCE_PERIOD;
    customPreviousStatus = customStatus;
    return customButtonStatus;
}
