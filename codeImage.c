#include "codeImage.h"
#include "fileErrors.h"
#include <stdlib.h>
#define FIRST_INSTRUCTION_COUNTER_VALUE 100

struct codeNode {
    long address;
    char *label;
    long currentLine;
    int type;
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
        printError(ERROR_MEMORY_ALLOCATION, ERROR_TYPE_MEMORY, 0);
    }
    return  p;
}




void setMissingData(long data, codeNode *codeImageHead) {
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

int getCodeNodeType(codeNode *node) {
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

unsigned char getCodeRepresentation(codeNode *node, int part) {
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
codeNode *setCodeImageTail(codeNode *codeImageTail, int *error, long *IC, char *label, int type, long currentLine) {
    int firstInput = *IC == FIRST_INSTRUCTION_COUNTER_VALUE ? 1 : 0;
    if (!firstInput) {
        codeNode *p = calloc(1, sizeof(codeNode));
        if (!p) {
            printError(ERROR_MEMORY_ALLOCATION, ERROR_TYPE_MEMORY, 0);
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



codeNode *insertRWord(int *error, int opcode, int funct, int rs, int rt, int rd, long *IC, codeNode *codeImageTail,
                      long currentLine) {
    codeImageTail = setCodeImageTail(codeImageTail, error, IC, NULL, R_TYPE, currentLine);
    codeImageTail -> data.R.opcode = opcode;
    codeImageTail -> data.R.funct = funct;
    codeImageTail -> data.R.rs = rs;
    codeImageTail -> data.R.rt = rt;
    codeImageTail -> data.R.rd = rd;
    return codeImageTail;
}

codeNode *insertIWord(int *error, int opcode, int rs, int rt, long immed, long *IC, codeNode *codeImageTail,
                      char *label, int type, long currentLine) {
    codeImageTail = setCodeImageTail(codeImageTail, error, IC, label, type, currentLine);
    codeImageTail -> data.I.rs = rs;
    codeImageTail -> data.I.rt = rt;
    codeImageTail -> data.I.immed = immed;
    codeImageTail -> data.I.opcode = opcode;
    return codeImageTail;
}

codeNode *insertJWord(int *error, int opcode, short reg, long address, long *IC, codeNode *codeImageTail,
                      char *label, int type, long currentLine) {
    codeImageTail = setCodeImageTail(codeImageTail, error, IC, label, type, currentLine);
    codeImageTail -> data.J.opcode = opcode;
    codeImageTail -> data.J.reg = reg;
    codeImageTail -> data.J.address = address;
    return codeImageTail;
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


