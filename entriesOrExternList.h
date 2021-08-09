
#ifndef MAMAN_14_ENTRIESOREXTERNLIST_H
#define MAMAN_14_ENTRIESOREXTERNLIST_H
#include "fileErrors.h"
#include "dataTable.h"
/*
 * this struct holds the names of labels which are flagged as entry or extern
 */
typedef struct entriesOrExternNode entriesOrExternList;
/*
 * frees the linked list of entriesOrExtern
 * accepts head of entriesOrExternList
 */
void freeEntriesOrExternList(entriesOrExternList *head);
/*
 * links a new node to the tail of the list, and returns the new tail.
 * accepts a label name, a symbol type, an entriesOrExternList tail, an error code pointer and the current line counter
 */
entriesOrExternList *setEntryOrExternListTail(char *name, symbolTypes type, entriesOrExternList *tail, errorCodes *error, long currentLine);
/*
 * returns a new empty head of entriesOrExternList
 */
entriesOrExternList *initializeEntriesOrExternList();
/*
 * returns the label name of the given node
 * accepts an entriesOrExternList node
 */
char *getEntryOrExternName(entriesOrExternList *node);
/*
 * returns the type of the given node
 * accepts an entriesOrExternList node
 */
symbolTypes getEntryOrExternType(entriesOrExternList *node);
/*
 * returns the next linked node of the given node, NULL if one doesn't exist.
 * accepts an entriesOrExternList node
 */
entriesOrExternList *getNextEntryOrExtern(entriesOrExternList *node);
/*
 * returns the line in which the symbol was added to the list
 * accepts an entriesOrExternList node
 */
long getEntryOrExternLine(entriesOrExternList *node);

#endif
