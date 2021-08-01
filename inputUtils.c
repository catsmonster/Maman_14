
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
int advanceToNextArgument(int *pos, const char *input, long currentLine, int commaDetected, errorCodes *error){
    int errorFound = 0;
    skipWhiteSpaces(pos, input);
    if (input[*pos] && input[*pos] != '\n') {
        if (input[*pos] == ',') {
            if (commaDetected) {
                *error = ERROR_MULTIPLE_CONSECUTIVE_COMMAS;
                printInputError(ERROR_MULTIPLE_CONSECUTIVE_COMMAS, "", currentLine, *pos);
                errorFound = LOCAL_ERROR;
            } else {
                (*pos)++;
                advanceToNextArgument(pos, input, currentLine, 1, error);
            }
        } else {
            if (!commaDetected) {
                *error = ERROR_MISSING_COMMA;
                printInputError(ERROR_MISSING_COMMA, "", currentLine, *pos);
                errorFound = LOCAL_ERROR;
            }
        }
    }
    else {
        skipWhiteSpaces(pos, input);
        if (commaDetected && (input[*pos] && !isgraph(input[*pos]) || !input[*pos])) {
            *error = ERROR_MISSING_ARGUMENT;
            printInputError(ERROR_MISSING_ARGUMENT, "", currentLine, *pos);
            errorFound = LOCAL_ERROR;
        }
    }
    return errorFound;
}

/*
 * skips to the start of the text to be processed by the assembler, notifying about any invalid characters along the way
 */
int preProcessing(int * pos, const char inputLine[], long currentLine, errorCodes *error) {
    int skipLine = 0;
    skipWhiteSpaces(pos, inputLine);
    if (!inputLine[*pos] || inputLine[*pos] == ';')
        skipLine = 1;

    if (!skipLine) {
        if (!isalnum(inputLine[*pos]) && inputLine[*pos] != '.') {
            char errorString[2];
            stringFromChar(inputLine[(*pos)++], errorString);
            printInputError(ERROR_INVALID_CHARACTER, errorString, currentLine, *pos - 1);
            skipLine = preProcessing(pos, inputLine, currentLine, error);
            *error = ERROR_INVALID_CHARACTER;
        }
    }
    return skipLine;
}

/*
 * validates there isn't excess data after the end of the expected input
 */
int isValidEndOfCommand(const int *pos, const char *inputLine, long currentLine, errorCodes *error) {
    int valid = 1;
    if (!isspace(inputLine[*pos])) {
        handleInvalidCharacterError(inputLine[*pos], currentLine, error, pos);
        *error = ERROR_INVALID_CHARACTER;
        valid = 0;
    }
    return valid;
}


