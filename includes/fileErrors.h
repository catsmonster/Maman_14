
#ifndef MAMAN_14_FILEERRORS_H
#define MAMAN_14_FILEERRORS_H

/*
 * This file provides an interface with error handling throughout the assembler process.
 */
typedef enum errorCodes {
    ERROR_MISSING_AS_FILE = 300,
    ERROR_INVALID_FILE_EXTENSION,
    ERROR_FILE_DOES_NOT_EXIST,
    ERROR_INVALID_CHARACTER,
    ERROR_DIRECTIVE_TOO_LONG,
    ERROR_CMD_TOO_LONG,
    ERROR_DIRECTIVE_CMD_NOT_FOUND,
    ERROR_MEMORY_ALLOCATION,
    ERROR_MEMORY_LIMIT,
    ERROR_LABEL_COMMAND_CONFLICT,
    ERROR_LABEL_NAME_CONFLICT,
    ERROR_COMMAND_NOT_FOUND,
    ERROR_FILE_IS_EMPTY,
    ERROR_MISSING_WHITESPACE,
    ERROR_MAX_LABEL_LENGTH,
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
    ERROR_NOT_AN_INTEGER,
    ERROR_LINE_LENGTH_EXCEEDS_LIMIT
}errorCodes;

/*
 * handles printing input related errors.
 * accepts an error code, a string representing the cause, a line counter and a position counter.
 */
void printInputError(errorCodes, char *errorCause, long line, int pos);
/*
 * handles printing memory related errors.
 * accepts an error code
 */
void printMemoryError(errorCodes);
/*
 * handles printing file access related errors.
 * accepts an error code and a string representing the cause
 */
void printFileError(errorCodes, char *errorCause);
/*
 * returns 1 if a known error was encountered
 * accepts an error code
 */
int isFileError(errorCodes);
/*
 * when passed a char and an array, edits the array to show as a string containing the given char
 * accepts a char and a destination string
 */
void stringFromChar(char, char[]);

/*
 * handles the process of converting a char to a string and printing the invalid character error.
 * accepts the char which caused the error, current line counter, error code pointer and the position counter pointer.
 */
void handleInvalidCharacterError(char errorChar, long currentLine, errorCodes *error, const int *pos);

/*
 * handles the process of converting a char to a string and printing the 'not a number' error.
 * accepts the char which caused the error, current line counter, error code pointer and the position counter pointer.
 */
void handleNANError(char errorChar, long currentLine, errorCodes *error, const int *pos);

#endif
