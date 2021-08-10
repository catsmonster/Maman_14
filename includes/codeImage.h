#ifndef MAMAN_14_CODEIMAGE_H
#define MAMAN_14_CODEIMAGE_H
#include "fileErrors.h"

typedef enum {
    I_TYPE = 600,
    I_TYPE_CONDITIONAL,
    J_TYPE,
    R_TYPE,
    FIRST_BYTE,
    SECOND_BYTE,
    THIRD_BYTE,
    FORTH_BYTE
}typesOfCommands;

typedef struct codeNode codeNode;

/*
 * returns the head of a linked list which will hold the code image, each node will represent a 32 bit long command word,
 * and any necessary data gathered from the line of assembly code.
 */
codeNode *initializeCodeImage();
/*
 * adds a new R word to the list, returns a new tail of the list.
 * accepts an error code pointer, opcode, funct, rs register, rt register, rd register, instructions counter pointer,
 * code image tail.
 */
codeNode *insertRWord(errorCodes *error, int opcode, int funct, int rs, int rt, int rd, long *IC, codeNode *codeImageTail,
                      long line);
/*
 * adds a new I word to the list, returns a new tail of the list.
 * accepts an error code pointer, opcode, rs register, rt register, immed value, instructions counter pointer,
 * code image tail.
 */
codeNode *insertIWord(errorCodes *error, int opcode, int rs, int rt, long immed, long *IC, codeNode *codeImageTail,
                      char *label, typesOfCommands type, long line);
/*
 * adds a new J word to the list, returns a new tail of the list.
 * accepts an error code pointer, opcode, reg flag, address value, instructions counter pointer, code image tail,
 * label argument, an indicator for the type of the command and the current line counter.
 */
codeNode *insertJWord(errorCodes *error, int opcode, short reg, long address, long *IC, codeNode *codeImageTail,
                      char *label, typesOfCommands type, long line);
/*
 * after all the necessary data was gathered, adjusts the address in the nodes which didn't have complete data
 * after the first iteration.
 * accepts an address from the symbols table and the code image.
 */
void adjustMissingAddressToCodeImage(long data, codeNode *codeImageHead);
/*
 * returns the address linked to the given command node
 * accepts a code image node.
 */
long getCodeNodeAddress(codeNode *node);
/*
 * returns the label which was given as a parameter to the command in the given node. If no such parameter exists,
 * returns NULL
 * accepts a code image node
 */
char *getCodeNodeLabel(codeNode *node);
/*
 * returns the line of the assembly file in which the command in the given node was invoked.
 * accepts a code image node
 */
long getCodeNodeLine(codeNode *node);
/*
 * returns the type of the command in the given node.
 * accepts a code image node.
 */
typesOfCommands getCodeNodeType(codeNode *node);
/*
 * returns a single 8 bit number representing 1 byte out of the 4 bytes in the word, according to the part specified.
 * accepts a code image node, and an indicator of which byte to represent.
 */
unsigned char getCodeRepresentation(codeNode *node, typesOfCommands part);
/*
 * returns the next linked code node.
 * accepts an image code node
 */
codeNode *getNextCodeNode(codeNode *node);
/*
 * iterates over the nodes of the linked list and frees them. Also frees any labels stored as parameters.
 * accepts the code image.
 */
void freeCodeImage(codeNode*);
#endif
