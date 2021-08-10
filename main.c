#include "includes/fileNameProcessing.h"
#include "includes/fileErrors.h"

/*
 * main calls the functions which handle the assembly files.
 */
int main(int argc, char * argv[]) {
    int error = inputFilesIteration(argc, argv);
    if (isFileError(error))
        printFileError(error, "");
    return error;
}
