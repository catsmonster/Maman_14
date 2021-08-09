
#include "listOfCommands.h"
#include "commands.h"
#include <stdlib.h>
#include <string.h>
#include "fileErrors.h"
#define NUM_OF_CMDS 27

/*
 * This struct holds the commands for easy access to data, and a pointer to the function which reads them
 */
struct commands {
    char *name;
    int funct;
    int opcode;
    void (*function)(int *pos, const char input[], int posInList, CMD *list, codeNode **codeImageTail, long *IC, errorCodes *error,
            long currentLine);
};

/*
 * getCommands returns a pointer to a buffer containing all the different commands and their information ordered alphabetically
 */
CMD *getCommands() {
    int i;
    CMD commandArr[] = {
            {"add", 1, 0,    arithmeticRFunctions},
            {"addi", -1, 10, arithmeticAndLoaderIFunctions},
            {"and", 3, 0,     arithmeticRFunctions},
            {"andi", -1, 12,  arithmeticAndLoaderIFunctions},
            {"beq", -1, 16,   conditionalIFunctions},
            {"bgt", -1, 18,   conditionalIFunctions},
            {"blt", -1, 17,   conditionalIFunctions},
            {"bne", -1, 15,   conditionalIFunctions},
            {"call", -1, 32,  jmpLaCall},
            {"jmp", -1, 30,   jmpLaCall},
            {"la", -1, 31,  jmpLaCall},
            {"lb", -1, 19,    arithmeticAndLoaderIFunctions},
            {"lh", -1, 23,    arithmeticAndLoaderIFunctions},
            {"lw", -1, 21,    arithmeticAndLoaderIFunctions},
            {"move", 1, 1,    copyRFunctions},
            {"mvhi", 2, 1,    copyRFunctions},
            {"mvlo", 3, 1,    copyRFunctions},
            {"nor", 5, 0,    arithmeticRFunctions},
            {"nori", -1, 14,  arithmeticAndLoaderIFunctions},
            {"or", 4, 0,     arithmeticRFunctions},
            {"ori", -1, 13,  arithmeticAndLoaderIFunctions},
            {"sb", -1, 20,  arithmeticAndLoaderIFunctions},
            {"sh", -1, 24,  arithmeticAndLoaderIFunctions},
            {"stop", -1, 63, stop},
            {"sub", 2, 0, arithmeticRFunctions},
            {"subi", -1, 11, arithmeticAndLoaderIFunctions},
            {"sw", -1, 22, arithmeticAndLoaderIFunctions}
    };
    CMD * commandList = (CMD *)malloc(NUM_OF_CMDS * sizeof(CMD));
    if (!commandList) {
        printMemoryError(ERROR_MEMORY_ALLOCATION);
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
 * returns the name string of a command in a given position in the buffer
 */
char *getCMDName(int i, CMD *list) {
    return list[i].name;
}

/*
 * runs the command stored as a pointer to a function in the list of commands array.
 */
void runCMD(int *pos, const char input[], int posInList, CMD *list, codeNode **codeImageTail, long *IC, errorCodes *error,
            long currentLine) {
    list[posInList].function(pos, input, posInList, list, codeImageTail, IC, error, currentLine);
}
