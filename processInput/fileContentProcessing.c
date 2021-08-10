#include <stdio.h>
#include "../includes/firstIteration.h"
#include "../includes/fileContentProcessing.h"
#include "../includes/secondIteration.h"
#define INSTRUCTIONS_COUNTER_START 100
#define DATA_COUNTER_START 0

/*
 * close entry or extern files. In case an error was encountered, deletes any output file, if created
 */
void closeEntExtFiles(errorCodes *error, FILE *entFile, FILE *extFile, char *fileName) {
    if (extFile)
        fclose(extFile);
    if (entFile)
        fclose(entFile);
    if (isFileError(*error)) {
        deleteEntExtFiles(fileName, entFile, extFile);
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
            extFile = generateExtOrEntFile(fileName, EXT_FILE);
        }
        if (hasEntry) {
            entFile = generateExtOrEntFile(fileName, ENT_FILE);
        }
        if ((hasExtern && !extFile) || (hasEntry && !entFile)) {
            *error = ERROR_FILE_PERMISSIONS;
            printFileError(ERROR_FILE_PERMISSIONS, "");
        } else if (!isFileError(*error)) {
            secondIteration(listOfSymbols, error, codeImageHead, IC, entriesOrExternHead, &extFile, &entFile);
        }
        closeEntExtFiles(error, entFile, extFile, fileName);
    }
}


/*
 * reading the assembly file and creating the necessary data structures before converting to machine code
 */
void readFileAndGenerateOutput(FILE * fp, CMD *listOfCommands, dataTable *listOfSymbols, dataNode *dataImageHead, codeNode *codeImageHead,
                               entriesList *entriesHead, char *fileName) {
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
                       entriesHead, &hasExtern, &hasEntry);
        finalizeExtractedInformation(&error, fileName, listOfSymbols, codeImageHead, IC, entriesHead, hasExtern, hasEntry);
        if (!isFileError(error)) {
            printOBFile(codeImageHead, dataImageHead, IC, DC, fileName);
            printf("Output files for \"%s\" generated.\n----------------------------------------------\n", fileName);
        }
        else {
            printf("No output files generated.\n----------------------------------------------\n");
        }
    }
}


