
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
void advanceToStartOfValidInput(int * pos, const char inputLine[], long currentLine, errorCodes *error) {
    skipWhiteSpaces(pos, inputLine);
    if (!inputLine[*pos] || inputLine[*pos] == ';')
        *pos = -1;

    if (*pos != -1) {
        if (!isalnum(inputLine[*pos]) && inputLine[*pos] != '.') {
            char errorString[2];
            stringFromChar(inputLine[(*pos)++], errorString);
            printInputError(ERROR_INVALID_CHARACTER, errorString, currentLine, *pos - 1);
            advanceToStartOfValidInput(pos, inputLine, currentLine, error);
            *error = ERROR_INVALID_CHARACTER;
        }
    }
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

/*
 * checks whether the given argument label is valid
 */
void validateLabelName(int *pos, const char input[], errorCodes *error, long currentLine, int index) {
    if (index >= MAX_LABEL_LENGTH && isalnum(input[*pos])) {
        *error = ERROR_MAX_LENGTH;
        printInputError(ERROR_MAX_LENGTH, "", currentLine, *pos);
    }
    skipWhiteSpaces(pos, input);
    if (isgraph(input[*pos])) {
        printInputError(ERROR_EXCESSIVE_ARGUMENT, "", currentLine, *pos);
    }
}

/*
 * reads an argument label from the input and validates it
 */
void readInputLabel(int *pos, const char *input, long currentLine, errorCodes *error, char *labelName) {
    skipWhiteSpaces(pos, input);
    if (!isalpha(input[*pos])) {
        handleInvalidCharacterError(input[*pos], currentLine, error, pos);
    }
    else {
        int index = 0;
        while (isalnum(input[*pos]) && index < MAX_LABEL_LENGTH) {
            labelName[index++] = input[(*pos)++];
        }
        validateLabelName(pos, input, error, currentLine, index);
    }
}


