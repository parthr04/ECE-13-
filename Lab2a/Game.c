// Game.c
#include "Game.h"
#include <stdio.h>


int runGame(char* theWord) {
    // Rule 2: Check if the word length exceeds the maximum allowed length
    if (strlen(theWord) > MAX_WORD_LENGTH) {
        printf("ERROR: The input word is longer than allowable length.\n");
        return -2;
    }

    // Rule 3: Check for invalid characters
    for (int i = 0; theWord[i]; i++) {
        if ((theWord[i] < 'A' || theWord[i] > 'Z') && (theWord[i] < 'a' || theWord[i] > 'z')) {
            printf("ERROR: Invalid character in input word.\n");
            return -3;
        }
    }

    // Rule 4: Check for leading or trailing whitespaces and consecutive whitespaces
    if (theWord[0] == ' ' || theWord[strlen(theWord) - 1] == ' ') {
        printf("ERROR: Word begins or ends with a whitespace.\n");
        return -4;
    }
    for (int i = 0; theWord[i]; i++) {
        if (theWord[i] == ' ' && theWord[i + 1] == ' ') {
            printf("ERROR: Word has consecutive whitespaces.\n");
            return -4;
        }
    }

    // Convert the input word to uppercase
    for (int i = 0; theWord[i]; i++) {
        if (theWord[i] >= 'a' && theWord[i] <= 'z') {
            theWord[i] = theWord[i] - 'a' + 'A';
        }
    }

    // Initialize game variables
    int balloonsRemaining = MAX_WRONG_ATTEMPTS;
    char revealedWord[MAX_WORD_LENGTH];
    int wordLength = strlen(theWord);

    // Initialize revealedWord with underscores
    for (int i = 0; i < wordLength; i++) {
        if (theWord[i] == ' ') {
            revealedWord[i] = ' ';
        } else {
            revealedWord[i] = '_';
        }
    }
    revealedWord[wordLength] = '\0';

    // Main game loop
    while (balloonsRemaining > 0) {
        printf("Save Stickman! Complete the word: %s\n", revealedWord);
        printf("Enter a letter: ");

        char guess;
        scanf(" %c", &guess);  // Read a character (ignores leading whitespace)

        // Check if the guess is a valid letter
        if (guess < 'A' || (guess > 'Z' && guess < 'a') || guess > 'z') {
            printf("Invalid input! Please try again.\n");
            continue;
        }

        // Convert lowercase guess to uppercase
        if (guess >= 'a' && guess <= 'z') {
            guess = guess - 'a' + 'A';
        }

        // Check if the letter has already been revealed
        int letterRevealed = 0;
        for (int i = 0; i < wordLength; i++) {
            if (theWord[i] == guess && revealedWord[i] == '_') {
                revealedWord[i] = guess;
                letterRevealed = 1;
            }
        }

        if (letterRevealed) {
            printf("Correct guess! You have %d balloons remaining.\n", balloonsRemaining);
        } else {
            printf("Letter was input previously and is wrong. Please enter another.\n");
            balloonsRemaining--;
            printf("Wrong guess! You have %d balloons remaining.\n", balloonsRemaining);
        }

        // Check if the word has been completely revealed
        int wordRevealed = 1;
        for (int i = 0; i < wordLength; i++) {
            if (revealedWord[i] == '_') {
                wordRevealed = 0;
                break;
            }
        }

        if (wordRevealed) {
            printf("Correct guess! You have %d balloons remaining.\n", balloonsRemaining);
            printf("Stickman is safe! The correct word is: %s\n", theWord);
            return balloonsRemaining;
        }
    }

    // All balloons have popped, the game is lost
    printf("Wrong guess! You have 0 balloons remaining.\n");
    printf("Monster ate Stickman :( The correct word was: %s\n", theWord);
    return 0;
}
