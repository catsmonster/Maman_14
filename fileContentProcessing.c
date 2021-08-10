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
 * iterates over the entriesList and updates hasEntry and hasExtern to determine whether an ent/ext file should
 * be created or not
 */
void determineExistenceOfEntryOrExtern(entriesList *entriesOrExternHead, int *hasEntry, int *hasExtern) {
    entriesList *temp = entriesOrExternHead;
    while (temp && (!*hasEntry || !*hasExtern)) {
        if (isEmpty(temp) == ENTRY)
            *hasEntry = 1;
        if (isEmpty(temp) == EXTERN)
            *hasExtern = 1;
        temp = getNextEntry(temp);
    }
}

/*
 * handles creating the entry/extern files if needed, passing the extracted information from the first iteration to the
 * second iteration to finalize missing data and write into the ext/ent files
 */
void finalizeExtractedInformation(errorCodes *error, char *fileName, dataTable *listOfSymbols, codeNode *codeImageHead,
                                  long IC, entriesList *entriesOrExternHead, int hasExtern, int hasEntry) {
    if (!isFileError(*error)) {
        FILE *extFile = NULL, *entFile = NULL;
        if (hasExtern) {
            extFile = generateExtFile(fileName);
        }
        if (hasEntry) {
            entFile = generateEntFile(fileName);
        }
        if ((hasExtern && !extFile) || (hasEntry && !entFile)) {
            *error = ERROR_FILE_PERMISSIONS;
            printFileError(ERROR_FILE_PERMISSIONS, "");
        } else if (!isFileError(*error)) {
            secondIteration(listOfSymbols, error, codeImageHead, IC, entriesOrExternHead, &extFile, &entFile);
        }
        if (extFile)
            fclose(extFile);
        if (entFile)
            fclose(entFile);
        if (isFileError(*error)) {
            deleteEntExtFiles(fileName, entFile, extFile);
            printf("No output files generated.\n----------------------------------------------\n");
        }
    }
}


/*
 * reading the assembly file and creating the necessary data structures before converting to machine code
 */
void readFileAndGenerateOutput(FILE * fp, CMD *listOfCommands, dataTable *listOfSymbols, dataNode *dataImageHead, codeNode *codeImageHead,
                               entriesList *entriesOrExternHead, char *fileName) {
    errorCodes error = 0;
    long DC = DATA_COUNTER_START, IC = INSTRUCTIONS_COUNTER_START;
    int hasExtern = 0, hasEntry = 0, c = fgetc(fp);
    if (c == EOF) {
        error = ERROR_FILE_IS_EMPTY;
        printFileError(ERROR_FILE_IS_EMPTY, fileName);
    } else {
        ungetc(c, fp);
        printf("Reading from the input file \"%s\":\n", fileName);
        firstIteration(fp, &DC, &IC, &error, listOfCommands, listOfSymbols, dataImageHead, codeImageHead,
                       entriesOrExternHead, &hasExtern, &hasEntry);
        finalizeExtractedInformation(&error, fileName, listOfSymbols, codeImageHead, IC, entriesOrExternHead, hasExtern, hasEntry);
        if (!isFileError(error)) {
            printOBFile(codeImageHead, dataImageHead, IC, DC, fileName);
            printf("Output files for \"%s\" generated.\n----------------------------------------------\n", fileName);
        }
    }
}


