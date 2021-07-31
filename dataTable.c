
#include "dataTable.h"
#include <stdlib.h>
#include <string.h>
#include "fileErrors.h"
#define HASH_COEFFICIENT_ODD 37
#define HASH_COEFFICIENT_EVEN 101


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
        printMemoryError(ERROR_MEMORY_ALLOCATION);
    }
    else {
        p->entries = (symbol **) calloc(currentTableSize, sizeof(symbol *));
        if (!(p->entries)) {
            printMemoryError(ERROR_MEMORY_ALLOCATION);
            free(p);
            p = NULL;
        }
    }
    return p;
}


/*
 * generates an easy to reproduce hash based on the name of the symbol and the position of each character in the name
 */
long generateHash(const char *name) {
    int i;
    long res = 0;
    for (i = 0; name[i] ; ++i) {
        res += name[i] * (i % 2 ? HASH_COEFFICIENT_ODD : HASH_COEFFICIENT_EVEN);
    }
    res %= currentTableSize;
    return res;
}

/*
 * attempts to create a symbol based on input name, address and the first attribute (code/data)
 */
symbol *createSymbol(char *name, long address, int attribute) {
    symbol *entry = (symbol *)calloc(1, sizeof(symbol));
    if (!entry) {
        printMemoryError(ERROR_MEMORY_ALLOCATION);
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
void handleConflict(symbol **entries, long posInTable, int *itemExists, symbol *newSymbol, char *name) {
    if (!strcmp(entries[posInTable]->name, name)) {
        *itemExists = 1;
    }
    else {
        symbol *currentPos = entries[posInTable];
        symbol *previousPos;
        while (currentPos && !(*itemExists)) {
            if (strcmp(currentPos -> name, name) != 0) {
                previousPos = currentPos;
                currentPos = currentPos -> next;
            }
            else {
                *itemExists = 1;
            }
        }
        if (!(*itemExists)) {
            previousPos->next = newSymbol;
            numOfSymbols++;
            if (!previousPos->next) {
                printMemoryError(ERROR_MEMORY_ALLOCATION);
                *itemExists = ERROR_MEMORY_ALLOCATION;
            }
        }
    }
}

void insertSymbol(symbol *symbolToAdd, symbol **entries) {
    int itemExists = 0;
    long newPos = generateHash(symbolToAdd -> name);
    if (entries[newPos]) {
        handleConflict(entries, newPos, &itemExists, symbolToAdd, symbolToAdd -> name);
    }
    else {
        entries[newPos] = symbolToAdd;
    }
}

void populateEntries(symbol **entries, dataTable *table, long previousTableSize) {
    long i;
    for (i = 0; i < previousTableSize; i++) {
        symbol *currentSymbol = table -> entries[i];
        if (currentSymbol) {
            if (currentSymbol -> next) {
                symbol *nextSymbol = currentSymbol -> next;
                while (nextSymbol) {
                    currentSymbol -> next = NULL;
                    insertSymbol(nextSymbol, entries);
                    nextSymbol = nextSymbol -> next;
                }
            }
            insertSymbol(currentSymbol, entries);
        }
    }
}

void handlePossibleTableExtension(dataTable *table, int *itemExists) {
    if (numOfSymbols >= currentTableSize) {
        symbol **tempEntries = table -> entries;
        symbol **entries;
        long previousTableSize = currentTableSize;
        currentTableSize *= currentTableSize;
        entries = calloc(currentTableSize, sizeof(symbol *));
        if (!entries) {
            printMemoryError(ERROR_MEMORY_ALLOCATION);
            *itemExists = ERROR_MEMORY_ALLOCATION;
        }
        else {
            populateEntries(entries, table, previousTableSize);
            table -> entries = entries;
            free(tempEntries);
        }
    }
}

/*
 * adds a new symbol to the hashtable, and returning 1 if item was added more than once
 */
int addItemToDataTable(char *name, long address, int firstAttribute, dataTable *table) {
    int itemExists = 0;
    long posInTable;
    symbol *newSymbol = createSymbol(name, address, firstAttribute);
    handlePossibleTableExtension(table, &itemExists);
    posInTable = generateHash(name);
    if (!newSymbol) {
        itemExists = ERROR_MEMORY_ALLOCATION;
    }
    if (!table -> entries[posInTable]) {
        table -> entries[posInTable] = newSymbol;
        numOfSymbols++;
    }
    else {
        handleConflict(table -> entries, posInTable, &itemExists, newSymbol, name);
    }
    return itemExists;
}

symbol *findSymbol(char *name, dataTable *table) {
    long posOfLabel = generateHash(name);
    symbol * dataEntry = table -> entries[posOfLabel];
    if (dataEntry) {
        while (dataEntry && strcmp(name, dataEntry -> name) != 0) {
            dataEntry = dataEntry->next;
        }
    }
    return dataEntry && strcmp(dataEntry -> name, name) == 0 ? dataEntry : NULL;
}


long getSymbolAddress(char *name, dataTable *table, errorCodes *error, long currentLine) {
    long res;
    symbol *dataEntry = findSymbol(name, table);
    if (dataEntry)
        res = dataEntry -> address;
    else {
        *error = ERROR_SYMBOL_NOT_FOUND;
        printInputError(ERROR_SYMBOL_NOT_FOUND, name, currentLine, 0);
        res = ERROR_SYMBOL_NOT_FOUND;
    }
    return res;
}




int getSymbolType(char *name, dataTable *table, errorCodes *error, long currentLine) {
    int type;
    symbol *dataEntry = findSymbol(name, table);
    if (dataEntry) {
        type = dataEntry -> attribute;
    }
    else {
        *error = ERROR_SYMBOL_NOT_FOUND;
        printInputError(ERROR_SYMBOL_NOT_FOUND, name, currentLine,0);
        type = ERROR_SYMBOL_NOT_FOUND;
    }
    return type;
}

/*
 * runs over the entire hashtable, freeing all the symbols.
 */
void freeDataTable(dataTable *table) {
    long i;
    for (i = 0; i < currentTableSize; i++) {
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
