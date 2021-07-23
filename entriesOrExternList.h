
#ifndef MAMAN_14_ENTRIESOREXTERNLIST_H
#define MAMAN_14_ENTRIESOREXTERNLIST_H

typedef struct entriesOrExternNode entriesOrExtern;
void freeEntriesOrExternList(entriesOrExtern *head);
entriesOrExtern *setEntryOrExternTail(char *name, int type, entriesOrExtern *tail, int *error, long currentLine);
entriesOrExtern *initializeEntriesOrExtern();
char *getEntryOrExternName(entriesOrExtern *node);
int getEntryOrExternType(entriesOrExtern *node);
entriesOrExtern *getNextEntryOrExtern(entriesOrExtern *node);
entriesOrExtern *addExternToHead(entriesOrExtern *head, long address, char *name, int type, long currentLine);
long getEntryOrExternLine(entriesOrExtern *node);

#endif
