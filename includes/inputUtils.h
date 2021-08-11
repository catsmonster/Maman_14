
#ifndef MAMAN_14_INPUTUTILS_H
#define MAMAN_14_INPUTUTILS_H
#define MAX_LINE_LENGTH 81
#define MAX_LABEL_LENGTH 32
#define MAX_DIRECTIVE_CMD_LENGTH 8
#define NOT_FOUND -1
#include "fileErrors.h"
/*
 * iterates over the given input line and skips any whitespaces, advancing the position counter.
 * accepts a position counter, and an input string.
 */
void skipWhiteSpaces(int *, const char *);
/*
 * attempts to find the next element in a list separated by commas, handles format related errors.
 * accepts the position counter, an input string, current line counter and a pointer to an error code.
 */
int advanceToNextArgument(int *pos, const char *input, long currentLine, errorCodes *error);
/*
 * advances to the first valid character in the line, announcing any illegal characters along the way, and skipping
 * empty lines or comment lines.
 * accepts a position counter, input string, current line counter and error code pointer.
 */
void advanceToStartOfValidInput(int * pos, const char inputLine[], long currentLine, errorCodes *error);
/*
 * makes sure there is no invalid excess data after the end of the command (if a command was found).
 * accepts a position counter, an input string, current line counter and error code pointer.
 */
int isValidEndOfCommand(const int *pos, const char *inputLine, long currentLine, errorCodes *error);
/*
 * attempts to read a label from the input line.
 * accepts a position counter, input string, current line counter, error code pointer and a destination string.
 */
void readInputLabel(int *pos, const char *input, long currentLine, errorCodes *error, char *labelName);
#endif
