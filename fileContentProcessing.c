#include <stdio.h>
#include "firstIteration.h"
#include "directiveFunctions.h"
#include "fileContentProcessing.h"
#include "secondIteration.h"
#include "fileErrors.h"
#include "printOutput.h"
#define INSTRUCTIONS_COUNTER_START 100
#define DATA_COUNTER_START 0

/*
 * reading the assembly file and creating the necessary data structures before converting to machine code
 */
void readFile(FILE * fp, void (*directivesArr[])(int *, const char[], long *, errorCodes *, dataNode**, long),
              CMD *listOfCommands, dataTable *listOfSymbols, dataNode *dataImageHead, codeNode *codeImageHead,
              entriesOrExternList *entriesOrExternHead, char *fileName) {

    errorCodes error = 0;
    long DC = DATA_COUNTER_START, IC = INSTRUCTIONS_COUNTER_START;
    int c = fgetc(fp);
    if (c == EOF) {
        error = ERROR_FILE_IS_EMPTY;
        printFileError(ERROR_FILE_IS_EMPTY, fileName);
    } else {
        ungetc(c, fp);
        firstIteration(fp, &DC, &IC, &error, directivesArr, listOfCommands, listOfSymbols, dataImageHead, codeImageHead,
                       entriesOrExternHead);
        rewind(fp);
        if (!isFileError(error)) {
            FILE *extFile = generateExtFile(fileName);
            FILE *entFile = generateEntFile(fileName);
            if (!extFile || !entFile) {
                error = ERROR_FILE_PERMISSIONS;
                printFileError(ERROR_FILE_PERMISSIONS, "");
            } else {
                secondIteration(listOfSymbols, &error, codeImageHead, IC, entriesOrExternHead, &extFile, &entFile);
                fclose(extFile);
                fclose(entFile);
            }
        }
        if (!isFileError(error)) {
            printOBFile(codeImageHead, dataImageHead, IC, DC, fileName);
        } else {
            deleteEntExtFiles(fileName);
        }
    }
}


