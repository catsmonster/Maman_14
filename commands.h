
#ifndef MAMAN_14_COMMANDS_H
#define MAMAN_14_COMMANDS_H
#include "codeImage.h"
#include "listOfCommands.h"
#include "fileErrors.h"

/*
 * handles reading any type of arithmetic R function
 */
void arithmeticRFunctions(int *pos, const char input[], int posInList, CMD *listOfCommands, codeNode **codeImageTail,
                          long *IC, errorCodes *error, long currentLine);

/*
 * handles reading any type of copy R function
 */
void copyRFunctions(int *pos, const char input[], int posInList, CMD *listOfCommands, codeNode **codeImageTail, long *IC,
                    errorCodes *error, long currentLine);

/*
 * handles reading any arithmetic or loader I function
 */
void arithmeticAndLoaderIFunctions(int *pos, const char *input, int posInList, CMD *listOfCommands, codeNode **codeImageTail,
                                   long *IC, errorCodes *error, long currentLine);

/*
 * handles reading any conditional I function
 */
void conditionalIFunctions(int *pos, const char input[], int posInList, CMD *listOfCommands, codeNode **codeImageTail,
                           long *IC, errorCodes *error, long currentLine);


/*
 * handles reading jmp, la or call function
 */
void jmpLaCall(int *pos, const char input[], int posInList, CMD *listOfCommands, codeNode **codeImageTail, long *IC,
               errorCodes *error, long currentLine);

/*
 * handles reading stop function
 */
void stop(int *pos, const char input[], int posInList, CMD *listOfCommands, codeNode **codeImageTail, long *IC,
          errorCodes *error, long currentLine);



#endif
