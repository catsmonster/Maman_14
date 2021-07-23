#include <stdio.h>
#include "firstIteration.h"
#include "directiveFunctions.h"
#include "fileContentProcessing.h"
#include "secondIteration.h"
#include "fileErrors.h"
#include "printOutput.h"

/*
 * reading the assembly file and creating the necessary data structures before converting to machine code
 */
void readFile(FILE * fp, void (*directivesArr[])(int *, const char[], long *, int*, dataNode**, long),
              CMD *listOfCommands, dataTable *listOfSymbols, dataNode *dataImageHead, codeNode *codeImageHead,
              entriesOrExtern *entriesOrExternHead, char *fileName) {

    int error = 0;
    long DC = 0, IC = 100;
    firstIteration(fp, &DC, &IC, &error, directivesArr, listOfCommands, listOfSymbols, dataImageHead, codeImageHead,
                   entriesOrExternHead);
    rewind(fp);
    if (!isFileError(error)) {
        FILE *extFile = generateExtFile(fileName);
        FILE *entFile = generateEntFile(fileName);
        if (!extFile || !entFile) {
                error = ERROR_FILE_PERMISSIONS;
                printError(ERROR_FILE_PERMISSIONS, ERROR_TYPE_FILE, 0);
        }
        else {
            secondIteration(listOfSymbols, &error, codeImageHead, IC, entriesOrExternHead, &extFile, &entFile);
            fclose(extFile);
            fclose(entFile);
        }
    }
    if (!isFileError(error)) {
        printOBFile(codeImageHead, dataImageHead, IC, DC, fileName);
    }
    else {
        deleteEntExtFiles(fileName);
    }
}


