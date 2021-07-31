
#ifndef MAMAN_14_COMMANDS_H
#define MAMAN_14_COMMANDS_H
#include "codeImage.h"
#include "listOfCommands.h"
#include "fileErrors.h"

void arithmeticRFunctions(int *pos, const char input[], int posInList, CMD *listOfCommands, codeNode **codeImageTail,
                          long *IC, errorCodes *error, long currentLine);

void copyRFunctions(int *pos, const char input[], int posInList, CMD *listOfCommands, codeNode **codeImageTail, long *IC,
                    errorCodes *error, long currentLine);

void arithmeticAndLoaderIFunctions(int *pos, const char *input, int posInList, CMD *listOfCommands, codeNode **codeImageTail,
                                   long *IC, errorCodes *error, long currentLine);

void conditionalIFunctions(int *pos, const char input[], int posInList, CMD *listOfCommands, codeNode **codeImageTail,
                           long *IC, errorCodes *error, long currentLine);


void jmpLaCall(int *pos, const char input[], int posInList, CMD *listOfCommands, codeNode **codeImageTail, long *IC,
               errorCodes *error, long currentLine);


void stop(int *pos, const char input[], int posInList, CMD *listOfCommands, codeNode **codeImageTail, long *IC,
          errorCodes *error, long currentLine);



#endif
