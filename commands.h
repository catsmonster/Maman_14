
#ifndef MAMAN_14_COMMANDS_H
#define MAMAN_14_COMMANDS_H
#include "codeImage.h"
#include "listOfCommands.h"
#include "fileErrors.h"

/*
 * handles reading any type of arithmetic R function
 * accepts the position counter, the input string, the index of the command in the list of the commands,
 * the list of the commands, the code image tail, the instructions counter pointer, error code pointer and the current
 * line counter
 */
void arithmeticRFunctions(int *pos, const char input[], int posInList, CMD *listOfCommands, codeNode **codeImageTail,
                          long *IC, errorCodes *error, long currentLine);

/*
 * handles reading any type of copy R function
 * accepts the position counter, the input string, the index of the command in the list of the commands,
 * the list of the commands, the code image tail, the instructions counter pointer, error code pointer and the current
 * line counter
 */
void copyRFunctions(int *pos, const char input[], int posInList, CMD *listOfCommands, codeNode **codeImageTail, long *IC,
                    errorCodes *error, long currentLine);

/*
 * handles reading any arithmetic or loader I function
 * accepts the position counter, the input string, the index of the command in the list of the commands,
 * the list of the commands, the code image tail, the instructions counter pointer, error code pointer and the current
 * line counter
 */
void arithmeticAndLoaderIFunctions(int *pos, const char *input, int posInList, CMD *listOfCommands, codeNode **codeImageTail,
                                   long *IC, errorCodes *error, long currentLine);

/*
 * handles reading any conditional I function
 * accepts the position counter, the input string, the index of the command in the list of the commands,
 * the list of the commands, the code image tail, the instructions counter pointer, error code pointer and the current
 * line counter
 */
void conditionalIFunctions(int *pos, const char input[], int posInList, CMD *listOfCommands, codeNode **codeImageTail,
                           long *IC, errorCodes *error, long currentLine);


/*
 * handles reading jmp, la or call function
 * accepts the position counter, the input string, the index of the command in the list of the commands,
 * the list of the commands, the code image tail, the instructions counter pointer, error code pointer and the current
 * line counter
 */
void jmpLaCall(int *pos, const char input[], int posInList, CMD *listOfCommands, codeNode **codeImageTail, long *IC,
               errorCodes *error, long currentLine);

/*
 * handles reading stop function
 * accepts the position counter, the input string, the index of the command in the list of the commands,
 * the list of the commands, the code image tail, the instructions counter pointer, error code pointer and the current
 * line counter
 */
void stop(int *pos, const char input[], int posInList, CMD *listOfCommands, codeNode **codeImageTail, long *IC,
          errorCodes *error, long currentLine);



#endif
