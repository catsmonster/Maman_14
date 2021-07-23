
#include "entriesOrExternList.h"
#include <stdlib.h>
#include "fileErrors.h"
#define FIRST_NODE 666

struct entriesOrExternNode {
    char *name;
    int type;
    long currentLine;
    long address;
    entriesOrExtern *next;
};

entriesOrExtern *initializeEntriesOrExtern() {
    entriesOrExtern *p = calloc(1, sizeof(entriesOrExtern));
    if (!p) {
        printError(ERROR_MEMORY_ALLOCATION, ERROR_TYPE_MEMORY, 0);
    }
    else {
        p -> type = FIRST_NODE;
    }
    return p;
}

/*
 * adds a new empty code node in case this isn't the head of the data image
 */
entriesOrExtern *setNodeTail(entriesOrExtern *tailNode, int *error) {
    int firstInput = tailNode -> type == FIRST_NODE ? 1 : 0;
    if (!firstInput) {
        entriesOrExtern *p = calloc(1, sizeof(entriesOrExtern));
        if (!p) {
            printError(ERROR_MEMORY_ALLOCATION, ERROR_TYPE_MEMORY, 0);
            *error = ERROR_MEMORY_ALLOCATION;
        }
        else {
            tailNode -> next = p;
            tailNode = tailNode -> next;
        }
    }

    return tailNode;
}

entriesOrExtern *setEntryOrExternTail(char *name, int type, entriesOrExtern *tail, int *error, long currentLine) {
    tail = setNodeTail(tail, error);
    tail -> type = type;
    tail -> name = name;
    tail -> currentLine = currentLine;
    return tail;
}

char *getEntryOrExternName(entriesOrExtern *node) {
    return node -> name;
}

int getEntryOrExternType(entriesOrExtern *node) {
    return node -> type;
}

entriesOrExtern *getNextEntryOrExtern(entriesOrExtern *node) {
    return node -> next;
}

long getEntryOrExternLine(entriesOrExtern *node) {
    return node -> currentLine;
}

entriesOrExtern *addExternToHead(entriesOrExtern *head, long address, char *name, int type, long currentLine) {
    entriesOrExtern *temp = initializeEntriesOrExtern();
    temp -> next = head;
    temp -> type = type;
    temp -> name = name;
    temp -> address = address;
    temp -> currentLine = currentLine;
    return temp;
}



void freeEntriesOrExternList(entriesOrExtern *head) {
    while (head) {
        entriesOrExtern *temp = head;
        head = head -> next;
        free(temp -> name);
        free(temp);
    }
}
