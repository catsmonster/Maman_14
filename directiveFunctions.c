#include "directiveFunctions.h"
#include "inputUtils.h"
#include "fileErrors.h"
#include "dataTable.h"
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
int determineMaxLength(int type) {
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
int isNumTooBig(int numArrIndex, int maxLength, const int *pos, const char input[], char num[], int type) {
    int tooBig = 0;
    int compareToLimitations = num[0] == '-' ? strcmp(num + 1, type == DB_ASCIZ ? MIN_SIZE_8_BITS
            : type == DH ? MIN_SIZE_16_BITS : MIN_SIZE_32_BITS)
            : strcmp(num[0] == '+' ? num + 1 : num, type == DB_ASCIZ ? MAX_SIZE_8_BITS : type == DH ? MAX_SIZE_16_BITS
            : MAX_SIZE_32_BITS);
    if (numArrIndex > maxLength && isdigit(input[*pos]) || (numArrIndex >= maxLength - 1 && compareToLimitations == 1)) {
        tooBig = 1;
    }

    return tooBig;
}


/*
 * reads one number from the input line and stores it in the list
 */
int readNum(int *pos, const char input[],errorCodes *error, long currentLine, int type, long *list, int *listIndex) {
    char num[MAX_NUM_LENGTH] = {0};
    int numArrIndex = 0, foundError = 0;
    long givenNum;
    char *end;
    int maxLength = determineMaxLength(type);
    if (!isdigit(input[*pos]) && input[*pos] != '-' && input[*pos] != '+') {
        handleInvalidCharacterError(input[*pos], currentLine, error, pos);
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
            givenNum = strtol(num, &end, 10);
            list[(*listIndex)++] = givenNum;
        }
    }
    return foundError;
}


/*
 * coordinates the process of adding numbers to the list
 */
void readData(int *pos, const char input[], long *DC, errorCodes *error, dataNode **dataImageTail, long currentLine, int type) {
    int listIndex = 0, localError = 0;
    long *list = initializeList(error);
    if (list) {
        while (input[*pos] && input[*pos] != '\n' && !localError) {
            skipWhiteSpaces(pos, input);
            localError = readNum(pos, input, error, currentLine, type, list, &listIndex);
            if (localError != LOCAL_ERROR)
                advanceToNextArgument(pos, input, currentLine, 0, error);
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
    readData(pos, input, DC, error, dataImageTail, currentLine, DH);
}

/*
 * handles .dw directives
 */
void dw(int *pos, const char input[], long *DC, errorCodes *error, dataNode **dataImageTail, long currentLine) {
    readData(pos, input, DC, error, dataImageTail, currentLine, DW);
}

/*
 * handles .db directives
 */
void db(int *pos, const char input[], long *DC, errorCodes *error, dataNode **dataImageTail, long currentLine){
    readData(pos, input, DC, error, dataImageTail, currentLine, DB_ASCIZ);
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
            handleInvalidCharacterError(input[*pos], currentLine, error, pos);
        }
    }
}


/*
 * adds a label given as an argument to the linked list containing all the labels flagged as entries or extern.
 */
void entry(int *pos, const char input[], errorCodes *error, long currentLine,
           entriesOrExternList **entriesOrExternTail){
    char* labelName = calloc(MAX_LABEL_LENGTH, sizeof(char ));
    readInputLabel(pos, input, currentLine, error, labelName);
    if (!isFileError(*error)) {
        *entriesOrExternTail = setEntryOrExternListTail(labelName, ENTRY, *entriesOrExternTail, error, currentLine);
    }
}


/*
 * reads a label name as argument and adds it to the symbol list with the address 0 and the attribute external,
 * also adds the label name to the entry or extern list.
 */
void external(int *pos, const char input[], dataTable *table, errorCodes *error, long currentLine, entriesOrExternList **entriesOrExternTail){
    char* labelName = calloc(MAX_LABEL_LENGTH, sizeof(char ));
    int labelExists;
    readInputLabel(pos, input, currentLine, error, labelName);
    if (!isFileError(*error)) {
        labelExists = addItemToDataTable(labelName, 0, EXTERN, table);
        if (labelExists && getSymbolType(labelName, table, error, currentLine) != EXTERN) {
            *error = ERROR_LABEL_NAME_CONFLICT;
            printInputError(ERROR_LABEL_NAME_CONFLICT, labelName, currentLine, *pos);
        }
        else {
            *entriesOrExternTail = setEntryOrExternListTail(labelName, EXTERN, *entriesOrExternTail, error, currentLine);
        }
    }
}
