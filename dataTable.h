
#ifndef MAMAN_14_DATATABLE_H
#define MAMAN_14_DATATABLE_H
#include "fileErrors.h"
enum {
    CODE = 400,
    DATA,
    ENTRY,
    EXTERN
};

typedef struct dataTable dataTable;

int addItemToDataTable(char * labelName, long address, int firstAttribute, dataTable *listOfSymbols);
void freeDataTable(dataTable *);
dataTable *initializeDataTable();
long getSymbolAddress(char *name, dataTable *table, errorCodes *error, long line);
int getSymbolType(char *name, dataTable *table, errorCodes *error, long line);
#endif
