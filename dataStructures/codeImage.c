#include "../includes/codeImage.h"
#include <stdlib.h>
#define FIRST_INSTRUCTION_COUNTER_VALUE 100
#define MAX_SIZE_16_BITS 32767
#define MIN_SIZE_16_BITS (-32768)

/*
 * this struct holds the necessary information to build the code image. it holds the address from which the command was executed
 * a possible label argument, the line from which the command was executed, the type of the command, a pointer to the next node
 * and a union containing each type of command structure, and a structure of 4 bytes used for printing the data in little endian.
 */
struct codeNode {
    long address;
    char *label;
    long currentLine;
    typesOfCommands type;
    codeNode *next;
    union {
        struct {
            long :6;
            unsigned long funct:5;
            unsigned long rd:5;
            unsigned long rt:5;
            unsigned long rs:5;
            unsigned long opcode:6;
        }R;
        struct {
            long immed:16;
            unsigned long rt:5;
            unsigned long rs:5;
            unsigned long opcode:6;
        }I;
        struct {
            long address:25;
            unsigned long reg:1;
            unsigned long opcode:6;
        }J;
        struct {
            unsigned long a:8;
            unsigned long b:8;
            unsigned long c:8;
            unsigned long d:8;
        }representation;
    }data;
};

/*
 * returns the head node of the code image.
 */
codeNode *initializeCodeImage() {
    codeNode *p = calloc(1, sizeof(codeNode));
    return  p;
}



/*
 * updates the address in the immed or address fields of the I or J commands (if applicable).
 */
void adjustMissingAddressToCodeImage(long data, codeNode *codeImageHead, errorCodes *error) {
    if (codeImageHead -> type == I_TYPE) {
        codeImageHead -> data.I.immed = data;
    }
    else if (codeImageHead -> type == I_TYPE_CONDITIONAL) {
        long diff = data - codeImageHead -> address;
        if (diff >= MIN_SIZE_16_BITS && diff <= MAX_SIZE_16_BITS)
            codeImageHead -> data.I.immed = diff;
        else {
            printMemoryError(ERROR_MEMORY_LIMIT);
            *error = ERROR_MEMORY_LIMIT;
        }
    }
    else {
        codeImageHead -> data.J.address = data;
    }
}

/*
 * returns the label argument of the command (or NULL if a label doesn't exist).
 */
char *getCodeNodeLabel(codeNode *node) {
    return node -> label;
}

/*
 * returns the type of the command
 */
typesOfCommands getCodeNodeType(codeNode *node) {
    return node -> type;
}

/*
 * returns the line from which the command was executed
 */
long getCodeNodeLine(codeNode *node) {
    return node -> currentLine;
}

/*
 * returns the address from which the command was executed
 */
long getCodeNodeAddress(codeNode *node) {
    return node -> address;
}


/*
 * returns the next lined code image node
 */
codeNode *getNextCodeNode(codeNode *node) {
    return node -> next;
}

/*
 * returns a byte representation of the code word, according to the part given (1-4)
 */
unsigned char getCodeRepresentation(codeNode *node, typesOfCommands part) {
    unsigned char res;
    switch (part) {
        case FIRST_BYTE:
            res = node -> data.representation.a;
            break;
        case SECOND_BYTE:
            res = node -> data.representation.b;
            break;
        case THIRD_BYTE:
            res = node -> data.representation.c;
            break;
        default:
            res = node -> data.representation.d;
            break;
    }
    return res;
}

/*
 * adds a new empty code node in case this isn't the head of the data image
 */
codeNode *setCodeImageTail(codeNode *codeImageTail, errorCodes *error, long *IC, char *label, typesOfCommands type, long currentLine) {
    int firstInput = *IC == FIRST_INSTRUCTION_COUNTER_VALUE ? 1 : 0;
    if (!firstInput) {
        codeNode *p = calloc(1, sizeof(codeNode));
        if (!p) {
            printMemoryError(ERROR_MEMORY_ALLOCATION);
            *error = ERROR_MEMORY_ALLOCATION;
        }
        else {
            codeImageTail -> next = p;
            codeImageTail = codeImageTail -> next;
        }
    }
    if (label)
        codeImageTail -> label = label;
    codeImageTail -> address = *IC;
    codeImageTail -> type = type;
    codeImageTail -> currentLine = currentLine;
    *IC += 4;
    return codeImageTail;
}


/*
 * inserts a new R word to the code image and returns the new tail
 */
codeNode *insertRWord(errorCodes *error, int opcode, int funct, int rs, int rt, int rd, long *IC, codeNode *codeImageTail,
                      long currentLine) {
    codeNode *newTail = setCodeImageTail(codeImageTail, error, IC, NULL, R_TYPE, currentLine);
    newTail -> data.R.opcode = opcode;
    newTail -> data.R.funct = funct;
    newTail -> data.R.rs = rs;
    newTail -> data.R.rt = rt;
    newTail -> data.R.rd = rd;
    return newTail;
}

/*
 * inserts a new I word to the code image and returns the new tail
 */
codeNode *insertIWord(errorCodes *error, int opcode, int rs, int rt, long immed, long *IC, codeNode *codeImageTail,
                      char *label, typesOfCommands type, long currentLine) {
    codeNode *newTail = setCodeImageTail(codeImageTail, error, IC, label, type, currentLine);
    newTail -> data.I.rs = rs;
    newTail -> data.I.rt = rt;
    newTail -> data.I.immed = immed;
    newTail -> data.I.opcode = opcode;
    return newTail;
}

/*
 * inserts a new J word to the code image and returns the new tail
 */
codeNode *insertJWord(errorCodes *error, int opcode, short reg, long address, long *IC, codeNode *codeImageTail,
                      char *label, typesOfCommands type, long currentLine) {
    codeNode *newTail = setCodeImageTail(codeImageTail, error, IC, label, type, currentLine);
    newTail -> data.J.opcode = opcode;
    newTail -> data.J.reg = reg;
    newTail -> data.J.address = address;
    return newTail;
}

/*
 * frees the code image and any labels which might be linked to the code image node.
 */
void freeCodeImage(codeNode *head) {
    while (head) {
        codeNode *temp = head;
        head = head -> next;
        if (temp -> label)
            free(temp -> label);
        free(temp);
    }
}


