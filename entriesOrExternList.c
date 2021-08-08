
#include "entriesOrExternList.h"
#include <stdlib.h>

/*
 * struct for a linked list containing the names of the entries or external symbols, their type, and the line in which
 * they were encountered.
 */
struct entriesOrExternNode {
    char *name;
    symbolTypes type;
    long currentLine;
    entriesOrExternList *next;
};

/*
 * returns an entriesOrExtern list head
 */
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

/*
 * adds a new node to the entriesOrExtern list tail, and returns the new tail
 */
entriesOrExternList *setEntryOrExternListTail(char *name, symbolTypes type, entriesOrExternList *tail, errorCodes *error, long currentLine) {
    tail = setNodeTail(tail, error);
    tail -> type = type;
    tail -> name = name;
    tail -> currentLine = currentLine;
    return tail;
}

/*
 * returns the name of the entry/extern label of the node
 */
char *getEntryOrExternName(entriesOrExternList *node) {
    return node -> name;
}

/*
 * returns the type of the symbol of the label of the node
 */
symbolTypes getEntryOrExternType(entriesOrExternList *node) {
    return node -> type;
}

/*
 * returns the next linked node to the current node
 */
entriesOrExternList *getNextEntryOrExtern(entriesOrExternList *node) {
    return node -> next;
}

/*
 * returns the line in which the label was added to the list
 */
long getEntryOrExternLine(entriesOrExternList *node) {
    return node -> currentLine;
}



/*
 * frees the list of entry and extern labels.
 */
void freeEntriesOrExternList(entriesOrExternList *head) {
    while (head) {
        entriesOrExternList *temp = head;
        head = head -> next;
        free(temp);
    }
}
