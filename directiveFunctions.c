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
long *initializeList(int *error) {
    long *list = calloc(LIST_MAX_LENGTH, sizeof(long));
    if (!list){
        printError(ERROR_MEMORY_ALLOCATION, ERROR_TYPE_MEMORY, 0);
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
            : strcmp(num, type == DB_ASCIZ ? MAX_SIZE_8_BITS : type == DH ? MAX_SIZE_16_BITS : MAX_SIZE_32_BITS);
    if (numArrIndex > maxLength && isdigit(input[*pos]) || (numArrIndex >= maxLength - 1 && compareToLimitations == 1)) {
        tooBig = 1;
    }

    return tooBig;
}


/*
 * reads one number from the input line and stores it in the list
 */
void readNum(int *pos, const char input[],int *error, long currentLine, int type, long *list, int *listIndex) {
    char num[MAX_NUM_LENGTH] = {0};
    int numArrIndex = 0;
    long givenNum;
    char *end;
    int maxLength = determineMaxLength(type);
    if (!isdigit(input[*pos]) && input[*pos] != '-') {
        handleInvalidCharacterError(input[*pos], currentLine, error, pos);
    }
    else {
        do {
            num[numArrIndex++] = input[(*pos)++];
        }
        while (isdigit(input[*pos]) && numArrIndex <= maxLength);
        if (isNumTooBig(numArrIndex, maxLength, pos, input, num, type)) {
            *error = ERROR_INTEGER_OUT_OF_RANGE;
            printError(ERROR_INTEGER_OUT_OF_RANGE, ERROR_TYPE_INPUT, 3, "", currentLine, *pos);
        }
        else {
            givenNum = strtol(num, &end, 10);
            list[(*listIndex)++] = givenNum;
        }
    }
}


/*
 * coordinates the process of adding numbers to the list
 */
void readData(int *pos, const char input[], long *DC, int *error, dataNode **dataImageTail, long currentLine, int type) {
    int listIndex = 0;
    long *list = initializeList(error);
    if (list) {
        while (input[*pos] && input[*pos] != '\n') {
            skipWhiteSpaces(pos, input);
            readNum(pos, input, error, currentLine, type, list, &listIndex);
            advanceToNextArgument(pos, input, currentLine, 0, error);
        }
        if (listIndex > 0) {
            *dataImageTail = insertLongArrayToDataImage(error, DC, type, *dataImageTail, listIndex, list);
        }
        else {
            *error = ERROR_MISSING_DATA;
            printError(ERROR_MISSING_DATA, ERROR_TYPE_INPUT, 3, "", currentLine, *pos);
        }

        free(list);
    }
}

/*
 * validates the string and sends the list to be inserted to the data image
 */
void validateString(int *pos, const char input[], int *error, long *DC, long currentLine, long *list, dataNode **dataImageTail, int index) {
    if (input[*pos] == '"') {
        if (isgraph(input[(*pos) + 1])) {
            handleInvalidCharacterError(input[(*pos) + 1], currentLine, error, pos);
        }
        else {
            *dataImageTail = insertLongArrayToDataImage(error, DC, DB_ASCIZ, *dataImageTail, index + 1, list);
        }
    }
    else {
        *error = ERROR_MISSING_QUOTATION;
        printError(ERROR_MISSING_QUOTATION, ERROR_TYPE_INPUT, 3, "", currentLine, *pos);
    }
}

/*
 * stores a string as a list of type long
 */
void readString(int *pos, const char input[], long currentLine, long *DC, dataNode **dataImageTail, int *error) {
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
void dh(int *pos, const char input[], long *DC, int *error, dataNode **dataImageTail, long currentLine) {
    readData(pos, input, DC, error, dataImageTail, currentLine, DH);
}

/*
 * handles .dw directives
 */
void dw(int *pos, const char input[], long *DC, int *error, dataNode **dataImageTail, long currentLine) {
    readData(pos, input, DC, error, dataImageTail, currentLine, DW);
}

/*
 * handles .db directives
 */
void db(int *pos, const char input[], long *DC, int *error, dataNode **dataImageTail, long currentLine){
    readData(pos, input, DC, error, dataImageTail, currentLine, DB_ASCIZ);
}

/*
 * handles .asciz directives
 */
void asciz(int *pos, const char input[], long *DC, int *error, dataNode **dataImageTail, long currentLine){
    skipWhiteSpaces(pos, input);
    if (input[*pos] == '"') {
        (*pos)++;
        readString(pos, input, currentLine, DC, dataImageTail, error);
    }
    else {
        if (!input[*pos] || input[*pos] == '\n') {
            *error = ERROR_MISSING_DATA;
            printError(ERROR_MISSING_DATA, ERROR_TYPE_INPUT, 3, "", currentLine, *pos);
        }
        else {
            handleInvalidCharacterError(input[*pos], currentLine, error, pos);
        }
    }
}

/*
 * checks whether the input argument for extern or entry is legal
 */
void validateLabelName(int *pos, const char input[], int *error, long currentLine, int index) {
    if (index >= MAX_LABEL_LENGTH && isalnum(input[*pos])) {
        *error = ERROR_MAX_LENGTH;
        printError(ERROR_MAX_LENGTH, ERROR_TYPE_INPUT, 3, "", currentLine, *pos);
    }
    else if (isgraph(input[*pos])) {
        handleInvalidCharacterError(input[*pos], currentLine, error, pos);
    }
    skipWhiteSpaces(pos, input);
    if (isgraph(input[*pos])) {
        handleInvalidCharacterError(input[*pos], currentLine, error, pos);
    }
}

void readInputLabel(int *pos, const char *input, long currentLine, int *error, char *labelName) {
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


void entry(int *pos, const char input[], int *error, long currentLine,
           entriesOrExtern **entriesOrExternTail){
    char* labelName = calloc(MAX_LABEL_LENGTH, sizeof(char ));
    readInputLabel(pos, input, currentLine, error, labelName);
    if (!isFileError(*error)) {
        *entriesOrExternTail = setEntryOrExternTail(labelName, ENTRY, *entriesOrExternTail, error, currentLine);
    }
}


/*
 * reads a label name as argument and adds it to the symbol list with the address 0 and the attribute external
 */
void external(int *pos, const char input[], dataTable *table, int *error, long currentLine, entriesOrExtern **entriesOrExternTail){
    char* labelName = calloc(MAX_LABEL_LENGTH, sizeof(char ));
    int labelExists;
    readInputLabel(pos, input, currentLine, error, labelName);
    if (!isFileError(*error)) {
        labelExists = addItemToDataTable(labelName, 0, EXTERN, table);
        if (labelExists && getSymbolType(labelName, table, error, currentLine) != EXTERN) {
            *error = ERROR_LABEL_NAME_CONFLICT;
            printError(ERROR_LABEL_NAME_CONFLICT, ERROR_TYPE_INPUT, 3, labelName, currentLine, *pos);
        }
        else {
            *entriesOrExternTail = setEntryOrExternTail(labelName, EXTERN, *entriesOrExternTail, error, currentLine);
        }
    }
}
