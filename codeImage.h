#ifndef MAMAN_14_CODEIMAGE_H
#define MAMAN_14_CODEIMAGE_H

enum {
    I_TYPE = 600,
    I_TYPE_CONDITIONAL,
    J_TYPE,
    R_TYPE,
    FIRST_BYTE,
    SECOND_BYTE,
    THIRD_BYTE,
    FORTH_BYTE
};

typedef struct codeNode codeNode;
typedef struct incompleteCodeNode incompleteCodeNode;

codeNode *initializeCodeImage();
codeNode *insertRWord(int *error, int opcode, int funct, int rs, int rt, int rd, long *IC, codeNode *codeImageTail, long line);
codeNode *insertIWord(int *error, int opcode, int rs, int rt, long immed, long *IC, codeNode *codeImageTail, char *label, int type, long line);
codeNode *insertJWord(int *error, int opcode, short reg, long address, long *IC, codeNode *codeImageTail, char *label, int type, long line);
void setMissingData(long data, codeNode *codeImageHead);
long getCodeNodeAddress(codeNode *node);
char *getCodeNodeLabel(codeNode *node);
long getCodeNodeLine(codeNode *node);
int getCodeNodeType(codeNode *node);
unsigned char getCodeRepresentation(codeNode *node, int part);
codeNode *getNextCodeNode(codeNode *node);
void freeCodeImage(codeNode*);
#endif
