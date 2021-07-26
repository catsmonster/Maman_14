
#include "inputUtils.h"
#include "fileErrors.h"
#include <ctype.h>

void skipWhiteSpaces(int *pos, const char *inputLine) {
    while (isspace(inputLine[*pos]))
        (*pos)++;
}

/*
 * advances to the next valid number to read into the list
 */
int advanceToNextArgument(int *pos, const char *input, long currentLine, int commaDetected, int *error){
    int errorFound = 0;
    skipWhiteSpaces(pos, input);
    if (input[*pos] && input[*pos] != '\n') {
        if (input[*pos] == ',') {
            if (commaDetected) {
                *error = ERROR_MULTIPLE_CONSECUTIVE_COMMAS;
                printError(ERROR_MULTIPLE_CONSECUTIVE_COMMAS, ERROR_TYPE_INPUT, 3, "", currentLine, *pos);
                errorFound = LOCAL_ERROR;
            } else {
                (*pos)++;
                advanceToNextArgument(pos, input, currentLine, 1, error);
            }
        } else {
            if (!commaDetected) {
                *error = ERROR_MISSING_COMMA;
                printError(ERROR_MISSING_COMMA, ERROR_TYPE_INPUT, 3, "", currentLine, *pos);
                errorFound = LOCAL_ERROR;
            }
        }
    }
    else {
        skipWhiteSpaces(pos, input);
        if (commaDetected && (input[*pos] && !isgraph(input[*pos]) || !input[*pos])) {
            *error = ERROR_MISSING_ARGUMENT;
            printError(ERROR_MISSING_ARGUMENT, ERROR_TYPE_INPUT, 3, "", currentLine, *pos);
            errorFound = LOCAL_ERROR;
        }
    }
    return errorFound;
}

/*
 * skips to the start of the text to be processed by the assembler, notifying about any invalid characters along the way
 */
int preProcessing(int * pos, const char inputLine[], long currentLine, int *error) {
    int skipLine = 0;
    skipWhiteSpaces(pos, inputLine);
    if (!inputLine[*pos] || inputLine[*pos] == ';')
        skipLine = 1;

    if (!skipLine) {
        if (!isalnum(inputLine[*pos]) && inputLine[*pos] != '.') {
            char errorString[2];
            stringFromChar(inputLine[(*pos)++], errorString);
            printError(ERROR_INVALID_CHARACTER, ERROR_TYPE_INPUT, 3, errorString, currentLine, *pos - 1);
            skipLine = preProcessing(pos, inputLine, currentLine, error);
            *error = 1;
        }
    }
    return skipLine;
}


