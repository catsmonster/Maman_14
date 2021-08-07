
#ifndef MAMAN_14_DATAIMAGE_H
#define MAMAN_14_DATAIMAGE_H
#define SIZE_OF_DH 2
#define SIZE_OF_DW 4
#define SIZE_OF_DB 1
#include "fileErrors.h"
/*
 * indicator for the requested data byte
 */
typedef enum {
    FIRST_DATA_BYTE,
    SECOND_DATA_BYTE,
    THIRD_DATA_BYTE
}dataByte;

/*
 * indicator for the type of data saved in the node
 */
typedef enum {
    DB_ASCIZ,
    DW,
    DH
}typeOfData;

typedef struct dataNode dataNode;
dataNode *initializeDataImage();
dataNode *insertLongArrayToDataImage(errorCodes *error, long *DC, typeOfData type, dataNode *dataImageTail, int numberOfItems, long *numList);
dataNode *getNextDataNode(dataNode *node);
unsigned char getDataRepresentation(dataNode *node, dataByte part, int *printCounter);
typeOfData getDataType(dataNode *node);
void freeDataImage(dataNode*);
#endif
