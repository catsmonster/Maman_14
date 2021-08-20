#include <stdio.h>
#include "../includes/firstIteration.h"
#include "../includes/fileContentProcessing.h"
#include "../includes/secondIteration.h"
#define INSTRUCTIONS_COUNTER_START 100
#define DATA_COUNTER_START 0

/*
 * reading the assembly file and creating the necessary data structures before converting to machine code
 */
void readFileAndGenerateOutput(FILE * fp, CMD *listOfCommands, dataTable *listOfSymbols, dataNode *dataImageHead, codeNode *codeImageHead,
                               entriesList *entriesHead, char *fileName) {
    errorCodes error = 0;
    long DC = DATA_COUNTER_START, IC = INSTRUCTIONS_COUNTER_START;
    int c = fgetc(fp);
    if (c == EOF) {
        error = ERROR_FILE_IS_EMPTY;
        printFileError(ERROR_FILE_IS_EMPTY, fileName);
    } else {
        ungetc(c, fp);
        printf("Reading from the input file \"%s\":\n", fileName);
        firstIteration(fp, &DC, &IC, &error, listOfCommands, listOfSymbols, dataImageHead, codeImageHead,
                       entriesHead);
        secondIteration(listOfSymbols, &error, codeImageHead, IC, entriesHead, fileName);
        if (!isFileError(error)) {
            printOBFile(codeImageHead, dataImageHead, IC, DC, fileName);
            printf("Output files for \"%s\" generated.\n----------------------------------------------\n", fileName);
        }
        else {
            printf("No output files generated.\n----------------------------------------------\n");
        }
    }
}


