
#include "entriesOrExternList.h"
#include <stdlib.h>

struct entriesOrExternNode {
    char *name;
    symbolTypes type;
    long currentLine;
    entriesOrExternList *next;
};

entriesOrExternList *initializeEntriesOrExternList() {
    entriesOrExternList *p = calloc(1, sizeof(entriesOrExternList));
    if (!p) {
        printMemoryError(ERROR_MEMORY_ALLOCATION);
    }
    else {
        p -> type = FIRST_NODE;
    }
    return p;
}

/*
 * adds a new empty code node in case this isn't the head of the data image
 */
entriesOrExternList *setNodeTail(entriesOrExternList *tailNode, errorCodes *error) {
    int firstInput = tailNode -> type == FIRST_NODE ? 1 : 0;
    if (!firstInput) {
        entriesOrExternList *p = calloc(1, sizeof(entriesOrExternList));
        if (!p) {
            printMemoryError(ERROR_MEMORY_ALLOCATION);
            *error = ERROR_MEMORY_ALLOCATION;
        }
        else {
            tailNode -> next = p;
            tailNode = tailNode -> next;
        }
    }

    return tailNode;
}

entriesOrExternList *setEntryOrExternListTail(char *name, symbolTypes type, entriesOrExternList *tail, errorCodes *error, long currentLine) {
    tail = setNodeTail(tail, error);
    tail -> type = type;
    tail -> name = name;
    tail -> currentLine = currentLine;
    return tail;
}

char *getEntryOrExternName(entriesOrExternList *node) {
    return node -> name;
}

symbolTypes getEntryOrExternType(entriesOrExternList *node) {
    return node -> type;
}

entriesOrExternList *getNextEntryOrExtern(entriesOrExternList *node) {
    return node -> next;
}

long getEntryOrExternLine(entriesOrExternList *node) {
    return node -> currentLine;
}




void freeEntriesOrExternList(entriesOrExternList *head) {
    while (head) {
        entriesOrExternList *temp = head;
        head = head -> next;
        free(temp -> name);
        free(temp);
    }
}
