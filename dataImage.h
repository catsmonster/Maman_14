
#ifndef MAMAN_14_DATAIMAGE_H
#define MAMAN_14_DATAIMAGE_H
#define SIZE_OF_DH 2
#define SIZE_OF_DW 4
#define SIZE_OF_DB 1
enum {
    FIRST_DATA_BYTE,
    SECOND_DATA_BYTE,
    THIRD_DATA_BYTE,
    FORTH_DATA_BYTE
};

enum {
    DB_ASCIZ,
    DW,
    DH
};

typedef struct dataNode dataNode;
dataNode *initializeDataImage();
dataNode *insertLongArrayToDataImage(int *error, long *DC, int type, dataNode *dataImageTail, int numberOfItems, long *numList);
dataNode *getNextDataNode(dataNode *node);
unsigned char getDataRepresentation(dataNode *node, int part, int *printCounter);
int getDataType(dataNode *node);
void freeDataImage(dataNode*);
#endif
