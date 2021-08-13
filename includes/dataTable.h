
#ifndef MAMAN_14_DATATABLE_H
#define MAMAN_14_DATATABLE_H
#include "fileErrors.h"
/*
 * codes for the symbol types
 */
typedef enum {
    CODE = 400,
    DATA,
    EXTERN,
    SYMBOL_NOT_FOUND
}symbolTypes;

typedef struct dataTable dataTable;

/*
 * adds a new label to the symbols table
 * accepts the label name to be added, the address of the symbol, the type of the symbol and the table of symbols.
 */
int addItemToDataTable(char * labelName, long address, symbolTypes firstAttribute, dataTable *listOfSymbols);
/*
 * frees the table of symbols. accepts the symbols table
 */
void freeDataTable(dataTable *);
/*
 * returns a pointer to a table of symbols
 */
dataTable *initializeDataTable();
/*
 * returns the address of the symbol
 * accepts the name of the symbol, the symbols table, an error code pointer, and the current line counter
 */
long getSymbolAddress(char *name, dataTable *table, errorCodes *error, long line);\
/*
 * returns the type of the symbol
 * accepts the name of the symbol, the symbols table, an error code pointer and the current line counter
 */
symbolTypes getSymbolType(char *name, dataTable *table, errorCodes *error, long line);
#endif
