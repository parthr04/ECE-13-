// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"

// Header file with runGame function
#include "Game.h"

int main() {
    BOARD_Init();
    char* wordsList[] = {
        "HI",
        "HELP",
        "DOWN",
        "COMPUTER",
        "DANCING",
        "FIGHTING",
        "SALSA",
        "INSTAGRAM",
    };

    for (int i = 0; i < sizeof(wordsList) / sizeof(wordsList[0]); i++) {
        int result = runGame(wordsList[i]);
        printf("Result: %d\n", result);
    }
    BOARD_End();
    return 0;
}