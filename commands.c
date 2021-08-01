
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

int isEndOfIntegerValid(int *pos, const char *input, errorCodes *error, long currentLine) {
    int valid = 1;
    if (!isspace(input[*pos]) && input[*pos] != ',' && !isdigit(input[*pos])) {
        handleNANError(input[*pos], currentLine, error, pos);
        valid = 0;
    }
    return valid;
}


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
            *error = ERROR_INTEGER_OUT_OF_RANGE;
            printInputError(ERROR_INTEGER_OUT_OF_RANGE, "", currentLine, *pos);
            foundError = LOCAL_ERROR;
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

int isImmedTooBig(int index, const int *pos, const char *input, long num ){
    int isTooBig = 0;
    if (index == MAX_IMMED_NUM_LENGTH && isdigit(input[*pos]) || num > MAX_SIZE_16_BITS || num < MIN_SIZE_16_BITS) {
        isTooBig = 1;
    }

    return isTooBig;
}

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
                    *error = ERROR_INTEGER_OUT_OF_RANGE;
                    printInputError(ERROR_INTEGER_OUT_OF_RANGE, "", currentLine, *pos);
                    foundError = LOCAL_ERROR;
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

void readLabel(int *pos, const char input[], errorCodes *error, long currentLine, char *label) {
    int index = 0;
    skipWhiteSpaces(pos, input);
    if (!isalpha(input[*pos])) {
        handleInvalidCharacterError(input[*pos], currentLine, error, pos);
    }
    else {
        while (isalnum(input[*pos]) && index < MAX_LABEL_LENGTH) {
            label[index++] = input[(*pos)++];
        }
    }
    skipWhiteSpaces(pos, input);
    if (input[*pos] && isgraph(input[*pos])) {
        handleInvalidCharacterError(input[*pos], currentLine, error, pos);
    }
}

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

void addRWord(int *pos, const char input[], errorCodes *error, long currentLine, codeNode **codeImageTail, int posInList,
              CMD *listOfCommands, int rs, int rt, int rd, long *IC) {
    if (isEndOfInputValid(pos, input, error, currentLine)){
        *codeImageTail = insertRWord(error, getOpcode(posInList, listOfCommands),
                                     getFunct(posInList, listOfCommands), rs, rt, rd, IC,
                                     *codeImageTail, currentLine);
    }
}

void addIWord(int *pos, const char input[], errorCodes *error, long currentLine, codeNode **codeImageTail, int posInList,
              CMD *listOfCommands, int rs, int rt, long immed, long *IC, char *label, typesOfCommands type) {
    if (isEndOfInputValid(pos, input, error, currentLine)){
        *codeImageTail = insertIWord(error, getOpcode(posInList, listOfCommands),
                                     rs, rt, immed, IC,
                                     *codeImageTail, label, type, currentLine);
    }
}

void arithmeticRFunctions(int *pos, const char input[], int posInList, CMD *listOfCommands, codeNode **codeImageTail,
                          long *IC, errorCodes *error, long currentLine) {
    int rs = -1, rt = -1, rd = -1;
    if (readRegister(pos, input, error, currentLine, &rs) != LOCAL_ERROR) {
        if (advanceToNextArgument(pos, input, currentLine, 0, error) != LOCAL_ERROR) {
            if (readRegister(pos, input, error, currentLine, &rt) != LOCAL_ERROR) {
                if (advanceToNextArgument(pos, input, currentLine, 0, error) != LOCAL_ERROR) {
                    if (readRegister(pos, input, error, currentLine, &rd) != LOCAL_ERROR) {
                        addRWord(pos, input, error, currentLine, codeImageTail, posInList, listOfCommands, rs, rt, rd, IC);
                    }
                }
            }
        }
    }
}

void copyRFunctions(int *pos, const char input[], int posInList, CMD *listOfCommands, codeNode **codeImageTail,
                    long *IC, errorCodes *error, long currentLine){
    int rs, rd, rt = 0;
    if (readRegister(pos, input, error, currentLine, &rs) != LOCAL_ERROR) {
        if (advanceToNextArgument(pos, input, currentLine, 0, error) != LOCAL_ERROR){
            if (readRegister(pos, input, error, currentLine, &rd) != LOCAL_ERROR) {
                addRWord(pos, input, error, currentLine, codeImageTail, posInList, listOfCommands, rs, rt, rd, IC);
            }
        }
    }
}

void arithmeticAndLoaderIFunctions(int *pos, const char *input, int posInList, CMD *listOfCommands, codeNode **codeImageTail,
                                   long *IC, errorCodes *error, long currentLine){
    int rs, rt;
    long immed;
    if (readRegister(pos, input, error, currentLine, &rs) != LOCAL_ERROR) {
        if (advanceToNextArgument(pos, input, currentLine, 0, error) != LOCAL_ERROR) {
            if (readInstantValue(pos, input, error, currentLine, &immed) != LOCAL_ERROR) {
                if (advanceToNextArgument(pos, input, currentLine, 0, error) != LOCAL_ERROR) {
                    if (readRegister(pos, input, error, currentLine, &rt) != LOCAL_ERROR) {
                        addIWord(pos, input, error, currentLine, codeImageTail, posInList, listOfCommands, rs, rt, immed, IC, NULL, I_TYPE);
                    }
                }
            }
        }
    }
}

void conditionalIFunctions(int *pos, const char input[], int posInList, CMD *listOfCommands, codeNode **codeImageTail,
                           long *IC, errorCodes *error, long currentLine){
    char *label = calloc(MAX_LABEL_LENGTH, sizeof(char));
    int rs, rt;
    long immed = *IC;
    if (readRegister(pos, input, error, currentLine, &rs) != LOCAL_ERROR) {
        if (advanceToNextArgument(pos, input, currentLine, 0, error) != LOCAL_ERROR) {
            if (readRegister(pos, input, error, currentLine, &rt) != LOCAL_ERROR) {
                if (advanceToNextArgument(pos, input, currentLine, 0, error) != LOCAL_ERROR) {
                    readLabel(pos, input, error, currentLine, label);
                    if (!isFileError(*error)) {
                        addIWord(pos, input, error, currentLine, codeImageTail, posInList, listOfCommands, rs, rt,
                                 immed, IC, label, I_TYPE_CONDITIONAL);
                    }
                }
            }
        }
    }
}



int isRegister(int *pos, const char input[]) {
    int res;
    skipWhiteSpaces(pos, input);
    res = input[*pos] == '$' ? 1 : 0;
    if (res) {
        (*pos)++;
    }
    return res;
}

int isJmp(int posInList, CMD *listOfCommands) {
    return strcmp(getCMDName(posInList, listOfCommands), "jmp") ? 0 : 1;
}

void jmpLaCall(int *pos, const char input[], int posInList, CMD *listOfCommands, codeNode **codeImageTail, long *IC, errorCodes *error,
         long currentLine){
    int reg = -1;
    if (isJmp(posInList, listOfCommands) && isRegister(pos, input)) {
        int foundError;
        foundError = readRegNum(pos, input, currentLine, error, &reg);
        if (foundError != LOCAL_ERROR && isEndOfInputValid(pos, input, error, currentLine) && reg != -1) {
            *codeImageTail = insertJWord(error, getOpcode(posInList, listOfCommands), 1, reg, IC, *codeImageTail, NULL, J_TYPE, currentLine);
        }
    }
    else {
        char *label = calloc(MAX_LABEL_LENGTH, sizeof(char));
        readLabel(pos, input, error, currentLine, label);
        if (!isFileError(*error)) {
            *codeImageTail = insertJWord(error, getOpcode(posInList, listOfCommands), 0, 0, IC, *codeImageTail, label, J_TYPE, currentLine);
        }
    }
}



void stop(int *pos, const char input[], int posInList, CMD *listOfCommands, codeNode **codeImageTail, long *IC,
          errorCodes *error, long currentLine){
    skipWhiteSpaces(pos, input);
    if (input[*pos] && isgraph(input[*pos])) {
        *error = ERROR_EXCESSIVE_ARGUMENT;
        printInputError(ERROR_EXCESSIVE_ARGUMENT, "", currentLine, *pos);
    }
    else {
        *codeImageTail = insertJWord(error, getOpcode(posInList, listOfCommands), 0, 0, IC, *codeImageTail, NULL, J_TYPE, currentLine);
    }
}
