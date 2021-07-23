
#include "dataTable.h"
#include <stdlib.h>
#include <string.h>
#include "fileErrors.h"
#define HASH_COEFFICIENT_ODD 37
#define HASH_COEFFICIENT_EVEN 101
#define TABLE_SIZE 33554432

typedef struct symbol symbol;

/*
 * each symbol has a name (which determines its address in the hash table), an address based on the DC
 * in the point of creation, and an attribute that could signal code/data and entry/external.
 * the pointer "next" is designed for conflict resolutions.
 */
struct symbol {
    char * name;
    long address;
    int attribute;
    symbol *next;
};

/*
 * the structure holding the symbols
 */
struct dataTable {
    symbol **entries;
};

/*
 * attempts to assign memory to the whole hashtable structure
 */
dataTable *initializeDataTable() {
    dataTable *p = (dataTable *)malloc(sizeof(dataTable));
    if (!p) {
        printError(ERROR_MEMORY_ALLOCATION, ERROR_TYPE_MEMORY, 0);
    }
    else {
        p->entries = (symbol **) calloc(TABLE_SIZE, sizeof(symbol *));
        if (!(p->entries)) {
            printError(ERROR_MEMORY_ALLOCATION, ERROR_TYPE_MEMORY, 0);
            free(p);
            p = NULL;
        }
    }
    return p;
}


/*
 * generates an easy to reproduce hash based on the name of the symbol and the position of each character in the name
 */
int generateHash(const char *name) {
    int i;
    long res = 0;
    for (i = 0; name[i] ; ++i) {
        res += name[i] * (i % 2 ? HASH_COEFFICIENT_ODD : HASH_COEFFICIENT_EVEN);
    }
    res %= TABLE_SIZE;
    return (int)res;
}

/*
 * attempts to create a symbol based on input name, address and the first attribute (code/data)
 */
symbol *createSymbol(char *name, long address, int attribute) {
    symbol *entry = (symbol *)calloc(1, sizeof(symbol));
    if (!entry) {
        printError(ERROR_MEMORY_ALLOCATION, ERROR_TYPE_MEMORY, 0);
    }
    else {
        entry->name = name;
        entry->address = address;
        entry->attribute = attribute;
    }
    return entry;
}

/*
 * resolving any conflicts
 */
void handleConflict(dataTable *table, int posInTable, int *itemExists, symbol *newSymbol, char *name) {
    if (!strcmp(table -> entries[posInTable]->name, name)) {
        *itemExists = 1;
    }
    else {
        symbol *currentPos = table -> entries[posInTable];
        while (currentPos && !(*itemExists)) {
            if (strcmp(currentPos -> name, name) != 0) {
                currentPos = table -> entries[posInTable] -> next;
            }
            else {
                *itemExists = 1;
            }
        }
        if (!itemExists) {
            currentPos = newSymbol;
            if (!currentPos) {
                printError(ERROR_MEMORY_ALLOCATION, ERROR_TYPE_MEMORY, 0);
                *itemExists = ERROR_MEMORY_ALLOCATION;
            }
        }
    }
}

/*
 * adds a new symbol to the hashtable, and returning 1 if item was added more than once
 */
int addItemToDataTable(char *name, long address, int firstAttribute, dataTable *table) {
    int itemExists = 0, posInTable = generateHash(name);
    symbol *newSymbol = createSymbol(name, address, firstAttribute);
    if (!newSymbol) {
        itemExists = ERROR_MEMORY_ALLOCATION;
    }
    if (!table -> entries[posInTable]) {
        table -> entries[posInTable] = newSymbol;
    }
    else {
        handleConflict(table, posInTable, &itemExists, newSymbol, name);
    }
    return itemExists;
}

symbol *findSymbol(char *name, dataTable *table) {
    int posOfLabel = generateHash(name);
    symbol * dataEntry = table -> entries[posOfLabel];
    if (dataEntry) {
        while (dataEntry && strcmp(name, dataEntry -> name) != 0) {
            dataEntry = dataEntry->next;
        }
    }
    return dataEntry && strcmp(dataEntry -> name, name) == 0 ? dataEntry : NULL;
}

long getSymbolAddress(char *name, dataTable *table, int *error, long currentLine) {
    long res;
    symbol *dataEntry = findSymbol(name, table);
    if (dataEntry)
        res = dataEntry -> address;
    else {
        *error = ERROR_SYMBOL_NOT_FOUND;
        printError(ERROR_SYMBOL_NOT_FOUND, ERROR_TYPE_INPUT, 3, name, currentLine, 0);
        res = ERROR_SYMBOL_NOT_FOUND;
    }
    return res;
}




int getSymbolType(char *name, dataTable *table, int *error, long currentLine) {
    int type;
    symbol *dataEntry = findSymbol(name, table);
    if (dataEntry) {
        type = dataEntry -> attribute;
    }
    else {
        *error = ERROR_SYMBOL_NOT_FOUND;
        printError(ERROR_SYMBOL_NOT_FOUND, ERROR_TYPE_INPUT, 3, name, currentLine,0);
        type = ERROR_SYMBOL_NOT_FOUND;
    }
    return type;
}

/*
 * runs over the entire hashtable, freeing all the symbols.
 */
void freeDataTable(dataTable *table) {
    int i;
    for (i = 0; i < TABLE_SIZE; i++) {
        if (table -> entries[i]) {
            symbol *currPos = table -> entries[i];
            while (currPos) {
                symbol *temp = currPos;
                currPos = currPos -> next;
                free(temp);
            }
        }
    }
    free(table);
}
