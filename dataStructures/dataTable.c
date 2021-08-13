
#include "../includes/dataTable.h"
#include <stdlib.h>
#include <string.h>
#define HASH_COEFFICIENT_ODD 37
#define HASH_COEFFICIENT_EVEN 101
#define INITIAL_SYMBOLS_TABLE_SIZE 32


typedef struct symbol symbol;

/*
 * each symbol has a name (which determines its address in the hash table), an address based on the DC
 * in the point of creation, and an attribute that could signal code/data and entry/external.
 * the pointer "next" is designed for conflict resolutions.
 */
struct symbol {
    char * name;
    long address;
    symbolTypes attribute;
    symbol *next;
};

/*
 * the structure holding the symbols
 */
struct dataTable {
    symbol **entries;
    long currentTableSize;
    long numOfSymbols;
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
        p->entries = (symbol **) calloc(INITIAL_SYMBOLS_TABLE_SIZE, sizeof(symbol *));
        if (!(p->entries)) {
            printMemoryError(ERROR_MEMORY_ALLOCATION);
            free(p);
            p = NULL;
        }
        else {
            p -> currentTableSize = INITIAL_SYMBOLS_TABLE_SIZE;
            p -> numOfSymbols = 0;
        }
    }
    return p;
}


/*
 * generates an easy to reproduce hash based on the name of the symbol and the position of each character in the name
 */
long generateHash(const char *name, dataTable *table) {
    int i;
    long res = 0;
    for (i = 0; name[i] ; ++i) {
        res += name[i] * (i % 2 ? HASH_COEFFICIENT_ODD : HASH_COEFFICIENT_EVEN);
    }
    res %= table -> currentTableSize;
    return res;
}

/*
 * attempts to create a symbol based on input name, address and the first attribute (code/data)
 */
symbol *createSymbol(char *name, long address, symbolTypes attribute) {
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
 * resolving any conflicts with same hash results, raising flag if the symbol name already exists.
 */
void handleConflict(symbol **entries, long posInTable, int *itemExists, symbol *newSymbol, char *name, dataTable *table) {
    if (!strcmp(entries[posInTable]->name, name)) {
        *itemExists = 1;
        free(newSymbol);
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
            (table -> numOfSymbols)++;
            if (!previousPos->next) {
                printMemoryError(ERROR_MEMORY_ALLOCATION);
                *itemExists = ERROR_MEMORY_ALLOCATION;
            }
        }
    }
}

/*
 * adds a symbol to the data table
 */
void insertSymbol(symbol *symbolToAdd, symbol **entries, dataTable *table) {
    int itemExists = 0;
    long newPos = generateHash(symbolToAdd -> name, table);
    if (entries[newPos]) {
        handleConflict(entries, newPos, &itemExists, symbolToAdd, symbolToAdd -> name, table);
    }
    else {
        entries[newPos] = symbolToAdd;
    }
}

/*
 * takes the entries from the old data table and reassigns it to the new one
 */
void redistributeEntriesToNewTable(symbol **entries, dataTable *table, long previousTableSize) {
    long i;
    for (i = 0; i < previousTableSize; i++) {
        symbol *currentSymbol = table -> entries[i];
        if (currentSymbol) {
            if (currentSymbol -> next) {
                symbol *nextSymbol = currentSymbol -> next;
                while (nextSymbol) {
                    currentSymbol -> next = NULL;
                    insertSymbol(nextSymbol, entries, table);
                    nextSymbol = nextSymbol -> next;
                }
            }
            insertSymbol(currentSymbol, entries, table);
        }
    }
}

/*
 * if the number of symbols added is greater or equal to half the size of the current table, squares the size of the table
 * and redistributes the entries into the new list.
 */
void handlePossibleTableExtension(dataTable *table, int *itemExists) {
    if (table -> numOfSymbols >= table -> currentTableSize / 2) {
        symbol **tempEntries = table -> entries;
        symbol **entries;
        long previousTableSize = table -> currentTableSize;
        (table -> currentTableSize) *= (table -> currentTableSize);
        entries = calloc(table -> currentTableSize, sizeof(symbol *));
        if (!entries) {
            printMemoryError(ERROR_MEMORY_ALLOCATION);
            *itemExists = ERROR_MEMORY_ALLOCATION;
        }
        else {
            redistributeEntriesToNewTable(entries, table, previousTableSize);
            table -> entries = entries;
            free(tempEntries);
        }
    }
}

/*
 * adds a new symbol to the hashtable, and returning 1 if item was added more than once
 */
int addItemToDataTable(char *name, long address, symbolTypes firstAttribute, dataTable *table) {
    int itemExists = 0;
    long posInTable;
    symbol *newSymbol = createSymbol(name, address, firstAttribute);
    handlePossibleTableExtension(table, &itemExists);
    posInTable = generateHash(name, table);
    if (!newSymbol) {
        itemExists = ERROR_MEMORY_ALLOCATION;
    }
    if (!table -> entries[posInTable]) {
        table -> entries[posInTable] = newSymbol;
        (table -> numOfSymbols)++;
    }
    else {
        handleConflict(table -> entries, posInTable, &itemExists, newSymbol, name, table);
    }
    return itemExists;
}

/*
 * returns the symbol with the corresponding name, or NULL if it doesn't exist.
 */
symbol *getSymbol(char *name, dataTable *table) {
    long posOfLabel = generateHash(name, table);
    symbol * dataEntry = table -> entries[posOfLabel];
    if (dataEntry) {
        while (dataEntry && strcmp(name, dataEntry -> name) != 0) {
            dataEntry = dataEntry->next;
        }
    }
    return dataEntry && strcmp(dataEntry -> name, name) == 0 ? dataEntry : NULL;
}

/*
 * returns the address of the symbol
 */
long getSymbolAddress(char *name, dataTable *table, errorCodes *error, long currentLine) {
    long res;
    symbol *dataEntry = getSymbol(name, table);
    if (dataEntry)
        res = dataEntry -> address;
    else {
        *error = ERROR_SYMBOL_NOT_FOUND;
        printInputError(ERROR_SYMBOL_NOT_FOUND, name, currentLine, 0);
        res = ERROR_SYMBOL_NOT_FOUND;
    }
    return res;
}

/*
 * returns the type of the symbol
 */
symbolTypes getSymbolType(char *name, dataTable *table, errorCodes *error, long currentLine) {
    symbolTypes type;
    symbol *dataEntry = getSymbol(name, table);
    if (dataEntry) {
        type = dataEntry -> attribute;
    }
    else {
        *error = ERROR_SYMBOL_NOT_FOUND;
        printInputError(ERROR_SYMBOL_NOT_FOUND, name, currentLine,0);
        type = SYMBOL_NOT_FOUND;
        free(name);
    }
    return type;
}

/*
 * runs over the entire hashtable, freeing all the symbols and the labels.
 */
void freeDataTable(dataTable *table) {
    long i;
    for (i = 0; i < table -> currentTableSize; i++) {
        if (table -> entries[i]) {
            symbol *currPos = table -> entries[i];
            while (currPos) {
                symbol *temp = currPos;
                free(temp->name);
                currPos = currPos -> next;
                free(temp);
            }
        }
    }
    free(table -> entries);
    free(table);
}
