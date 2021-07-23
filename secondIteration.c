
#include "secondIteration.h"
#include "fileErrors.h"


void updateCodeImage(codeNode *codeImageHead, dataTable *listOfSymbols, int *error, long ICF, FILE **extFile) {
    while (codeImageHead && !isFileError(*error)) {
        long symbolAddress, currentLine, address;
        int type, symbolType;
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
                    setMissingData(symbolAddress, codeImageHead);
                } else {
                    if (type == I_TYPE_CONDITIONAL) {
                        *error = ERROR_EXTERN_IN_CONDITIONAL;
                        printError(ERROR_EXTERN_IN_CONDITIONAL, ERROR_TYPE_INPUT, 3, label, currentLine, 0);
                    } else {
                        printEntOrExtLine(extFile, address, label);
                    }
                }
            }
        }
        codeImageHead = getNextCodeNode(codeImageHead);
    }
}

void updateEntries(entriesOrExtern *entriesOrExternHead, dataTable *listOfSymbols, int *error, FILE **entFile, long ICF) {
    while (entriesOrExternHead && !isFileError(*error)) {
        int declaredType;
        long symbolAddress, currentLine = getEntryOrExternLine(entriesOrExternHead);
        declaredType = getEntryOrExternType(entriesOrExternHead);
        if (declaredType != EXTERN) {
            char *name = getEntryOrExternName(entriesOrExternHead);
            if (!isFileError(*error)) {
                int symbolType = getSymbolType(name, listOfSymbols, error, currentLine);
                if (!isFileError(*error)) {
                    if (symbolType != EXTERN) {
                        symbolAddress = getSymbolAddress(name, listOfSymbols, error, currentLine);
                        symbolAddress = symbolType == DATA ? symbolAddress + ICF : symbolAddress;
                        printEntOrExtLine(entFile, symbolAddress, name);
                    }
                    else {
                        *error = ERROR_EXTERN_IS_ENTRY;
                        printError(ERROR_EXTERN_IS_ENTRY, ERROR_TYPE_INPUT, 3, name, currentLine, 0);
                    }
                }
            }
        }
        entriesOrExternHead = getNextEntryOrExtern(entriesOrExternHead);
    }
}


/*
 * running the second iteration over the input file to finalize the necessary data before converting to machine code.
 */
void secondIteration(dataTable *listOfSymbols, int * error, codeNode *codeImageHead, long ICF,
                     entriesOrExtern *entriesOrExternHead, FILE **extFile, FILE **entFile) {
    updateEntries(entriesOrExternHead, listOfSymbols, error, entFile, ICF);
    updateCodeImage(codeImageHead, listOfSymbols, error, ICF, extFile);
}
