#include "../includes/fileErrors.h"
#include <stdio.h>

/*
 * Printing file opening related errors
 */
void printFileError(errorCodes errorCode, char * errorCause) {
    switch (errorCode) {
        case ERROR_INVALID_FILE_EXTENSION:
            printf("The file %s is not supported, please provide a valid .as file.\n----------------------------------------------\n", errorCause);
            break;
        case ERROR_MISSING_AS_FILE:
            printf("Missing assembly file, please provide a valid assembly file.\n");
            break;
        case ERROR_FILE_DOES_NOT_EXIST:
            printf("The file %s couldn't open, please make sure the file name was entered correctly.\n----------------------------------------------\n", errorCause);
            break;
        case ERROR_FILE_IS_EMPTY:
            printf("The requested file %s is empty.\n----------------------------------------------\n", errorCause);
            break;
        case ERROR_FILE_PERMISSIONS:
            printf("The requested file couldn't be generated. Possible permissions related error.\n----------------------------------------------\n");
            break;
        default:
            printf("An unknown error has occurred");
    }
}


/*
 * Printing file content related errors
 */
void printInputError(errorCodes errorCode, char * errorCause, long line, int pos) {
    switch (errorCode) {
        case ERROR_INVALID_CHARACTER:
            printf("Line %ld in position %d: The character '%s' is not valid here.\n", line, pos, errorCause);
            break;
        case ERROR_DIRECTIVE_CMD_TOO_LONG:
            printf("Line %ld in position %d: The command/directive was too long.\n", line, pos);
            break;
        case ERROR_DIRECTIVE_CMD_NOT_FOUND:
            printf("Line %ld in position %d: The directive \".%s\" does not exist\n", line, pos, errorCause);
            break;
        case ERROR_LABEL_NAME_CONFLICT:
            printf("Line %ld in position %d: The label \"%s\" conflicts with an existing label name\n", line, pos, errorCause);
            break;
        case ERROR_LABEL_COMMAND_CONFLICT:
            printf("Line %ld in position %d: The label \"%s\" conflicts with an existing command name\n", line, pos, errorCause);
            break;
        case ERROR_COMMAND_NOT_FOUND:
            printf("Line %ld in position %d: The command \"%s\" doesn't exist\n", line, pos, errorCause);
            break;
        case ERROR_MISSING_WHITESPACE:
            printf("Line %ld in position %d: expected a whitespace\n", line, pos);
            break;
        case ERROR_MAX_LENGTH:
            printf("Line %ld in position %d: label/command name too long.\n", line, pos);
            break;
        case ERROR_INTEGER_OUT_OF_RANGE:
            printf("Line %ld in position %d: given number is out of range\n", line, pos);
            break;
        case ERROR_MULTIPLE_CONSECUTIVE_COMMAS:
            printf("Line %ld in position %d: encountered multiple consecutive commas.\n", line, pos);
            break;
        case ERROR_MISSING_COMMA:
            printf("Line %ld in position %d: missing expected comma\n", line, pos);
            break;
        case ERROR_MISSING_DATA:
            printf("Line %ld in position %d: no data was given.\n", line, pos);
            break;
        case ERROR_MISSING_QUOTATION:
            printf("Line %ld in position %d: missing quotation mark, invalid string.\n", line, pos);
            break;
        case ERROR_MISSING_ARGUMENT:
            printf("Line %ld in position %d: missing arguments.\n", line, pos);
            break;
        case ERROR_EXCESSIVE_ARGUMENT:
            printf("Line %ld in position %d: excessive arguments\n", line, pos);
            break;
        case ERROR_SYMBOL_NOT_FOUND:
            printf("Line %ld: The symbol \"%s\" doesn't exist.\n", line, errorCause);
            break;
        case WARNING_LABEL_IGNORED:
            printf("Line %ld: a label \"%s\" before an entry/extern directive will be ignored.\n", line, errorCause);
            break;
        case ERROR_EXTERN_IS_ENTRY:
            printf("Line %ld: the label \"%s\" can't be both an external value and an entry.\n", line, errorCause);
            break;
        case ERROR_EXTERN_IN_CONDITIONAL:
            printf("Line %ld: external variable \"%s\" cannot be used together with a conditional command.\n", line, errorCause);
            break;
        case ERROR_NOT_AN_INTEGER:
            printf("Line %ld in position %d: error, expected a digit, instead encountered the character \"%s\".\n", line, pos, errorCause);
            break;
        default:
            printf("An unknown error has occurred\n");
    }
}

/*
 * Printing memory related errors
 */
void printMemoryError(errorCodes errorCode) {
    if (errorCode == ERROR_MEMORY_ALLOCATION)
        printf("Memory allocation failed! terminating program\n");
    else if (errorCode == ERROR_MEMORY_LIMIT)
        printf("Memory limit reached! terminating program\n");
}



/*
 * returns 1 for any known error given
 */
int isFileError(errorCodes errorCode) {
    int error;
    switch (errorCode) {
        case ERROR_INVALID_FILE_EXTENSION:
        case ERROR_MISSING_AS_FILE:
        case ERROR_FILE_DOES_NOT_EXIST:
        case ERROR_INVALID_CHARACTER:
        case ERROR_DIRECTIVE_CMD_TOO_LONG:
        case ERROR_DIRECTIVE_CMD_NOT_FOUND:
        case ERROR_MEMORY_ALLOCATION:
        case ERROR_MEMORY_LIMIT:
        case ERROR_LABEL_COMMAND_CONFLICT:
        case ERROR_LABEL_NAME_CONFLICT:
        case ERROR_COMMAND_NOT_FOUND:
        case ERROR_FILE_IS_EMPTY:
        case ERROR_MISSING_WHITESPACE:
        case ERROR_MAX_LENGTH:
        case ERROR_INTEGER_OUT_OF_RANGE:
        case ERROR_MULTIPLE_CONSECUTIVE_COMMAS:
        case ERROR_MISSING_COMMA:
        case ERROR_MISSING_DATA:
        case ERROR_MISSING_QUOTATION:
        case ERROR_MISSING_ARGUMENT:
        case ERROR_EXCESSIVE_ARGUMENT:
        case ERROR_SYMBOL_NOT_FOUND:
        case ERROR_EXTERN_IS_ENTRY:
        case ERROR_EXTERN_IN_CONDITIONAL:
        case ERROR_FILE_PERMISSIONS:
        case ERROR_NOT_AN_INTEGER:
            error = 1;
            break;
        default:
            error = 0;
    }
    return error;
}

/*
 * edits an array of 2 chars to pass as string argument to printError
 */
void stringFromChar(char c, char s[]) {
    s[0] = c;
    s[1] = 0;
}

/*
 * shows an invalid character error message
 */
void handleInvalidCharacterError(char errorChar, long currentLine, errorCodes *error, const int *pos) {
    char errorString[2];
    stringFromChar(errorChar, errorString);
    printInputError(ERROR_INVALID_CHARACTER, errorString, currentLine, *pos);
    *error = ERROR_INVALID_CHARACTER;
}

/*
 * shows invalid integer error message
 */
void handleNANError(char errorChar, long currentLine, errorCodes *error, const int *pos) {
    char errorString[2];
    stringFromChar(errorChar, errorString);
    printInputError(ERROR_NOT_AN_INTEGER, errorString, currentLine, *pos);
    *error = ERROR_NOT_AN_INTEGER;
}
