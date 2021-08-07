
#ifndef MAMAN_14_ENTRIESOREXTERNLIST_H
#define MAMAN_14_ENTRIESOREXTERNLIST_H
#include "fileErrors.h"
#include "dataTable.h"

typedef struct entriesOrExternNode entriesOrExternList;
void freeEntriesOrExternList(entriesOrExternList *head);
entriesOrExternList *setEntryOrExternListTail(char *name, symbolTypes type, entriesOrExternList *tail, errorCodes *error, long currentLine);
entriesOrExternList *initializeEntriesOrExternList();
char *getEntryOrExternName(entriesOrExternList *node);
symbolTypes getEntryOrExternType(entriesOrExternList *node);
entriesOrExternList *getNextEntryOrExtern(entriesOrExternList *node);
long getEntryOrExternLine(entriesOrExternList *node);

#endif
