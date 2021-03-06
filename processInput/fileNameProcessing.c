#include "../includes/fileNameProcessing.h"
#include "../includes/fileErrors.h"
#include "../includes/fileContentProcessing.h"
#include <string.h>
#include <stdlib.h>


/*
 * checks for a valid .as file extension
 */
int isValidExtension(char fileName[]) {
    int isError = 0;
    unsigned int i = strlen(fileName) - 1, j = 1;
    char extension[] = "as";
    for (; !isError && fileName[i] != '.'; i--, j--) {
        if (i == 0 || j < 0 || fileName[i] != extension[j])
            isError = ERROR_INVALID_FILE_EXTENSION;
    }
    return isError;
}

/*
 * attempts to read the file in r mode to start the reading process
 */
int openFile(char * fileName, CMD *listOfCommands) {
    dataTable *listOfSymbols = initializeDataTable();
    dataNode *dataImageHead = initializeDataImage();
    codeNode *codeImageHead = initializeCodeImage();
    entriesList *entriesHead = initializeEntriesList();
    int errorCode = 0;
    if (!listOfSymbols || !dataImageHead || !codeImageHead || !entriesHead) {
        printMemoryError(ERROR_MEMORY_ALLOCATION);
        errorCode = ERROR_MEMORY_ALLOCATION;
    }
    else {
        FILE *fp = fopen(fileName, "r");
        if (!fp)
            errorCode = ERROR_FILE_DOES_NOT_EXIST;
        if (!errorCode) {
            readFileAndGenerateOutput(fp, listOfCommands, listOfSymbols, dataImageHead, codeImageHead,
                                      entriesHead, fileName);
            fclose(fp);
        }
        freeDataTable(listOfSymbols);
        freeCodeImage(codeImageHead);
        freeDataImage(dataImageHead);
        freeEntriesList(entriesHead);
    }
    return errorCode;
}


/*
 * orchestrates the file reading process. Attempts to read all the files given by the user, giving out file related
 * errors if necessary.
 */
int inputFilesIteration(int argc, char **argv) {
    int isError = 0, i;
    CMD *listOfCommands = initializeCommandsList();
    if (!listOfCommands) {
        printMemoryError(ERROR_MEMORY_ALLOCATION);
    }
    else
    {
        if (argc < 2)
            isError = ERROR_MISSING_AS_FILE;
        if (!isError) {
            for (i = 1; i < argc; i++) {
                int error = isValidExtension(argv[i]);
                if (isFileError(error)) {
                    printFileError(error, argv[i]);
                } else {
                    error = openFile(argv[i], listOfCommands);
                    if (isFileError(error))
                        printFileError(error, argv[i]);
                }
            }
        }
        free(listOfCommands);
    }
    return isError;
}
