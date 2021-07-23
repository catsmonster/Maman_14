
#ifndef MAMAN_14_LISTOFCOMMANDS_H
#define MAMAN_14_LISTOFCOMMANDS_H
#include "codeImage.h"
typedef struct commands CMD;

CMD * getCommands();

int findCommand(char *, CMD *);
int getOpcode(int, CMD*);
int getFunct(int, CMD*);
char getType(int, CMD*);
char *getCMDName(int, CMD*);
void runCMD(int *pos, const char input[], int posInList, CMD *list, codeNode **codeImageTail, long *IC, int *error,
            long currentLine);

#endif
