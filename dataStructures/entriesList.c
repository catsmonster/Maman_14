
#include "../includes/entriesList.h"
#include <stdlib.h>
#include <string.h>

/*
 * struct for a linked list containing the names of the entries or external symbols, their type, and the line in which
 * they were encountered.
 */
struct entriesNode {
    char *name;
    long currentLine;
    entriesList *next;
};

/*
 * returns an entries list head
 */
entriesList *initializeEntriesList() {
    entriesList *p = calloc(1, sizeof(entriesList));
    if (!p) {
        printMemoryError(ERROR_MEMORY_ALLOCATION);
    }
    return p;
}

/*
 * adds a new empty code node in case this isn't the head of the data image
 */
entriesList *setNodeTail(entriesList *tailNode, errorCodes *error) {
    int firstInput = tailNode -> name ? 0 : 1;
    if (!firstInput) {
        entriesList *p = calloc(1, sizeof(entriesList));
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
 * adds a new node to the entries list tail, and returns the new tail
 */
entriesList *setEntryListTail(char *name, entriesList *tail, errorCodes *error, long currentLine) {
    tail = setNodeTail(tail, error);
    tail -> name = name;
    tail -> currentLine = currentLine;
    return tail;
}

/*
 * returns 1 if an entry with the same name already exists in the list, 0 otherwise
 */
int entryExists(char *name, entriesList *head) {
    int exists = 0;
    while (head) {
        if (head -> name && !strcmp(name, head -> name))
            exists = 1;
        head = head -> next;
    }
    return exists;
}

/*
 * returns the name of the entry/extern label of the node
 */
char *getEntryName(entriesList *node) {
    return node -> name;
}

/*
 * returns whether the node is empty or not
 */
int isEmpty(entriesList *node) {
    return node -> name ? 0 : 1;
}

/*
 * returns the next linked node to the current node
 */
entriesList *getNextEntry(entriesList *node) {
    return node -> next;
}

/*
 * returns the line in which the label was added to the list
 */
long getEntryLine(entriesList *node) {
    return node -> currentLine;
}



/*
 * frees the list of entry and extern labels.
 */
void freeEntriesList(entriesList *head) {
    while (head) {
        entriesList *temp = head;
        head = head -> next;
        free(temp);
    }
}

/*
 * frees the labels in the entriesList (called in case of an error before the second iteration)
 */
void freeUnusedEntries(entriesList *head) {
    entriesList *temp = head;
    while (temp) {
        free(temp -> name);
        temp = temp -> next;
    }
}
