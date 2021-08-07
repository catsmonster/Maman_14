
#ifndef MAMAN_14_DATATABLE_H
#define MAMAN_14_DATATABLE_H
#include "fileErrors.h"
typedef enum {
    CODE = 400,
    DATA,
    ENTRY,
    EXTERN,
    SYMBOL_NOT_FOUND
}symbolTypes;


typedef struct dataTable dataTable;

int addItemToDataTable(char * labelName, long address, symbolTypes firstAttribute, dataTable *listOfSymbols);
void freeDataTable(dataTable *);
dataTable *initializeDataTable();
long getSymbolAddress(char *name, dataTable *table, errorCodes *error, long line);
symbolTypes getSymbolType(char *name, dataTable *table, errorCodes *error, long line);
#endif
