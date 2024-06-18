#ifndef GAME_H
#   define GAME_H
#endif

/**
 * @file
 *
 * @section DESCRIPTION
 *
 * This file defines a single function named runGame(char* theWord) which implements a C-based
 * version of Hangman. First, the following sanity checks are done on theWord.
 *
 * 1) The length of string represented by theWord is less than MAX_WORD_LENGTH
 * 2) The word only comprises of alphabets A-Z (in upper- or lower-case) and/or whitespaces
 * 3) The word should not begin or end with a whitespace, and should not contain consecutive whitespaces.
 *
 * If the sanity checks are passed, runGame initiates a game of Hangman with the string contained
 * in char* theWord.
 */


/**
 * MAX_WORD_LENGTH defines the max word or phrase length
 * MAX_ATTEMPTS defines the max number of times the user can guess the word/phrase
 * NUM_ALPHABETS defines the number of alphabets in english i.e. 26
 */

#define MAX_WORD_LENGTH 20
#define MAX_WRONG_ATTEMPTS 5
#define NUM_ALPHABETS 26

/******************************************************************************
 * Hangman Implementation Function:
 *****************************************************************************/
/**
 * Perform sanity checks on char* theWord. If any sanity check fails, return from the function with an error code.
 * Else play a game of Hangman with the provided word and return the number of balloons remaining.
 *
 * @param: theWord, pointer to char string
 * @return: zero or positive: number of balloons remaining, negative: rule violated in input validation
 */
int runGame(char* theWord);
