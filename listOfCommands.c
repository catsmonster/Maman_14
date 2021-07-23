
#include "listOfCommands.h"
#include "commands.h"
#include <stdlib.h>
#include <string.h>
#include "fileErrors.h"
#define NUM_OF_CMDS 27

/*
 * This struct holds the commands for easy access to data
 */
struct commands {
    char *name;
    int funct;
    int opcode;
    char type;
    void (*function)(int *pos, const char input[], int posInList, CMD *list, codeNode **codeImageTail, long *IC, int *error,
            long currentLine);
};

/*
 * getCommands returns a pointer to a buffer containing all the different commands
 */
CMD *getCommands() {
    int i;
    CMD commandArr[] = {
            {"add", 1, 0, 'R',    arithmeticRFunctions},
            {"addi", -1, 10, 'I', arithmeticAndLoaderIFunctions},
            {"and", 3, 0, 'R',    arithmeticRFunctions},
            {"andi", -1, 12, 'I', arithmeticAndLoaderIFunctions},
            {"beq", -1, 16, 'I',  conditionalIFunctions},
            {"bgt", -1, 18, 'I',  conditionalIFunctions},
            {"blt", -1, 17, 'I',  conditionalIFunctions},
            {"bne", -1, 15, 'I',  conditionalIFunctions},
            {"call", -1, 32, 'J', jmpLaCall},
            {"jmp", -1, 30, 'J',  jmpLaCall},
            {"la", -1, 31, 'J', jmpLaCall},
            {"lb", -1, 19, 'I',   arithmeticAndLoaderIFunctions},
            {"lh", -1, 23, 'I',   arithmeticAndLoaderIFunctions},
            {"lw", -1, 21, 'I',   arithmeticAndLoaderIFunctions},
            {"move", 1, 1, 'R',   copyRFunctions},
            {"mvhi", 2, 1, 'R',   copyRFunctions},
            {"mvlo", 3, 1, 'R',   copyRFunctions},
            {"nor", 5, 0, 'R',    arithmeticRFunctions},
            {"nori", -1, 14, 'I', arithmeticAndLoaderIFunctions},
            {"or", 4, 0, 'R',     arithmeticRFunctions},
            {"ori", -1, 13, 'I',  arithmeticAndLoaderIFunctions},
            {"sb", -1, 20, 'I',   arithmeticAndLoaderIFunctions},
            {"sh", -1, 24, 'I',   arithmeticAndLoaderIFunctions},
            {"stop", -1, 63, 'J', stop},
            {"sub", 2, 0, 'R',    arithmeticRFunctions},
            {"subi", -1, 11, 'I', arithmeticAndLoaderIFunctions},
            {"sw", -1, 22, 'I',   arithmeticAndLoaderIFunctions}
    };
    CMD * commandList = (CMD *)malloc(NUM_OF_CMDS * sizeof(CMD));
    if (!commandList) {
        printError(ERROR_MEMORY_ALLOCATION, ERROR_TYPE_MEMORY, 0);
    }
    else {
        for (i = 0; i < NUM_OF_CMDS; i++) {
            commandList[i] = commandArr[i];
        }
    }
    return commandList;
}

/*
 * finds a command by name string and returns its position in the buffer of structs
 */
int findCommand(char *input, CMD * list) {
    int found = -1, high = NUM_OF_CMDS - 1, low = 0, mid, res;
    while (found == -1 && high >= low) {
        mid = (high + low) / 2;
        res = strcmp(input, list[mid].name);
        if (!res)
            found = mid;
        else if (res < 0)
            high = mid - 1;
        else
            low = mid + 1;
    }
    return found;
}

/*
 * returns the opcode of a command in a given position in the buffer
 */
int getOpcode(int i, CMD *list) {
    return list[i].opcode;
}

/*
 * returns the funct of a command in a given position in the buffer
 */
int getFunct(int i, CMD *list) {
    return list[i].funct;
}

/*
 * returns the type of a command in a given position in the buffer
 */
char getType(int i, CMD *list) {
    return list[i].type;
}

/*
 * returns the name string of a command in a given position in the buffer
 */
char *getCMDName(int i, CMD *list) {
    return list[i].name;
}

void runCMD(int *pos, const char input[], int posInList, CMD *list, codeNode **codeImageTail, long *IC, int *error,
            long currentLine) {
    list[posInList].function(pos, input, posInList, list, codeImageTail, IC, error, currentLine);
}
