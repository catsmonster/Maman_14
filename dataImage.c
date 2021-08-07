
#include "dataImage.h"
#include <stdlib.h>
#define WORD 32


/*
 * this struct holds the information of one cell in the data image
 */
struct dataNode {
    typeOfData type;
    dataNode *next;
    union {
        char db;
        struct {
            long dw:WORD;
        }dw;
        struct {
            int dh:(WORD / 2);
        }dh;
        struct {
            unsigned long a:8;
            unsigned long b:8;
            unsigned long c:8;
            unsigned long d:8;
        }representation;
    }data;
};

/*
 * returns an empty head node
 */
dataNode * initializeDataImage() {
    dataNode *p = calloc(1, sizeof(dataNode));
    if (!p) {
        printMemoryError(ERROR_MEMORY_ALLOCATION);
    }
    return p;
}

/*
 * adds a new empty data node in case this isn't the head of the data image
 */
dataNode *setDataImageTail(dataNode *dataImageTail, int firstInput, errorCodes *error) {
    if (!firstInput) {
        dataNode *p = calloc(1, sizeof(dataNode));
        if (!p) {
            printMemoryError(ERROR_MEMORY_ALLOCATION);
            *error = ERROR_MEMORY_ALLOCATION;
        }
        else {
            dataImageTail -> next = p;
            dataImageTail = dataImageTail -> next;
            dataImageTail -> next = NULL;
        }
    }
    return dataImageTail;
}

/*
 * adds a long to the data image (up to 32 bit - check is performed before the number is received)
 */
void addDW(dataNode *dataImageTail, long num) {
    dataImageTail -> data.dw.dw = num;
    dataImageTail -> type = DW;
}

/*
 * adds an int to the data image (up to 16 bit - check is performed before the number is received)
 */
void addDH(dataNode *dataImageTail, int num) {
    dataImageTail -> data.dh.dh = num;
    dataImageTail -> type = DH;
}

/*
 * adds a char to the data image
 */
void addDBOrAsciz(dataNode *dataImageTail, char num) {
    dataImageTail -> data.db = num;
    dataImageTail -> type = DB_ASCIZ;
}

void advanceDC(typeOfData type, long *DC) {
    if (type == DB_ASCIZ)
        (*DC)++;
    else if (type == DH)
        *DC += SIZE_OF_DH;
    else
        *DC += SIZE_OF_DW;
}

/*
 * populates the data image with data sent from the directive functions based on their type
 */
dataNode *insertLongArrayToDataImage(errorCodes *error, long *DC, typeOfData type, dataNode *dataImageTail, int numberOfItems, long *numList) {
    int i;
    int firstInput = *DC == 0 ? 1 : 0;
    dataNode *newDataTail = setDataImageTail(dataImageTail, firstInput, error);

    for (i = 0; i < numberOfItems; i++) {
        if (type == DB_ASCIZ)
            addDBOrAsciz(newDataTail, (char)numList[i]);
        if (type == DW)
            addDW(newDataTail, numList[i]);
        if (type == DH)
            addDH(newDataTail, (int)numList[i]);
        advanceDC(type, DC);
        if (i < numberOfItems - 1)
            newDataTail = setDataImageTail(newDataTail, 0, error);
    }
    return newDataTail;
}

dataNode *getNextDataNode(dataNode *node) {
    return node -> next;
}


unsigned char getDataRepresentation(dataNode *node, dataByte part, int *printCounter) {
    unsigned char res;
    switch (part) {
        case FIRST_DATA_BYTE:
            res = node -> data.representation.a;
            break;
        case SECOND_DATA_BYTE:
            res = node -> data.representation.b;
            break;
        case THIRD_DATA_BYTE:
            res = node -> data.representation.c;
            break;
        default:
            res = node -> data.representation.d;
            break;
    }
    (*printCounter)++;
    return res;
}

typeOfData getDataType(dataNode *node) {
    return node -> type;
}


void freeDataImage(dataNode *head) {
    while (head) {
        dataNode *temp = head;
        head = head -> next;
        free(temp);
    }
}
