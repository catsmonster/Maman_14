
#ifndef MAMAN_14_ENTRIESLIST_H
#define MAMAN_14_ENTRIESLIST_H
#include "fileErrors.h"
#include "dataTable.h"
/*
 * this struct holds the names of labels which are flagged as entry
 */
typedef struct entriesNode entriesList;
/*
 * frees the linked list of entries
 * accepts head of entriesList
 */
void freeEntriesList(entriesList *head);
/*
 * links a new node to the tail of the list, and returns the new tail.
 * accepts a label name, a symbol type, an entriesList tail, an error code pointer and the current line counter
 */
entriesList *setEntryListTail(char *name, entriesList *tail, errorCodes *error, long currentLine);
/*
 * returns a new empty head of entriesList
 */
entriesList *initializeEntriesList();
/*
 * returns the label name of the given node
 * accepts an entriesList node
 */
char *getEntryName(entriesList *node);
/*
 * returns the type of the given node
 * accepts an entriesList node
 */
int isEmpty(entriesList *node);
/*
 * returns the next linked node of the given node, NULL if one doesn't exist.
 * accepts an entriesList node
 */
entriesList *getNextEntry(entriesList *node);
/*
 * returns the line in which the symbol was added to the list
 * accepts an entriesList node
 */
long getEntryLine(entriesList *node);

/*
 * determines whether an entry with the same name was already declared
 * accepts a string name and the entries list head
 */
int entryExists(char *name, entriesList *head);

#endif
