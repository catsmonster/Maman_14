
#include "../includes/printOutput.h"
#include <string.h>
#include <stdlib.h>
#define OB_EXTENSION_LENGTH 4
#define ENT_EXT_EXTENSION_LENGTH 5
#define IC_START_VALUE 100
#define BYTES_IN_LINE 4


/*
 * returns the position of the file extension
 */
int getExtensionPos(const char * assemblyFile) {
    int lastPeriodPos = -1;
    int i = 0;
    while (assemblyFile[++i]) {
        if (assemblyFile[i] == '.')
            lastPeriodPos = i;
    }
    return lastPeriodPos;
}

/*
 * removes the .as extensions from the file name
 */
char * removeExtension(char * assemblyFile, fileTypes type) {
    char * noExtensionFile;
    int i = getExtensionPos(assemblyFile);
    noExtensionFile = (char *)calloc(i + (type == OB_FILE ? OB_EXTENSION_LENGTH : ENT_EXT_EXTENSION_LENGTH), 1);
    memcpy(noExtensionFile, assemblyFile, i);
    return noExtensionFile;
}

/*
 * changes the extension of the file to .ob, .ent or .ext
 */
char * processFileName(char * assemblyFile, fileTypes type) {
    char * processedName = removeExtension(assemblyFile, type);
    strcat(processedName, type == OB_FILE ? ".ob" : type == ENT_FILE ? ".ent" : ".ext");
    return processedName;
}

/*
 * prints the number of instruction words and the number of data words in decimal values in the top of the .ob file
 */
void printICAndDC(FILE *fp, long IC, long DC) {
    fprintf(fp, "\t\t%ld\t%ld\n", IC - IC_START_VALUE, DC);
}

/*
 * prints the code words 4 bytes in each line, starting with a decimal address presented in 4 digits, and followed by
 * the bytes in little endian of the instruction word, in hexadecimal values of 2 digits each.
 */
void printCodeWords(FILE *fp, codeNode *codeImageHead) {
    while (codeImageHead) {
        fprintf(fp, "%04ld\t%02X\t%02X\t%02X\t%02X\n", getCodeNodeAddress(codeImageHead),
                getCodeRepresentation(codeImageHead, FIRST_BYTE), getCodeRepresentation(codeImageHead, SECOND_BYTE),
                getCodeRepresentation(codeImageHead, THIRD_BYTE), getCodeRepresentation(codeImageHead, FORTH_BYTE));
        codeImageHead = getNextCodeNode(codeImageHead);
    }
}

/*
 * either prints a new line with an address and a tab, or just a tab (exceptions for first and last values printed).
 */
void newLineOrTab(FILE *fp, int *printedDataCounter, long ICF, long *dataAddress, long DC) {
    if (*printedDataCounter % BYTES_IN_LINE == 0) {
        if (*dataAddress != ICF) {
            fputc('\n', fp);
        }
        if (*dataAddress != DC + ICF)
            fprintf(fp, "%04ld\t", *dataAddress);
        *dataAddress += BYTES_IN_LINE;
        *printedDataCounter = 0;
    }
    else {
        fputc('\t', fp);
    }
}

/*
 * prints the representation of the data words in little endian, 1 byte at a time - presented as a hexadecimal value of
 * 2 digits, following the format of 4 bytes in a line
 */
void printDataByte(dataNode *dataImageHead, long *dataAddress, int *printedDataCounter, long IC, FILE *fp, int size, long DC) {
    dataByte i;
    for (i = FIRST_DATA_BYTE; i < size + FIRST_DATA_BYTE; i++) {
        fprintf(fp, "%02X", getDataRepresentation(dataImageHead, i, printedDataCounter));
        newLineOrTab(fp, printedDataCounter, IC, dataAddress, DC);
    }
}

/*
 * iterates over the data image and prints each node according to its specified type
 */
void printData(FILE *fp, dataNode *dataImageHead, long IC, long DC) {
    int printedDataCounter = 0;
    long dataAddress = IC;
    while (dataImageHead) {
        typeOfData dataType;
        dataType = getDataType(dataImageHead);
        if (IC == dataAddress) {
            newLineOrTab(fp, &printedDataCounter, IC, &dataAddress, DC);
        }
        switch (dataType) {
            case DB_ASCIZ:
                printDataByte(dataImageHead, &dataAddress, &printedDataCounter, IC, fp, SIZE_OF_DB, DC);
                break;
            case DH:
                printDataByte(dataImageHead, &dataAddress, &printedDataCounter, IC, fp, SIZE_OF_DH, DC);
                break;
            default:
                printDataByte(dataImageHead, &dataAddress, &printedDataCounter, IC, fp, SIZE_OF_DW, DC);
        }
        dataImageHead = getNextDataNode(dataImageHead);
    }
}

/*
 * handles the printing process of the .ob file
 */
void printOBFile(codeNode *codeImageHead, dataNode *dataImageHead, long IC, long DC, char *fileName){
    FILE *fp;
    char *outputFileName = processFileName(fileName, OB_FILE);
    fp = fopen(outputFileName, "w");
    printICAndDC(fp, IC, DC);
    printCodeWords(fp, codeImageHead);
    printData(fp, dataImageHead, IC, DC);
    fclose(fp);
    free(outputFileName);
}

/*
 * generates an .ext or .ent file with the same name as the input file, returning its pointer
 */
FILE *generateExtOrEntFile(char *fileName, fileTypes type){
    FILE *fp;
    char *outputFileName = processFileName(fileName, type);
    fp = fopen(outputFileName, "w");
    free(outputFileName);
    return fp;
}

/*
 * prints a line to the .ent or .ext file with the name of the label and the address of its usage in 4 a digits decimal
 */
void printEntOrExtLine(FILE **fp, long address, char *label) {
    fprintf(*fp, "%s\t%04ld\n", label, address);
}

/*
 * deletes .ent and .ext files if any exist (in case an error was encountered)
 */
void deleteEntExtFiles(char *fileName, FILE *entFile, FILE *extFile) {
    if (entFile)
        remove(processFileName(fileName, ENT_FILE));
    if (extFile)
        remove(processFileName(fileName, EXT_FILE));
}

