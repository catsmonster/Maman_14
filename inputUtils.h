
#ifndef MAMAN_14_INPUTUTILS_H
#define MAMAN_14_INPUTUTILS_H
#define MAX_LINE_LENGTH 81
#define MAX_LABEL_LENGTH 32
#define MAX_DIRECTIVE_CMD_LENGTH 8
void skipWhiteSpaces(int *, const char *);
int advanceToNextArgument(int *pos, const char *input, long currentLine, int commaDetected, int *error);
int preProcessing(int * pos, const char inputLine[], long currentLine, int *error);
#endif
