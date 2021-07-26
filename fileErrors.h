
#ifndef MAMAN_14_FILEERRORS_H
#define MAMAN_14_FILEERRORS_H

/*
 * This file provides an interface with error handling throughout the assembly process.
 */
enum errorCodes {
    ERROR_MISSING_AS_FILE = 300,
    ERROR_INVALID_FILE_EXTENSION,
    ERROR_FILE_DOES_NOT_EXIST,
    ERROR_INVALID_CHARACTER,
    ERROR_DIRECTIVE_CMD_TOO_LONG,
    ERROR_DIRECTIVE_CMD_NOT_FOUND,
    ERROR_TYPE_FILE,
    ERROR_TYPE_INPUT,
    ERROR_MEMORY_ALLOCATION,
    ERROR_TYPE_MEMORY,
    ERROR_MEMORY_LIMIT,
    ERROR_LABEL_COMMAND_CONFLICT,
    ERROR_LABEL_NAME_CONFLICT,
    ERROR_COMMAND_NOT_FOUND,
    ERROR_FILE_IS_EMPTY,
    ERROR_MISSING_WHITESPACE,
    ERROR_MAX_LENGTH,
    ERROR_INTEGER_OUT_OF_RANGE,
    ERROR_MULTIPLE_CONSECUTIVE_COMMAS,
    ERROR_MISSING_COMMA,
    ERROR_MISSING_DATA,
    ERROR_MISSING_QUOTATION,
    ERROR_MISSING_ARGUMENT,
    ERROR_EXCESSIVE_ARGUMENT,
    LOCAL_ERROR,
    ERROR_SYMBOL_NOT_FOUND,
    WARNING_LABEL_IGNORED,
    ERROR_EXTERN_IS_ENTRY,
    ERROR_EXTERN_IN_CONDITIONAL,
    ERROR_FILE_PERMISSIONS,
    ERROR_NOT_AN_INTEGER
};


/*
 * Expects an error code, type of error code and number of arguments.
 * If file type error, can accept a file name as possible argument.
 * If input type error, can accept a string of the error cause, a line and a position - in that order.
 */
void printError(int, int, int, ...);
/*
 * returns 1 if a known error was encountered
 */
int isFileError(int);
/*
 * when passed a char and an array, edits the array to show as a string containing the given char
 */
void stringFromChar(char, char[]);

void handleInvalidCharacterError(char errorChar, long currentLine, int *error, const int *pos);

void handleNANError(char errorChar, long currentLine, int *error, const int *pos);

#endif
