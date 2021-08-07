
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

/*
 * type of data image node
 */
typedef struct dataNode dataNode;
/*
 * returns head of data image
 */
dataNode *initializeDataImage();
/*
 * inserts data to the image based on the type.
 * accepts an error code pointer, the data counter pointer, the data type, the tail of the data image, the number of items to be added
 * and an array of items of type long, containing the data to be added
 */
dataNode *insertLongArrayToDataImage(errorCodes *error, long *DC, typeOfData type, dataNode *dataImageTail, int numberOfItems, long *numList);
/*
 * returns the next linked data image node (or NULL if there is non)
 * accepts a data node.
 */
dataNode *getNextDataNode(dataNode *node);
/*
 * returns a char representing the data in a certain position of the word, in little endian
 * accepts a data image node, the byte to be returned, and a pointer to the print counter (to manage print format)
 */
unsigned char getDataRepresentation(dataNode *node, dataByte part, int *printCounter);
/*
 * returns the data type of the current data image node.
 * accepts a data image node
 */
typeOfData getDataType(dataNode *node);
/*
 * frees the data image, accepts the data image head.
 */
void freeDataImage(dataNode*);
#endif
