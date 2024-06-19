// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
//CSE13E Support Library
#include "BOARD.h"

//Lab specific libraries
#include "Leds.h"
#include "Adc.h"
#include "Ascii.h"
#include "Buttons.h"
#include "Oled.h"
#include "OledDriver.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// **** Set any macros or preprocessor directives here ****
#define TIMER_2HZ_RESET() (TMR1 = 0)

// **** Set any local typedefs here ****
typedef enum {
    STRUCTURE, SELECTOR_PENDING, COOK, RESET_PROCESS, BONUS_POINT
} OvenState;

typedef struct {
    OvenState state; 
    uint16_t initCookTime; // Used to store the time cooked
    uint16_t TimeLeft; // Used when cooking to change the time left
    uint16_t Temp; // Temperature setting
    uint16_t buttonPressTime; // Used to measure how long a button was pressed
    uint8_t mode; // Used to determine what cooking mode the toaster is in
} OvenData;

// Enum for the different cooking modes of the toaster
typedef enum {
    BAKE, TOAST, BROIL
} CookingStates;

// Variables declared below
// Consist of various flags, counters, and variables for storing/keeping track of values
static uint16_t TimeLeft = 0;
static uint16_t adcValues;
static uint8_t editingTheTemp = FALSE;
static uint8_t buttonsEvents = BUTTON_EVENT_NONE;
static uint16_t remainderStuff;
static uint16_t LEDIntervals;
static uint16_t storedTimeLeft = 0;
static OvenData Data;
static uint16_t TIMER_TICKING = FALSE;
static uint8_t adcModify = FALSE;
static uint16_t temperatureHolder;
static uint8_t invert = FALSE;
static char currentLED;
static uint16_t TickCounter = 0;

// **** Define any module-level, global, or external variables here ****
#define SECOND 5
#define DEFAULT_TEMP 350
#define BROIL_TEMP 500
#define ALL_LEDS_ON 0xFF
#define CLEAR_LEDS 0x00
// **** Put any helper functions here ****

/*This function will update your OLED to reflect the state .*/
void updateOvenOLED(OvenData Data){
    char toPrint[60] = "";
    char topOn[6];
    sprintf(topOn, "%s%s%s%s%s", OVEN_TOP_ON, OVEN_TOP_ON, OVEN_TOP_ON, OVEN_TOP_ON, OVEN_TOP_ON);
    char topOff[6];
    sprintf(topOff, "%s%s%s%s%s", OVEN_TOP_OFF, OVEN_TOP_OFF, OVEN_TOP_OFF, OVEN_TOP_OFF, 
            OVEN_TOP_OFF);
    char botOn[6];
    sprintf(botOn, "%s%s%s%s%s", OVEN_BOTTOM_ON, OVEN_BOTTOM_ON, OVEN_BOTTOM_ON, OVEN_BOTTOM_ON, 
            OVEN_BOTTOM_ON);
    char botOff[6];
    sprintf(botOff, "%s%s%s%s%s", OVEN_BOTTOM_OFF, OVEN_BOTTOM_OFF, OVEN_BOTTOM_OFF, OVEN_BOTTOM_OFF, 
            OVEN_BOTTOM_OFF);

    // Switch statement to determine what to print.
    // The logic works as follows:
    // - We have cases based on the cooking mode of the toaster.
    // - If the state of the oven is not in cooking or reset pending, then we print the same thing.
    // - Else, if the state is cooking or reset pending, we print with specifications to what the toaster looks like when it is cooking.
    // - The cook times are divided by 60 to get the minute and modulused by 60 to get the second, because there are sixty seconds in a minute.
    switch (Data.mode) {
        case BAKE:
            if (!(Data.state == COOK || Data.state == RESET_PROCESS)) {
                if (!editingTheTemp) {
                    sprintf(toPrint, "|%s| Mode: Bake\n|   "
                            "  | >Time: %d:%02d\n|-----|  Temp: %d%sF"
                            "\n|%s|", topOff, Data.initCookTime/60, Data.initCookTime%60,
                            Data.Temp, DEGREE_SYMBOL, botOff);
                } else {
                    sprintf(toPrint, "|%s| Mode: Bake\n|    "
                            " |  Time: %d:%02d\n|-----| >Temp: %d%sF"
                            "\n|%s|", topOff, Data.initCookTime/60, Data.initCookTime%60,
                            Data.Temp, DEGREE_SYMBOL, botOff);
                }
            } else {
                sprintf(toPrint, "|%s| Mode: Bake\n|   "
                        "  |  Time: %d:%02d\n|-----|  Temp: %d%sF"
                            "\n|%s|", topOn, Data.TimeLeft/60, Data.TimeLeft%60,
                            Data.Temp, DEGREE_SYMBOL, botOn);
            }
            break;
        case TOAST:
            if (!(Data.state == COOK || Data.state == RESET_PROCESS)) {
                sprintf(toPrint, "|%s| Mode: Toast\n|  "
                        "   |  Time: %d:%02d\n|-----|"
                        "\n|%s|", topOff, Data.initCookTime/60, Data.initCookTime%60,
                         botOff);
            } else {
                sprintf(toPrint, "|%s| Mode: Toast\n|  "
                        "   |  Time: %d:%02d\n|-----|"
                        "\n|%s|", topOff, Data.TimeLeft/60, Data.TimeLeft%60,
                         botOn);
            }
            break;
        case BROIL:
            if (!(Data.state == COOK || Data.state == RESET_PROCESS)) {
                sprintf(toPrint, "|%s| Mode: Broil\n|  "
                        "   |  Time: %d:%02d\n|-----|  Temp: 500%sF"
                        "\n|%s|", topOff, Data.initCookTime/60, Data.initCookTime%60,
                        DEGREE_SYMBOL, botOff);
            } else {
                sprintf(toPrint, "|%s| Mode: Broil\n|   "
                        "  |  Time: %d:%02d\n|-----|  Temp: 500%sF"
                        "\n|%s|", topOn, Data.TimeLeft/60, Data.TimeLeft%60,
                        DEGREE_SYMBOL, botOff);
            }
            break;
    }
    // After determining what to print and storing it in a string,
    // we clear the OLED to reset it,
    // draw the string on the OLED,
    // if we have just finished cooking and are flashing the screen to display so, we check to see
    // if the OLED is inverted and invert/display normally accordingly.
    OledClear(OLED_COLOR_BLACK);
    OledDrawString(toPrint);
    if (Data.state == BONUS_POINT) {
        if (invert) {
            OledSetDisplayNormal();
        } else {
            OledSetDisplayInverted();
        }
    }
    // Update OLED to make sure changes show.
    OledUpdate();
}

/*This function will execute your state machine.  
 * It should ONLY run if an event flag has been set.*/
void runOvenSM(void)
{
    switch (Data.state) {
        case STRUCTURE:
            if (adcModify) {
                adcValues = AdcRead();
                // Shifts 2 bits because there are six zeros at the end
                // ANDs with the mask 0000001111111100 to get the top 8 bits
                // Then we shift the integer two bits to get rid of extra zeros at the end,
                // thus we have isolated the top 8 bits.
                adcValues = (adcValues & 0x03FC) >> 2;
                // If we are editing the temperature, scale the adcValue by adding 300 to get
                // within the range of desired temperatures,
                // else add 1 to get within the range of desired cooking times.
                if (Data.mode == BAKE && editingTheTemp) {
                    Data.Temp = adcValues + 300;
                } else {
                    Data.initCookTime = adcValues + 1;
                    Data.TimeLeft = Data.initCookTime;
                }               
                updateOvenOLED(Data);
            }
            // If button 3 is pressed, we move to selector change pending because either we will
            // change from editing temp to editing time, or change cooking modes.
            // Store the free time.
            if (buttonsEvents & BUTTON_EVENT_3DOWN) {
                Data.buttonPressTime = TimeLeft;
                Data.state = SELECTOR_PENDING;
            }
            
            // If button four is pressed, we start cooking.
            // I store the initial cook time multiplied by 5 and divided by 8 because that converts
            // the cook time to the number of timer ticks it will take to cook, and I store the
            // remainder of the division in a remainder variable. These variables will be used to
            // keep track of when the LED bar should tick down as accurately as possible.
            if (buttonsEvents & BUTTON_EVENT_4DOWN) {
                storedTimeLeft = TimeLeft;
                Data.state = COOK;
                updateOvenOLED(Data);
                LEDS_SET(ALL_LEDS_ON);
                LEDIntervals = (Data.initCookTime * 5) / 8;
                remainderStuff = (Data.initCookTime * 5) % 8;
                TickCounter = 0;
            }
            break;
        case SELECTOR_PENDING:
            // Once a button 3 has been released, we calculate how much time it took to do that.
            // If it took 1 second or over to release and we are in bake mode, we change from
            // editing time to temperature and vice versa.
            // Afterward, we update the OLED and go back to the setup state.
            if (buttonsEvents & BUTTON_EVENT_3UP) {
                uint16_t elapsedTime = TimeLeft - Data.buttonPressTime;
                if (elapsedTime >= SECOND) {
                    if (Data.mode == BAKE) {
                        if (editingTheTemp) {
                            editingTheTemp = FALSE;
                        } else {
                            editingTheTemp = TRUE;
                        }
                    }
                    updateOvenOLED(Data);
                    Data.state = STRUCTURE;
                } else {
                    // Else, the button took less than a second to be released, and we change
                    // between modes.
                    if (Data.mode == BROIL) {
                        Data.mode = BAKE;
                    } else {
                        Data.mode++;
                    }
                    if (Data.mode == BROIL) {
                        temperatureHolder = Data.Temp;
                        Data.Temp = 500;
                    } else if (Data.mode == BAKE) {
                        Data.Temp = temperatureHolder;
                    }
                    updateOvenOLED(Data);
                    Data.state = STRUCTURE;  
                }
            }
            break;
        case COOK:
            // Every time there is a timer tick, we check if we need to reduce the time on the OLED
            // by a second, or if an LED needs to go off.
            if (TIMER_TICKING) { 
                // timeTickCounter keeps track of how many timer ticks have happened
                // If the amount is equal to the amount of timer ticks it should take for one of
                // the LEDs to turn off, then it turns off an LED. We occasionally add one to the
                // # of ticks it takes to turn off an LED because we only have 8 LEDs to represent
                // the progress bar and time is not always divisible by 8. Thus, the remaining
                // after a division by 8 are distributed evenly among the turning off of each
                // LED, so it represents as close to 1/8 of the time as we can get
                TickCounter++;
                if ((remainderStuff > 0 && TickCounter == LEDIntervals + 1) || 
                        (remainderStuff == 0 && TickCounter == LEDIntervals)) {
                    currentLED = LEDS_GET();
                    TickCounter = 0;
                    remainderStuff--;
                    LEDS_SET(currentLED << 1);
                }
                // If the cook time is 0, we go to the finished, flashing extra credit mode
                if (Data.TimeLeft == 0) {
                    Data.TimeLeft = Data.initCookTime;
                    Data.state = BONUS_POINT;
                    updateOvenOLED(Data);
                    LEDS_SET(CLEAR_LEDS);
                    break;
                }
                // If the freeTime - the free time that was stored is divisible by 5, that means a
                // second has passed and it is time to change the OLED
                if ((TimeLeft - storedTimeLeft) % 5 == 0) {
                    Data.TimeLeft--;
                    updateOvenOLED(Data);
                }
            }
            // If button 4 is pressed, it means the user could want to reset the cook time and we
            // go to reset pending mode
            if (buttonsEvents & BUTTON_EVENT_4DOWN) {
                Data.state = RESET_PROCESS;
                Data.buttonPressTime = TimeLeft;
            }
            break;
        case RESET_PROCESS:
            
            // While determining if a timer is reset, there's a chance that a
            // second could pass or an LED could turn off during the button press.
            // To address this, code from the cooking state is added to ensure
            // the OLED updates accordingly if needed. If the button is held for
            // a second or more, the timer resets; otherwise, if held and released
            // for less than a second, we return to the cooking state.
            if (TIMER_TICKING) {
                TickCounter++;
                if ((remainderStuff > 0 && TickCounter == LEDIntervals + 1) || 
                        (remainderStuff == 0 && TickCounter == LEDIntervals)) {
                    currentLED = LEDS_GET();
                    TickCounter = 0;
                    remainderStuff--;
                    LEDS_SET(currentLED << 1);
                }
                if ((TimeLeft - storedTimeLeft) % 5 == 0) {
                    if (Data.TimeLeft) {
                        Data.TimeLeft--;
                        updateOvenOLED(Data);
                    }
                }
                if (TimeLeft - Data.buttonPressTime >= SECOND) {
                    Data.TimeLeft = Data.initCookTime;
                    Data.state = STRUCTURE;
                    updateOvenOLED(Data);
                    LEDS_SET(CLEAR_LEDS);
                    break;
                }
            }
            if (buttonsEvents & BUTTON_EVENT_4UP && (TimeLeft - Data.buttonPressTime < SECOND)) {
                    Data.state = COOK;
            }
            break;
        case BONUS_POINT:
            // Extra credit case
            // When cooking is done, the program routes here. If the screen looks
            // normal, it becomes inverted; otherwise, if it is inverted, it becomes
            // normal, creating a flashing effect. This is done with a flag.
            if (TIMER_TICKING) {
                if (invert) {
                    invert = FALSE;
                } else {
                    invert = TRUE;
                }
                updateOvenOLED(Data);
            }
            if (buttonsEvents & BUTTON_EVENT_4UP) {
                invert = TRUE;
                updateOvenOLED(Data);
                Data.state = STRUCTURE;
                updateOvenOLED(Data);
            }
            break;
    }
}


int main()
{
    BOARD_Init();

     //initalize timers and timer ISRs:
    // <editor-fold defaultstate="collapsed" desc="TIMER SETUP">
    
    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.

    T2CON = 0; // everything should be off
    T2CONbits.TCKPS = 0b100; // 1:16 prescaler
    PR2 = BOARD_GetPBClock() / 16 / 100; // interrupt at .5s intervals
    T2CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T2IF = 0; //clear the interrupt flag before configuring
    IPC2bits.T2IP = 4; // priority of  4
    IPC2bits.T2IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T2IE = 1; // turn the interrupt on

    // Configure Timer 3 using PBCLK as input. We configure it using a 1:256 prescaler, so each timer
    // tick is actually at F_PB / 256 Hz, so setting PR3 to F_PB / 256 / 5 yields a .2s timer.

    T3CON = 0; // everything should be off
    T3CONbits.TCKPS = 0b111; // 1:256 prescaler
    PR3 = BOARD_GetPBClock() / 256 / 5; // interrupt at .5s intervals
    T3CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T3IF = 0; //clear the interrupt flag before configuring
    IPC3bits.T3IP = 4; // priority of  4
    IPC3bits.T3IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T3IE = 1; // turn the interrupt on;

    // </editor-fold>
   
    printf("Welcome to pramolia937's Lab07 (Toaster Oven).  Compiled on %s %s.", __TIME__, __DATE__);

    //initialize state machine here
    Data.buttonPressTime = 0;
    Data.initCookTime = 1;
    Data.TimeLeft = 1;
    Data.Temp = DEFAULT_TEMP;
    Data.state = STRUCTURE;
    Data.mode = BAKE;
    
    OledInit();
    ButtonsInit();
    AdcInit();
    LEDS_INIT();
    
    updateOvenOLED(Data);
    while (1){
        // Add main loop code here:
        // Check for events
        // On event, run runOvenSM()
        // Clear event flags
        if (buttonsEvents != BUTTON_EVENT_NONE || adcModify || TIMER_TICKING) {
            runOvenSM();
            buttonsEvents = BUTTON_EVENT_NONE;
            adcModify = FALSE;
            TIMER_TICKING = FALSE;
        }
    };
}


/*The 5hz timer is used to update the free-running timer and to generate TIMER_TICKING events*/
void __ISR(_TIMER_3_VECTOR, ipl4auto) TimerInterrupt5Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 12;
    // Add event-checking code here
    // Set TIMER_TICK to true
    // Increment free time
    TIMER_TICKING = TRUE;
    TimeLeft++;
}


/*The 100hz timer is used to check for button and ADC events*/
void __ISR(_TIMER_2_VECTOR, ipl4auto) TimerInterrupt100Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;
    // Add event-checking code here
    adcModify = AdcChanged();
    buttonsEvents = ButtonsCheckEvents();
}