#include "../includes/directiveFunctions.h"
#include "../includes/inputUtils.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define MAX_NUM_LENGTH 12
#define LIST_MAX_LENGTH 80
#define DW_MAX_STRING_LENGTH 11
#define DH_MAX_STRING_LENGTH 6
#define DB_MAX_STRING_LENGTH 4
#define MIN_SIZE_32_BITS "2147483648"
#define MAX_SIZE_32_BITS "2147483647"
#define MIN_SIZE_16_BITS "32768"
#define MAX_SIZE_16_BITS "32767"
#define MIN_SIZE_8_BITS "128"
#define MAX_SIZE_8_BITS "127"
#define MAX_LABEL_LENGTH 32

/*
 * creates a list of type long to store the information for later conversion to the data image
 */
long *initializeList(errorCodes *error) {
    long *list = calloc(LIST_MAX_LENGTH, sizeof(long));
    if (!list){
        printMemoryError(ERROR_MEMORY_ALLOCATION);
        *error = ERROR_MEMORY_ALLOCATION;
    }
    return list;

}

/*
 * determines what is the longest string (including null character and minus sign) for each data type
 */
int determineMaxLength(typeOfData type) {
    int maxLength = 0;
    if (type == DW) {
        maxLength = DW_MAX_STRING_LENGTH;
    }
    if (type == DH) {
        maxLength = DH_MAX_STRING_LENGTH;
    }
    if (type == DB_ASCIZ) {
        maxLength = DB_MAX_STRING_LENGTH;
    }
    return maxLength;
}

/*
 * determines if the number is too big for the data type, checking whether the string is at
 * max length, and if so, performing a string comparison with the known limits of each data type.
 */
int isNumTooBig(int numArrIndex, int maxLength, const int *pos, const char input[], char num[], typeOfData type) {
    int tooBig = 0;
    int compareToLimitations = num[0] == '-' ? strcmp(num + 1, type == DB_ASCIZ ? MIN_SIZE_8_BITS
            : type == DH ? MIN_SIZE_16_BITS : MIN_SIZE_32_BITS)
            : strcmp(num[0] == '+' ? num + 1 : num, type == DB_ASCIZ ? MAX_SIZE_8_BITS : type == DH ? MAX_SIZE_16_BITS
            : MAX_SIZE_32_BITS);
    if ((numArrIndex > maxLength && isdigit(input[*pos])) || (numArrIndex >= maxLength - 1 && compareToLimitations == 1)) {
        tooBig = 1;
    }

    return tooBig;
}

/*
 * converts the string representation of the number to a long, and if no errors are found, adds it to the list and updates
 * the index. returns 0 if successful or a LOCAL_ERROR code in case of failure.
 */
errorCodes convertToLongAndInsertToList(int *pos, char num[], long currentLine, errorCodes *error, long *list, int *listIndex) {
    char *end;
    long givenNum;
    errorCodes foundError = 0;
    givenNum = strtol(num, &end, 10);
    if (end[0]) {
        handleNANError(end[0], currentLine, error, pos);
        *error = ERROR_NOT_AN_INTEGER;
        foundError = LOCAL_ERROR;
    } else
        list[(*listIndex)++] = givenNum;
    return foundError;
}

/*
 * reads one number from the input line and stores it in the list
 */
int readNum(int *pos, const char input[],errorCodes *error, long currentLine, typeOfData type, long *list, int *listIndex) {
    char num[MAX_NUM_LENGTH] = {0};
    errorCodes foundError;
    int numArrIndex = 0, maxLength = determineMaxLength(type);
    if (!isdigit(input[*pos]) && input[*pos] != '-' && input[*pos] != '+') {
        handleNANError(input[*pos], currentLine, error, pos);
        foundError = LOCAL_ERROR;
    }
    else {
        do {
            num[numArrIndex++] = input[(*pos)++];
        }
        while (isdigit(input[*pos]) && numArrIndex <= maxLength);
        if (isNumTooBig(numArrIndex, maxLength, pos, input, num, type)) {
            *error = ERROR_INTEGER_OUT_OF_RANGE;
            printInputError(ERROR_INTEGER_OUT_OF_RANGE, "", currentLine, *pos);
            foundError = LOCAL_ERROR;
        }
        else {
            foundError = convertToLongAndInsertToList(pos, num, currentLine, error, list, listIndex);
        }
    }
    return foundError;
}


/*
 * coordinates the process of adding numbers to the list
 */
void readNumbersSeperatedByCommas(int *pos, const char input[], long *DC, errorCodes *error, dataNode **dataImageTail, long currentLine, typeOfData type) {
    int listIndex = 0, localError = 0;
    long *list = initializeList(error);
    if (list) {
        while (input[*pos] && input[*pos] != '\n' && !localError) {
            skipWhiteSpaces(pos, input);
            localError = readNum(pos, input, error, currentLine, type, list, &listIndex);
            if (localError != LOCAL_ERROR)
                localError = advanceToNextArgument(pos, input, currentLine, error);
        }
        if (!localError) {
            if (listIndex > 0) {
                *dataImageTail = insertLongArrayToDataImage(error, DC, type, *dataImageTail, listIndex, list);
            } else {
                *error = ERROR_MISSING_DATA;
                printInputError(ERROR_MISSING_DATA, "", currentLine, *pos);
            }
        }
        free(list);
    }
}

/*
 * validates the string and sends the list to be inserted to the data image
 */
void validateString(int *pos, const char input[], errorCodes *error, long *DC, long currentLine, long *list, dataNode **dataImageTail, int index) {
    if (input[*pos] == '"') {
        (*pos)++;
        skipWhiteSpaces(pos, input);
        if (isgraph(input[(*pos)])) {
            handleInvalidCharacterError(input[*pos], currentLine, error, pos);
        }
        else {
            *dataImageTail = insertLongArrayToDataImage(error, DC, DB_ASCIZ, *dataImageTail, index + 1, list);
        }
    }
    else {
        *error = ERROR_MISSING_QUOTATION;
        printInputError(ERROR_MISSING_QUOTATION, "", currentLine, *pos);
    }
}

/*
 * stores a string as a list of type long
 */
void readString(int *pos, const char input[], long currentLine, long *DC, dataNode **dataImageTail, errorCodes *error) {
    int index = 0, flag = 0;
    long *list = initializeList(error);
    if (list) {
        while (input[*pos] && (isgraph(input[*pos]) || isspace(input[*pos])) && !flag) {
            if (input[*pos] != '"' || input[(*pos) - 1] == '\\') {
                list[index++] = (unsigned char)input[(*pos)++];
            }
            else {
                flag = 1;
            }
        }
        validateString(pos, input, error, DC, currentLine, list, dataImageTail, index);
    }

    free(list);
}

/*
 * handles .dh directives
 */
void dh(int *pos, const char input[], long *DC, errorCodes *error, dataNode **dataImageTail, long currentLine) {
    readNumbersSeperatedByCommas(pos, input, DC, error, dataImageTail, currentLine, DH);
}

/*
 * handles .dw directives
 */
void dw(int *pos, const char input[], long *DC, errorCodes *error, dataNode **dataImageTail, long currentLine) {
    readNumbersSeperatedByCommas(pos, input, DC, error, dataImageTail, currentLine, DW);
}

/*
 * handles .db directives
 */
void db(int *pos, const char input[], long *DC, errorCodes *error, dataNode **dataImageTail, long currentLine){
    readNumbersSeperatedByCommas(pos, input, DC, error, dataImageTail, currentLine, DB_ASCIZ);
}

/*
 * handles .asciz directives
 */
void asciz(int *pos, const char input[], long *DC, errorCodes *error, dataNode **dataImageTail, long currentLine){
    skipWhiteSpaces(pos, input);
    if (input[*pos] == '"') {
        (*pos)++;
        readString(pos, input, currentLine, DC, dataImageTail, error);
    }
    else {
        if (!input[*pos] || input[*pos] == '\n') {
            *error = ERROR_MISSING_DATA;
            printInputError(ERROR_MISSING_DATA, "", currentLine, *pos);
        }
        else {
            *error = ERROR_MISSING_QUOTATION;
            printInputError(ERROR_MISSING_QUOTATION, "", currentLine, *pos);
        }
    }
}


/*
 * adds the label given as an argument to a linked list of entries
 */
void entry(int *pos, const char input[], errorCodes *error, long currentLine, entriesList **entriesTail, entriesList *entriesHead){
    char* labelName = calloc(MAX_LABEL_LENGTH, sizeof(char ));
    if (readInputLabel(pos, input, currentLine, error, labelName) != LOCAL_ERROR) {
        if (!entryExists(labelName, entriesHead))
            *entriesTail = setEntryListTail(labelName, *entriesTail, error, currentLine);
        else
            free(labelName);
    }
    else
        free(labelName);
}


/*
 * reads a label name as argument and adds it to the symbol list with the address 0 and the attribute external,
 */
void external(int *pos, const char input[], dataTable *table, errorCodes *error, long currentLine){
    char* labelName = calloc(MAX_LABEL_LENGTH, sizeof(char ));
    int labelExists;
    if (readInputLabel(pos, input, currentLine, error, labelName) != LOCAL_ERROR) {
        labelExists = addItemToDataTable(labelName, 0, EXTERN, table);
        if (labelExists) {
            if (getSymbolType(labelName, table, error, currentLine) != EXTERN) {
                *error = ERROR_LABEL_NAME_CONFLICT;
                printInputError(ERROR_LABEL_NAME_CONFLICT, labelName, currentLine, *pos);
            }
            free(labelName);
        }
    }
    else
        free(labelName);
}
