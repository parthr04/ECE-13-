#include <stdio.h>
#include <stdlib.h>
#include "Field.h"
#include "Uart1.h"
#include "BOARD.h"

static Field testFieldOwn;
static Field testFieldOther;
static Field otherRepr;
static GuessData guess;

/*
 * 
 */
int main() {
    
    printf("Testing Field:\n\n");
    
    // Test for init, makes sure that the fields are initialized as necessary
    FieldInit(&testFieldOwn, &testFieldOther);
    FieldInit(FIELD_SQUARE_EMPTY, &otherRepr);
    printf("Below, own should print with all FIELD_SQUARE_EMPTY squares, "
            "and other should print with all FIELD_SQUARE_UNKNOWN squares\n\n");
    FieldPrint_UART(&testFieldOwn, &testFieldOther);
    
    // Below are tests for get square and set square, just making sure they return the right value from the grid
    // get square test
    if (FieldGetSquareStatus(&testFieldOwn, 3, 3) == FIELD_SQUARE_EMPTY) {
        printf("\nFieldGetSquareStatus(): success\n");
    } else {
        printf("\nFieldGetSquareStatus() (or possibly FieldInit()): failed\n");
    }
    
    // set square test
    FieldSetSquareStatus(&testFieldOwn, 0, 0, FIELD_SQUARE_SMALL_BOAT);
    if (FieldGetSquareStatus(&testFieldOwn, 0, 0) == FIELD_SQUARE_SMALL_BOAT) {
        printf("FieldSetSquareStatus(): success\n");
    } else {
        printf("FieldSetSquareStatus() (or possibly GetSquareStatus()): failed\n");
    }
    
    FieldSetSquareStatus(&testFieldOwn, 0, 0, FIELD_SQUARE_EMPTY);
    
    // testing add boat, using field print uart as visual confirmation
    
    FieldAddBoat(&testFieldOwn, 0, 0, FIELD_DIR_EAST, FIELD_BOAT_TYPE_SMALL);
    FieldAddBoat(&testFieldOwn, 1, 0, FIELD_DIR_EAST, FIELD_BOAT_TYPE_MEDIUM);
    FieldAddBoat(&testFieldOwn, 0, 6, FIELD_DIR_SOUTH, FIELD_BOAT_TYPE_SMALL);
    
    // MAKE SURE THE HUGE BOAT IS OVERWRITTEN ENTIRELY AND NO PARTS OF IT REMAIN
    printf("Below, own should print with a horizontal small boat at (0, 0) and horizontal medium boat at (1, 0) "
            "and a vertical small boat at (0, 6), other should still be empty\n\n");
    
    FieldPrint_UART(&testFieldOwn, &testFieldOther);
    
    // calling field init just to clear the fields
    FieldInit(&testFieldOwn, &otherRepr);
    FieldInit(&testFieldOther, &otherRepr);
    
    
    if (FieldAIPlaceAllBoats(&testFieldOwn) == SUCCESS) {
        printf("\nFieldAIPlaceAllBoats(): success, matrix printed below\n\n");
        // uncomment this print if unsure about actual success, didn't want to print too much so left it commented
        //FieldPrint_UART(&testFieldOwn, &testFieldOther);
    } else {
        printf("\nFieldAIPlaceAllBoats(): failed\n");
    }
    
    FieldAIPlaceAllBoats(&testFieldOther);
    
    // simulates and attack and checks if the responses are correct below
    FieldPrint_UART(&testFieldOwn, &testFieldOther);
    
    guess = FieldAIDecideGuess(&otherRepr);
    
    
    SquareStatus square = FieldRegisterEnemyAttack(&testFieldOther, &guess);
    
   
    if (square != FieldGetSquareStatus(&testFieldOther, guess.row, guess.col)) {
        printf("\nAIDecideGuess() and RegisterEnemyAttack(): passed\n");
    } else {
        printf("Something failed!\n");
    }
    FieldUpdateKnowledge(&otherRepr, &guess);
    if (FieldGetBoatStates(&testFieldOther) == 0b1111) {
        printf("\nFieldGetBoatStates(): success\n");
    }
    
    Field test2;
    Field test3;
    
    printf("\nOne last FieldAIPlaceAllBoats(): \n\n");
    FieldInit(&test2, &otherRepr);
    FieldInit(&test3, &otherRepr);
    
    // prints the ai one more time to check for randomness
    FieldAIPlaceAllBoats(&test2);
    FieldAIPlaceAllBoats(&test3);
    
    FieldPrint_UART(&test2, &test3);
    

    while (1);
}