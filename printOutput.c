
#include "printOutput.h"
#include <string.h>
#include <stdlib.h>
#define OB_EXTENSION_LENGTH 4
#define ENT_EXT_EXTENSION_LENGTH 5
#define IC_START_VALUE 100
#define BYTES_IN_LINE 4

enum {
    OB_FILE = 700,
    EXT_FILE,
    ENT_FILE
};

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
char * removeExtension(char * assemblyFile, int type) {
    char * noExtensionFile;
    int i = getExtensionPos(assemblyFile);
    noExtensionFile = (char *)calloc(i + (type == OB_FILE ? OB_EXTENSION_LENGTH : ENT_EXT_EXTENSION_LENGTH), 1);
    memcpy(noExtensionFile, assemblyFile, i);
    return noExtensionFile;
}

/*
 * changes the extension of the file to .ob, .ent or .ext
 */
char * processFileName(char * assemblyFile, int type) {
    char * processedName = removeExtension(assemblyFile, type);
    strcat(processedName, type == OB_FILE ? ".ob" : type == ENT_FILE ? ".ent" : ".ext");
    return processedName;
}

void printICAndDC(FILE *fp, long IC, long DC) {
    fprintf(fp, "\t\t%ld\t%ld\n", IC - IC_START_VALUE, DC);
}

void printCodeWords(FILE *fp, codeNode *codeImageHead) {
    while (codeImageHead) {
        fprintf(fp, "%04ld\t%02X\t%02X\t%02X\t%02X\n", getCodeNodeAddress(codeImageHead),
                getCodeRepresentation(codeImageHead, FIRST_BYTE), getCodeRepresentation(codeImageHead, SECOND_BYTE),
                getCodeRepresentation(codeImageHead, THIRD_BYTE), getCodeRepresentation(codeImageHead, FORTH_BYTE));
        codeImageHead = getNextCodeNode(codeImageHead);
    }
}

void newLineOrTab(FILE *fp, int *printedDataCounter, long ICF, long *dataAddress) {
    if (*printedDataCounter % BYTES_IN_LINE == 0) {
        if (*dataAddress != ICF) {
            fputc('\n', fp);
        }
        fprintf(fp, "%04ld\t", *dataAddress);
        *dataAddress += BYTES_IN_LINE;
        *printedDataCounter = 0;
    }
    else {
        fputc('\t', fp);
    }
}


void printDataByte(dataNode *dataImageHead, long *dataAddress, int *printedDataCounter, long IC, FILE *fp, int size) {
    int i;
    for (i = FIRST_DATA_BYTE; i < size + FIRST_DATA_BYTE; i++) {
        fprintf(fp, "%02X", getDataRepresentation(dataImageHead, i, printedDataCounter));
        newLineOrTab(fp, printedDataCounter, IC, dataAddress);
    }
}


void printData(FILE *fp, dataNode *dataImageHead, long IC) {
    int printedDataCounter = 0;
    long dataAddress = IC;
    while (dataImageHead) {
        int dataType;
        dataType = getDataType(dataImageHead);
        if (IC == dataAddress) {
            newLineOrTab(fp, &printedDataCounter, IC, &dataAddress);
        }
        switch (dataType) {
            case DB_ASCIZ:
                printDataByte(dataImageHead, &dataAddress, &printedDataCounter, IC, fp, SIZE_OF_DB);
                break;
            case DH:
                printDataByte(dataImageHead, &dataAddress, &printedDataCounter, IC, fp, SIZE_OF_DH);
                break;
            default:
                printDataByte(dataImageHead, &dataAddress, &printedDataCounter, IC, fp, SIZE_OF_DW);
        }
        dataImageHead = getNextDataNode(dataImageHead);
    }
}

void printOBFile(codeNode *codeImageHead, dataNode *dataImageHead, long IC, long DC, char *fileName){
    FILE *fp;
    char *outputFileName = processFileName(fileName, OB_FILE);
    fp = fopen(outputFileName, "w");
    printICAndDC(fp, IC, DC);
    printCodeWords(fp, codeImageHead);
    printData(fp, dataImageHead, IC);
    fclose(fp);
}

FILE *generateExtFile(char *fileName){
    FILE *fp;
    char *outputFileName = processFileName(fileName, EXT_FILE);
    fp = fopen(outputFileName, "w");
    return fp;
}


FILE *generateEntFile(char *fileName){
    FILE *fp;
    char *outputFileName = processFileName(fileName, ENT_FILE);
    fp = fopen(outputFileName, "w");
    return fp;
}

void printEntOrExtLine(FILE **fp, long address, char *label) {
    fprintf(*fp, "%s\t%04ld\n", label, address);
}

void deleteEntExtFiles(char *fileName) {
    remove(processFileName(fileName, ENT_FILE));
    remove(processFileName(fileName, EXT_FILE));
}

