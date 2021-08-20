
#include "../includes/secondIteration.h"
#include <stdlib.h>


/*
 * close entry or extern files. In case an error was encountered, deletes any output file, if created
 */
void closeEntExtFiles(const errorCodes *error, FILE *entFile, FILE *extFile, char *fileName) {
    if (extFile)
        fclose(extFile);
    if (entFile)
        fclose(entFile);
    if (isFileError(*error)) {
        deleteEntExtFiles(fileName, entFile, extFile);
    }
}

/*
 * in case an external symbol was encountered, prints the address in which it was used as an argument in an instruction
 * to a new .ext file
 */
void handlePrintExt(typesOfCommands type, char *label, errorCodes *error, long currentLine, FILE **extFile,
                    long address, char *fileName) {
    if (!(*extFile)) {
        *extFile = generateExtOrEntFile(fileName, EXT_FILE);
    }
    if (!*extFile) {
        *error = ERROR_FILE_PERMISSIONS;
        printFileError(ERROR_FILE_PERMISSIONS, "");
    }
    else {
        if (type == I_TYPE_CONDITIONAL) {
            *error = ERROR_EXTERN_IN_CONDITIONAL;
            printInputError(ERROR_EXTERN_IN_CONDITIONAL, label, currentLine, 0);
        } else {
            printEntOrExtLine(extFile, address, label);
        }
    }
}

/*
 * updates the information which couldn't be fully processed in the first iteration into the code image, and prints
 * the .ext file if necessary.
 */
void updateCodeImage(codeNode *codeImageHead, dataTable *listOfSymbols, errorCodes *error, long ICF, FILE **extFile,
                     char *fileName) {
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
                if (symbolType != EXTERN)
                    adjustMissingAddressToCodeImage(symbolAddress, codeImageHead, error);
                else
                    handlePrintExt(type, label, error, currentLine, extFile, address, fileName);
            }
        }
        codeImageHead = getNextCodeNode(codeImageHead);
    }
}

/*
 * creates an .ent file if one doesn't already exist, and prints the address of each symbol which was declared as entry.
 */
void handlePrintEntry(FILE **entFile, errorCodes *error, char *name, char *fileName,
                      symbolTypes symbolType, dataTable *listOfSymbols, long currentLine, long ICF) {
    if (symbolType != EXTERN) {
        long symbolAddress;
        symbolAddress = getSymbolAddress(name, listOfSymbols, error, currentLine);
        symbolAddress = symbolType == DATA ? symbolAddress + ICF : symbolAddress;
        if (!(*entFile)) {
            *entFile = generateExtOrEntFile(fileName, ENT_FILE);
        }
        if (!(*entFile)) {
            *error = ERROR_FILE_PERMISSIONS;
            printFileError(ERROR_FILE_PERMISSIONS, "");
        }
        else {
            printEntOrExtLine(entFile, symbolAddress, name);
            free(name);
        }
    }
    else {
        *error = ERROR_EXTERN_IS_ENTRY;
        printInputError(ERROR_EXTERN_IS_ENTRY, name, currentLine, 0);
    }
}

/*
 * iterates over the list of entries and prints any entries to the .ent file
 */
void writeEntriesToFile(entriesList *entriesHead, dataTable *listOfSymbols, errorCodes *error, FILE **entFile, long ICF,
                        char *fileName) {
    while (entriesHead && !isEmpty(entriesHead) && !isFileError(*error)) {
        long currentLine = getEntryLine(entriesHead);
        char *name = getEntryName(entriesHead);
        if (!isFileError(*error)) {
            symbolTypes symbolType = getSymbolType(name, listOfSymbols, error, currentLine);
            if (!isFileError(*error)) {
                handlePrintEntry(entFile, error, name, fileName, symbolType, listOfSymbols, currentLine, ICF);
            }
        }
        entriesHead = getNextEntry(entriesHead);
    }
    if (entriesHead && !isEmpty(entriesHead) && isFileError(*error)) {
        freeUnusedEntries(entriesHead);
    }
}


/*
 * running the second iteration over the information extracted from the file to finalize the necessary data before
 * converting to machine code.
 */
void secondIteration(dataTable *listOfSymbols, errorCodes * error, codeNode *codeImageHead, long ICF,
                     entriesList *entriesHead, char *fileName) {
    FILE *entFile = NULL, *extFile = NULL;
    writeEntriesToFile(entriesHead, listOfSymbols, error, &entFile, ICF, fileName);
    updateCodeImage(codeImageHead, listOfSymbols, error, ICF, &extFile, fileName);
    closeEntExtFiles(error, entFile, extFile, fileName);
}
