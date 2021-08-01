#include "codeImage.h"
#include <stdlib.h>
#define FIRST_INSTRUCTION_COUNTER_VALUE 100

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


codeNode *initializeCodeImage() {
    codeNode *p = calloc(1, sizeof(codeNode));
    if (!p) {
        printMemoryError(ERROR_MEMORY_ALLOCATION);
    }
    return  p;
}




void adjustMissingAddressToCodeImage(long data, codeNode *codeImageHead) {
    if (codeImageHead -> type == I_TYPE || codeImageHead -> type == I_TYPE_CONDITIONAL) {
        codeImageHead -> data.I.immed = data - codeImageHead -> data.I.immed;
    }
    else {
        codeImageHead -> data.J.address = data - codeImageHead -> data.J.address;
    }
}

char *getCodeNodeLabel(codeNode *node) {
    return node -> label;
}

typesOfCommands getCodeNodeType(codeNode *node) {
    return node -> type;
}

long getCodeNodeLine(codeNode *node) {
    return node -> currentLine;
}

long getCodeNodeAddress(codeNode *node) {
    return node -> address;
}

codeNode *getNextCodeNode(codeNode *node) {
    return node -> next;
}

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

codeNode *insertIWord(errorCodes *error, int opcode, int rs, int rt, long immed, long *IC, codeNode *codeImageTail,
                      char *label, typesOfCommands type, long currentLine) {
    codeNode *newTail = setCodeImageTail(codeImageTail, error, IC, label, type, currentLine);
    newTail -> data.I.rs = rs;
    newTail -> data.I.rt = rt;
    newTail -> data.I.immed = immed;
    newTail -> data.I.opcode = opcode;
    return newTail;
}

codeNode *insertJWord(errorCodes *error, int opcode, short reg, long address, long *IC, codeNode *codeImageTail,
                      char *label, typesOfCommands type, long currentLine) {
    codeNode *newTail = setCodeImageTail(codeImageTail, error, IC, label, type, currentLine);
    newTail -> data.J.opcode = opcode;
    newTail -> data.J.reg = reg;
    newTail -> data.J.address = address;
    return newTail;
}

void freeCodeImage(codeNode *head) {
    while (head) {
        codeNode *temp = head;
        head = head -> next;
        if (temp -> label)
            free(temp -> label);
        free(temp);
    }
}


