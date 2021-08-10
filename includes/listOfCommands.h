
#ifndef MAMAN_14_LISTOFCOMMANDS_H
#define MAMAN_14_LISTOFCOMMANDS_H
#include "codeImage.h"
typedef struct commands CMD;

/*
 * returns a list of the assembly commands, containing opcode, funct, the name of the command and a pointer to a function
 * to handle the specific command type.
 */
CMD * getCommands();

/*
 * returns the position in the list of commands of the specified command name, -1 if the command doesn't exist.
 */
int findCommand(char *, CMD *);
/*
 * returns the opcode of a command in a given position of the list.
 */
int getOpcode(int, CMD*);
/*
 * returns the funct of a command in a given position of the list.
 */
int getFunct(int, CMD*);
/*
 * returns the name of the command in the given position of the list.
 */
char *getCMDName(int, CMD*);
/*
 * runs the function linked to the command in the given position of the list.
 */
void runCMD(int *pos, const char input[], int posInList, CMD *list, codeNode **codeImageTail, long *IC, errorCodes *error,
            long currentLine);

#endif
