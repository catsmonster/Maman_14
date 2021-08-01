
#include "commands.h"
#include "listOfCommands.h"
#include "inputUtils.h"
#include "fileErrors.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NUM_LENGTH 3
#define MAX_REGISTER_VALUE 31
#define MAX_IMMED_NUM_LENGTH 7
#define MIN_SIZE_16_BITS -32768
#define MAX_SIZE_16_BITS 32767
#define MAX_LABEL_LENGTH 32

/*
 * returns 1 if found '$' character, indicating the start of a register
 */
int foundRegister(int *pos, const char *input, errorCodes *error, long currentLine) {
    int registerFound = 0;
    skipWhiteSpaces(pos, input);
    if (input[*pos] != '$') {
        handleInvalidCharacterError(input[*pos], currentLine, error, pos);
    }
    else {
        registerFound = 1;
        (*pos)++;
    }
    return registerFound;
}

/*
 * makes sure there is no junk data after a given integer, returns 1 if no errors were detected.
 */
int isEndOfIntegerValid(int *pos, const char *input, errorCodes *error, long currentLine) {
    int valid = 1;
    if (!isspace(input[*pos]) && input[*pos] != ',' && !isdigit(input[*pos])) {
        handleNANError(input[*pos], currentLine, error, pos);
        valid = 0;
    }
    return valid;
}

/*
 * prints integer out of range error
 */
errorCodes handleIntegerOutOfRange(errorCodes *error, long currentLine, int *pos) {
    *error = ERROR_INTEGER_OUT_OF_RANGE;
    printInputError(ERROR_INTEGER_OUT_OF_RANGE, "", currentLine, *pos);
    return LOCAL_ERROR;
}

/*
 * reads the register number from the input, converts it to an integer and validates its range.
 */
int readRegNum(int *pos, const char input[], long currentLine, errorCodes *error, int *reg) {
    char numStr[MAX_NUM_LENGTH] = {0};
    int strIndex = 0, num, foundError = 0;
    if (!isdigit(input[*pos])) {
        handleNANError(input[*pos], currentLine, error, pos);
        foundError = LOCAL_ERROR;
    }
    else {
        while (isdigit(input[*pos]) && strIndex < MAX_NUM_LENGTH) {
            numStr[strIndex++] = input[(*pos)++];
        }
        num = atoi(numStr);
        if (isdigit(input[*pos]) || num > MAX_REGISTER_VALUE) {
            foundError = handleIntegerOutOfRange(error, currentLine, pos);
        }
        else {
            if (isEndOfIntegerValid(pos, input, error, currentLine))
                *reg = num;
            else
                foundError = LOCAL_ERROR;
        }
    }
    return foundError;
}

/*
 * coordinates the process of reading the following register from the input, returns an error if a register wasn't found.
 */
int readRegister(int *pos, const char input[], errorCodes *error, long currentLine, int *reg) {
    int foundError;
    if (foundRegister(pos, input, error, currentLine)) {
        foundError = readRegNum(pos, input, currentLine, error, reg);
    }
    else {
        *error = ERROR_MISSING_ARGUMENT;
        printInputError(ERROR_MISSING_ARGUMENT, "", currentLine, *pos);
        foundError = LOCAL_ERROR;
    }
    return foundError;
}

/*
 * makes sure immed is within the bounds of 16 bits.
 */
int isImmedTooBig(int index, const int *pos, const char *input, long num ){
    int isTooBig = 0;
    if (index == MAX_IMMED_NUM_LENGTH && isdigit(input[*pos]) || num > MAX_SIZE_16_BITS || num < MIN_SIZE_16_BITS) {
        isTooBig = 1;
    }

    return isTooBig;
}

/*
 * reads an instant value from the input line, validates it, and converts it to a long.
 */
int readInstantValue(int *pos, const char input[], errorCodes *error, long currentLine, long *immed) {
    char num[MAX_IMMED_NUM_LENGTH] = {0};
    char *end;
    int numStrIndex = 0, foundError = 0;
    long temp;
    if (!isdigit(input[*pos]) && input[*pos] != '-' && input[*pos] != '+') {
        handleNANError(input[*pos], currentLine, error, pos);
        foundError = LOCAL_ERROR;
    }
    else {
        do {
            num[numStrIndex++] = input[(*pos)++];
        } while (isdigit(input[*pos]) && numStrIndex < MAX_IMMED_NUM_LENGTH);
        if (isEndOfIntegerValid(pos, input, error, currentLine)) {
            temp = strtol(num, &end, 10);
            if (end[0]) {
                handleInvalidCharacterError(end[0], currentLine, error, pos);
                foundError = LOCAL_ERROR;
            } else {
                if (isImmedTooBig(numStrIndex, pos, input, temp)) {
                    foundError = handleIntegerOutOfRange(error, currentLine, pos);
                } else {
                    *immed = temp;
                }
            }
        }
        else {
            foundError = LOCAL_ERROR;
        }
    }

    return foundError;
}

/*
 * checks to make sure there aren't any more arguments, since they are not expected at this point.
 */
int isEndOfInputValid(int *pos, const char input[], errorCodes *error, long currentLine) {
    int isValid = 1;
    skipWhiteSpaces(pos, input);
    if (input[*pos] && isgraph(input[*pos])) {
        *error = ERROR_EXCESSIVE_ARGUMENT;
        printInputError(ERROR_EXCESSIVE_ARGUMENT, "", currentLine, *pos);
        isValid = 0;
    }
    return isValid;
}

/*
 * adds an R word to the end of code image.
 */
void addRWord(int *pos, const char input[], errorCodes *error, long currentLine, codeNode **codeImageTail, int posInList,
              CMD *listOfCommands, int rs, int rt, int rd, long *IC) {
    if (isEndOfInputValid(pos, input, error, currentLine)){
        *codeImageTail = insertRWord(error, getOpcode(posInList, listOfCommands),
                                     getFunct(posInList, listOfCommands), rs, rt, rd, IC,
                                     *codeImageTail, currentLine);
    }
}

/*
 * adds an I word to the end of the code image.
 */
void addIWord(int *pos, const char input[], errorCodes *error, long currentLine, codeNode **codeImageTail, int posInList,
              CMD *listOfCommands, int rs, int rt, long immed, long *IC, char *label, typesOfCommands type) {
    if (isEndOfInputValid(pos, input, error, currentLine)){
        *codeImageTail = insertIWord(error, getOpcode(posInList, listOfCommands),
                                     rs, rt, immed, IC,
                                     *codeImageTail, label, type, currentLine);
    }
}

/*
 * reads an arithmetic R function (3 registers, no additional data)
 */
void arithmeticRFunctions(int *pos, const char input[], int posInList, CMD *listOfCommands, codeNode **codeImageTail,
                          long *IC, errorCodes *error, long currentLine) {
    int rs = -1, rt = -1, rd = -1;
    if (readRegister(pos, input, error, currentLine, &rs) != LOCAL_ERROR) {
        if (advanceToNextArgument(pos, input, currentLine, 0, error) != LOCAL_ERROR) {
            if (readRegister(pos, input, error, currentLine, &rt) != LOCAL_ERROR) {
                if (advanceToNextArgument(pos, input, currentLine, 0, error) != LOCAL_ERROR) {
                    if (readRegister(pos, input, error, currentLine, &rd) != LOCAL_ERROR) {
                        if (isEndOfInputValid(pos, input, error, currentLine)) {
                            addRWord(pos, input, error, currentLine, codeImageTail, posInList, listOfCommands, rs, rt,
                                     rd, IC);
                        }
                    }
                }
            }
        }
    }
}

/*
 * reads a copy R function (2 registers, no additional data)
 */
void copyRFunctions(int *pos, const char input[], int posInList, CMD *listOfCommands, codeNode **codeImageTail,
                    long *IC, errorCodes *error, long currentLine){
    int rs, rd, rt = 0;
    if (readRegister(pos, input, error, currentLine, &rs) != LOCAL_ERROR) {
        if (advanceToNextArgument(pos, input, currentLine, 0, error) != LOCAL_ERROR){
            if (readRegister(pos, input, error, currentLine, &rd) != LOCAL_ERROR) {
                if (isEndOfInputValid(pos, input, error, currentLine)) {
                    addRWord(pos, input, error, currentLine, codeImageTail, posInList, listOfCommands, rs, rt, rd, IC);
                }
            }
        }
    }
}

/*
 * reads an arithmetic or a loader I function (register, immediate value, register and no additional data)
 */
void arithmeticAndLoaderIFunctions(int *pos, const char *input, int posInList, CMD *listOfCommands, codeNode **codeImageTail,
                                   long *IC, errorCodes *error, long currentLine){
    int rs, rt;
    long immed;
    if (readRegister(pos, input, error, currentLine, &rs) != LOCAL_ERROR) {
        if (advanceToNextArgument(pos, input, currentLine, 0, error) != LOCAL_ERROR) {
            if (readInstantValue(pos, input, error, currentLine, &immed) != LOCAL_ERROR) {
                if (advanceToNextArgument(pos, input, currentLine, 0, error) != LOCAL_ERROR) {
                    if (readRegister(pos, input, error, currentLine, &rt) != LOCAL_ERROR) {
                        if (isEndOfInputValid(pos, input, error, currentLine)) {
                            addIWord(pos, input, error, currentLine, codeImageTail, posInList, listOfCommands, rs, rt,
                                     immed, IC, NULL, I_TYPE);
                        }
                    }
                }
            }
        }
    }
}

/*
 * reads a conditional I function (2 registers and a label, no additional data)
 */
void conditionalIFunctions(int *pos, const char input[], int posInList, CMD *listOfCommands, codeNode **codeImageTail,
                           long *IC, errorCodes *error, long currentLine){
    char *label = calloc(MAX_LABEL_LENGTH, sizeof(char));
    int rs, rt;
    long immed = *IC;
    if (readRegister(pos, input, error, currentLine, &rs) != LOCAL_ERROR) {
        if (advanceToNextArgument(pos, input, currentLine, 0, error) != LOCAL_ERROR) {
            if (readRegister(pos, input, error, currentLine, &rt) != LOCAL_ERROR) {
                if (advanceToNextArgument(pos, input, currentLine, 0, error) != LOCAL_ERROR) {
                    readInputLabel(pos, input, currentLine, error, label);
                    if (isEndOfInputValid(pos, input, error, currentLine) && !isFileError(*error)) {
                        addIWord(pos, input, error, currentLine, codeImageTail, posInList, listOfCommands, rs, rt,
                                 immed, IC, label, I_TYPE_CONDITIONAL);
                    }
                }
            }
        }
    }
}


/*
 * simply returning 1 or 0 whether a '$' sign was detected
 */
int isRegister(int *pos, const char input[]) {
    int res;
    skipWhiteSpaces(pos, input);
    res = input[*pos] == '$' ? 1 : 0;
    if (res) {
        (*pos)++;
    }
    return res;
}

/*
 * returns 1 if the command which called this function was jmp
 */
int isJmp(int posInList, CMD *listOfCommands) {
    return strcmp(getCMDName(posInList, listOfCommands), "jmp") ? 0 : 1;
}

/*
 * reads functions of type jmp, la or call and adds the data to the end of the code image.
 */
void jmpLaCall(int *pos, const char input[], int posInList, CMD *listOfCommands, codeNode **codeImageTail, long *IC, errorCodes *error,
         long currentLine){
    int reg = NOT_FOUND;
    if (isJmp(posInList, listOfCommands) && isRegister(pos, input)) {
        int foundError;
        foundError = readRegNum(pos, input, currentLine, error, &reg);
        if (foundError != LOCAL_ERROR && isEndOfInputValid(pos, input, error, currentLine) && reg != NOT_FOUND) {
            *codeImageTail = insertJWord(error, getOpcode(posInList, listOfCommands), 1, reg, IC, *codeImageTail, NULL, J_TYPE, currentLine);
        }
    }
    else {
        char *label = calloc(MAX_LABEL_LENGTH, sizeof(char));
        readInputLabel(pos, input, currentLine, error, label);
        if (isEndOfInputValid(pos, input, error, currentLine) && !isFileError(*error)) {
            *codeImageTail = insertJWord(error, getOpcode(posInList, listOfCommands), 0, 0, IC, *codeImageTail, label, J_TYPE, currentLine);
        }
    }
}


/*
 * adds stop to the end of the code image.
 */
void stop(int *pos, const char input[], int posInList, CMD *listOfCommands, codeNode **codeImageTail, long *IC,
          errorCodes *error, long currentLine){
    if (isEndOfInputValid(pos, input, error, currentLine)) {
        *codeImageTail = insertJWord(error, getOpcode(posInList, listOfCommands), 0, 0, IC, *codeImageTail, NULL, J_TYPE, currentLine);
    }
}
