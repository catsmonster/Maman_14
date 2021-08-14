
#include "../includes/secondIteration.h"
#include <stdlib.h>


/*
 * in case an external symbol was encountered, prints the address in which it was used as an argument in an instruction
 * to the .ext file
 */
void handleExtSymbol(typesOfCommands type, char *label, errorCodes *error, long currentLine, FILE **extFile, long address) {
    if (type == I_TYPE_CONDITIONAL) {
        *error = ERROR_EXTERN_IN_CONDITIONAL;
        printInputError(ERROR_EXTERN_IN_CONDITIONAL, label, currentLine, 0);
    } else {
        printEntOrExtLine(extFile, address, label);
    }
}

/*
 * updates the information which couldn't be fully processed in the first iteration into the code image, and prints
 * the .ext file if necessary.
 */
void updateCodeImage(codeNode *codeImageHead, dataTable *listOfSymbols, errorCodes *error, long ICF, FILE **extFile) {
    while (codeImageHead && !isFileError(*error)) {
        long symbolAddress, currentLine, address;
        typesOfCommands type;
        symbolTypes symbolType;
        char *label;
        label = getCodeNodeLabel(codeImageHead);
        if (label) {
            type = getCodeNodeType(codeImageHead);
            currentLine = getCodeNodeLine(codeImageHead);
            address = getCodeNodeAddress(codeImageHead);
            symbolAddress = getSymbolAddress(label, listOfSymbols, error, currentLine);
            if (!isFileError(*error)) {
                symbolType = getSymbolType(label, listOfSymbols, error, currentLine);
                symbolAddress = symbolType == DATA ? symbolAddress + ICF : symbolType == EXTERN ? 0 : symbolAddress;
                if (symbolType != EXTERN) {
                    adjustMissingAddressToCodeImage(symbolAddress, codeImageHead, error);
                } else {
                    handleExtSymbol(type, label, error, currentLine, extFile, address);
                }
            }
        }
        codeImageHead = getNextCodeNode(codeImageHead);
    }
}

/*
 * iterates over the list of entries and prints any entries to the .ent file
 */
void updateEntries(entriesList *entriesHead, dataTable *listOfSymbols, errorCodes *error, FILE **entFile, long ICF) {
    while (entriesHead && !isEmpty(entriesHead) && !isFileError(*error)) {
        long symbolAddress, currentLine = getEntryLine(entriesHead);
        char *name = getEntryName(entriesHead);
        if (!isFileError(*error)) {
            symbolTypes symbolType = getSymbolType(name, listOfSymbols, error, currentLine);
            if (!isFileError(*error)) {
                if (symbolType != EXTERN) {
                    symbolAddress = getSymbolAddress(name, listOfSymbols, error, currentLine);
                    symbolAddress = symbolType == DATA ? symbolAddress + ICF : symbolAddress;
                    printEntOrExtLine(entFile, symbolAddress, name);
                    free(name);
                }
                else {
                    *error = ERROR_EXTERN_IS_ENTRY;
                    printInputError(ERROR_EXTERN_IS_ENTRY, name, currentLine, 0);
                }
            }
        }
        else if (isFileError(*error))
            free(name);
        entriesHead = getNextEntry(entriesHead);
    }
}


/*
 * running the second iteration over the input file to finalize the necessary data before converting to machine code.
 */
void secondIteration(dataTable *listOfSymbols, errorCodes * error, codeNode *codeImageHead, long ICF,
                     entriesList *entriesHead, FILE **extFile, FILE **entFile) {
    updateEntries(entriesHead, listOfSymbols, error, entFile, ICF);
    updateCodeImage(codeImageHead, listOfSymbols, error, ICF, extFile);
}
