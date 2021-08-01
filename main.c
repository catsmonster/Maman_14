#include "fileNameProcessing.h"
#include "fileErrors.h"

/*
 * main calls the functions which handle the assembly files.
 */
int main(int argc, char * argv[]) {
    int error = inputFileHandler(argc, argv);
    if (isFileError(error))
        printFileError(error, "");
    return error;
}
