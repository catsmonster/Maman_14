
#ifndef MAMAN_14_INPUTUTILS_H
#define MAMAN_14_INPUTUTILS_H
#define MAX_LINE_LENGTH 81
#define MAX_LABEL_LENGTH 32
#define MAX_DIRECTIVE_CMD_LENGTH 8
#define NOT_FOUND -1
#include "fileErrors.h"
void skipWhiteSpaces(int *, const char *);
int advanceToNextArgument(int *pos, const char *input, long currentLine, int commaDetected, errorCodes *error);
int preProcessing(int * pos, const char inputLine[], long currentLine, errorCodes *error);
int isValidEndOfCommand(const int *pos, const char *inputLine, long currentLine, errorCodes *error);
void readInputLabel(int *pos, const char *input, long currentLine, errorCodes *error, char *labelName);
#endif
