#include "../includes/firstIteration.h"
#include "../includes/inputUtils.h"
#include "../includes/directiveFunctions.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#define MAX_COMMAND_LENGTH 10



/*
 * iterates over the different directive commands and if the input command is the same as one of the commands,
 * returns the position in the array of that command. The array is built to mirror the function pointers array
 * for the different directive commands, so the position returned is the same as the one of the corresponding function
 */
int compareToKnownDirectiveCMDs(char *directiveInput) {
    int selected = NOT_FOUND, i;
    char *knownCMDs[] = {"db", "dw", "dh", "asciz", "entry", "extern"};
    for (i = 0; selected == NOT_FOUND && i < NUM_OF_DIRECTIVE_CMDS; i++) {
        if (!strcmp(directiveInput, knownCMDs[i]))
            selected = i;
    }
    return selected;
}


/*
 * attempts to read a directive command from the given line, after a dot was encountered
 */
int getDirectiveCMD(int *pos, const char *inputLine, long currentLine, errorCodes *error) {
    int selectedCMD = NOT_FOUND, counter = 0;
    char directiveInput[MAX_DIRECTIVE_CMD_LENGTH];
    (*pos)++;
    while (!isFileError(selectedCMD) && counter < MAX_DIRECTIVE_CMD_LENGTH && islower(inputLine[*pos])) {
        directiveInput[counter++] = inputLine[*pos];
        (*pos)++;
    }
    if (counter > MAX_DIRECTIVE_CMD_LENGTH) {
        printInputError(ERROR_DIRECTIVE_CMD_TOO_LONG, "", currentLine, *pos);
        selectedCMD = ERROR_DIRECTIVE_CMD_TOO_LONG;
    }
    else {
        directiveInput[counter] = '\0';
    }
    if (!isFileError(selectedCMD)) {
        if (isValidEndOfCommand(pos, inputLine, currentLine, error)) {
            selectedCMD = compareToKnownDirectiveCMDs(directiveInput);
            if (selectedCMD == NOT_FOUND) {
                printInputError(ERROR_DIRECTIVE_CMD_NOT_FOUND, directiveInput, currentLine, *pos);
                selectedCMD = ERROR_DIRECTIVE_CMD_NOT_FOUND;
            }
        }
    }
    return selectedCMD;
}

/*
 * extracts command name from the input and checks against known commands list, returning the position in the list
 * of the found command, or -1 if the command wasn't found
 */
int getCommandName(int *pos, const char inputLine[], long currentLine, CMD *listOfCommands, errorCodes *error) {
    int commandPos = NOT_FOUND, i = 0, startPos = *pos;
    char commandName[MAX_COMMAND_LENGTH];
    while (isalpha(inputLine[*pos]) && i < MAX_COMMAND_LENGTH) {
        commandName[i++] = inputLine[(*pos)++];
    }
    if (startPos == *pos) {
        printInputError(ERROR_MISSING_ARGUMENT, "", currentLine, *pos);
        commandPos = ERROR_MISSING_ARGUMENT;
    }
    else if (isValidEndOfCommand(pos, inputLine, currentLine, error)) {
        commandName[i] = '\0';
        commandPos = findCommand(commandName, listOfCommands);
        if (commandPos == NOT_FOUND) {
            printInputError(ERROR_COMMAND_NOT_FOUND, commandName, currentLine, *pos);
            commandPos = ERROR_COMMAND_NOT_FOUND;
        }
    }
    return commandPos;
}

/*
 * checks against the known list of commands, and returns 1 if the given name matches a known command
 */
int isKnownCMD(char *labelName, CMD *listOfCommands) {
    int isKnown = 0;
    if (findCommand(labelName, listOfCommands) != NOT_FOUND)
        isKnown = 1;
    return isKnown;
}

/*
 * prints an error message and resets the start position in case of an error while reading a label
 */
void handleLabelError(int *pos, int startPos, long currentLine, char labelName[], int *isLabel, errorCodes error) {
    if (error == ERROR_LABEL_COMMAND_CONFLICT)
        printInputError(ERROR_LABEL_COMMAND_CONFLICT, labelName, currentLine, *pos);
    if (error ==ERROR_MISSING_WHITESPACE)
        printInputError(ERROR_MISSING_WHITESPACE, "", currentLine, *pos);
    if (error ==ERROR_MAX_LENGTH)
        printInputError(ERROR_MAX_LENGTH, "", currentLine, startPos);
    *pos = startPos;
    *isLabel = error;
}

/*
 * attempts to read a label, if a label is illegal, or if the read text isn't a label, resets the position pointer and
 * returns an error code. otherwise, returns 1.
 */
int isValidLabel(int *pos, const char inputLine[], long currentLine, CMD *listOfCommands, char labelName[], errorCodes *error) {
    int startPos = *pos, isLabel = 0, i = 0;
    if (isalpha(inputLine[*pos])) {
        while (isalnum(inputLine[*pos]) && i < MAX_LABEL_LENGTH) {
            labelName[i++] = inputLine[(*pos)++];
        }
        if (inputLine[*pos] == ':' && startPos != *pos) {
            if (isspace(inputLine[*pos + 1])) {
                labelName[i] = '\0';
                (*pos)++;
                if (isKnownCMD(labelName, listOfCommands)) {
                    handleLabelError(pos, startPos, currentLine, labelName, &isLabel, ERROR_LABEL_COMMAND_CONFLICT);
                    *error = ERROR_LABEL_COMMAND_CONFLICT;
                } else {
                    isLabel = 1;
                }
            }
            else {
                handleLabelError(pos, startPos, currentLine, labelName, &isLabel, ERROR_MISSING_WHITESPACE);
                *error = ERROR_MISSING_WHITESPACE;
            }
        }
        else {
            if (i == MAX_LABEL_LENGTH) {
                handleLabelError(pos, startPos, currentLine, labelName, &isLabel, ERROR_MAX_LENGTH);
                *error = ERROR_MAX_LENGTH;
            }
            (*pos) = startPos;
        }
    } else if (isgraph(inputLine[*pos]) && inputLine[*pos] != '.') {
        handleInvalidCharacterError(inputLine[*pos], currentLine, error, pos);
        *error = ERROR_INVALID_CHARACTER;
        isLabel = ERROR_INVALID_CHARACTER;
    }
    return isLabel;

}

/*
 * attempts to read a command, be it a directive or a "normal" command
 */
void readCommand(int *pos, const char inputLine[], long currentLine, CMD *listOfCommands, int *selectedDirectiveCMD,
                int *selectedCMD, errorCodes *error){
    skipWhiteSpaces(pos, inputLine);
    if (inputLine[*pos] == '.') {
        *selectedDirectiveCMD = getDirectiveCMD(pos, inputLine, currentLine, error);
        if (isFileError(*selectedDirectiveCMD))
            *error = *selectedDirectiveCMD;
    }
    else {
        *selectedCMD = getCommandName(pos, inputLine, currentLine, listOfCommands, error);
        if (isFileError(*selectedCMD))
            *error = *selectedCMD;
    }
}

/*
 * handles an error in case there is a label name conflict
 */
void handleNameConflict(int itemExists, char *labelName, long currentLine, int pos, errorCodes *error){
    if (itemExists) {
        printInputError(ERROR_LABEL_NAME_CONFLICT, labelName, currentLine, pos);
        *error = ERROR_LABEL_NAME_CONFLICT;
    }
}

/*
 * handles labels and directives executions
 */
void handleDirectiveExecution(int *pos, const char inputLine[], errorCodes *error, int validLabel, int selectedDirectiveCMD,
                              int itemExists, char labelName[], long currentLine, long *DC, dataTable *listOfSymbols,
                              entriesList **entriesTail, dataNode **dataImageTail, int *hasExtern, int *hasEntry,
                              entriesList *entriesHead) {
    void (*directiveFuncArr[NUM_OF_DIRECTIVE_CMDS - 2])(int *, const char[], long *, errorCodes *, dataNode**, long) = {db, dw, dh, asciz};
    if (validLabel && selectedDirectiveCMD != ENTRY_DIRECTIVE && selectedDirectiveCMD != EXTERNAL_DIRECTIVE) {
        itemExists = addItemToDataTable(labelName, *DC, DATA, listOfSymbols);
        handleNameConflict(itemExists, labelName, currentLine, *pos, error);
    }
    if (!itemExists) {
        if (selectedDirectiveCMD == EXTERNAL_DIRECTIVE || selectedDirectiveCMD == ENTRY_DIRECTIVE) {
            if (validLabel) {
                printInputError(WARNING_LABEL_IGNORED, labelName, currentLine, *pos);
            }
            if (selectedDirectiveCMD == EXTERNAL_DIRECTIVE) {
                external(pos, inputLine, listOfSymbols, error, currentLine);
                *hasExtern = 1;
            }
            else {
                entry(pos, inputLine, error, currentLine, entriesTail, entriesHead);
                *hasEntry = 1;
            }
        }
        else
            directiveFuncArr[selectedDirectiveCMD](pos, inputLine, DC, error, dataImageTail, currentLine);
    }
    if (!validLabel)
        free(labelName);
}

/*
 * handles label and commands execution
 */
void handleCommandExecution(int *pos, const char inputLine[], int validLabel, int itemExists, char labelName[], long *IC,
                            dataTable *listOfSymbols, long currentLine, errorCodes *error, int selectedCMD,
                            CMD *listOfCommands, codeNode **codeImageTail) {
    if (validLabel) {
        itemExists = addItemToDataTable(labelName, *IC, CODE, listOfSymbols);
        handleNameConflict(itemExists, labelName, currentLine, *pos, error);
    }
    else
        free(labelName);
    if (!itemExists) {
        runCMD(pos, inputLine, selectedCMD, listOfCommands, codeImageTail, IC, error, currentLine);
    }
}

/*
 * if a label was found, adds it to the list of symbols, runs the found command or directive
 */
void addLabelAndExecuteCommand(int selectedDirectiveCMD, char labelName[], int validLabel, int selectedCMD, int *pos,
                               const char inputLine[], long * DC, long * IC, long currentLine,
                               CMD *listOfCommands, dataTable *listOfSymbols, errorCodes *error, dataNode **dataImageTail,
                               codeNode **codeImageTail, entriesList **entriesTail, int *hasExtern, int *hasEntry,
                               entriesList *entriesHead) {
    int itemExists = 0;
    if (selectedDirectiveCMD != NOT_FOUND) {
        handleDirectiveExecution(pos, inputLine, error, validLabel, selectedDirectiveCMD, itemExists, labelName,
                                 currentLine, DC, listOfSymbols, entriesTail, dataImageTail, hasExtern, hasEntry, entriesHead);
    }
    else if (selectedCMD != NOT_FOUND) {
       handleCommandExecution(pos, inputLine, validLabel, itemExists, labelName, IC, listOfSymbols, currentLine, error,
                              selectedCMD, listOfCommands, codeImageTail);
    }
}

/*
 * coordinates the line reading process
 */
void readLine(int *pos, const char inputLine[], long * DC, long * IC, long currentLine, CMD *listOfCommands,
              dataTable *listOfSymbols, errorCodes *error, dataNode **dataImageTail, codeNode **codeImageTail,
              entriesList **entriesTail, int *hasExtern, int *hasEntry, entriesList *entriesHead) {
    int selectedDirectiveCMD = NOT_FOUND, selectedCMD = NOT_FOUND;
    char *labelName = calloc(MAX_LABEL_LENGTH, sizeof(char));
    int validLabel = isValidLabel(pos, inputLine, currentLine, listOfCommands, labelName, error);
    if (!isFileError(validLabel)) {
        readCommand(pos, inputLine, currentLine, listOfCommands, &selectedDirectiveCMD, &selectedCMD, error);
        if (!isFileError(selectedDirectiveCMD) && !isFileError(selectedCMD)) {
            addLabelAndExecuteCommand(selectedDirectiveCMD, labelName, validLabel, selectedCMD, pos, inputLine, DC, IC,
                                      currentLine, listOfCommands, listOfSymbols, error, dataImageTail,
                                      codeImageTail, entriesTail, hasExtern, hasEntry, entriesHead);
        }
    }
    else {
        free(labelName);
    }

}

/*
 * running the first iteration over the input file to build the basic skeleton of the output, skipping any blank lines
 * or comment lines
 */
void firstIteration(FILE *fp, long * DC, long * IC, errorCodes *error, CMD *listOfCommands, dataTable *listOfSymbols,
                    dataNode *dataImageHead, codeNode *codeImageHead, entriesList *entriesOrExternHead, int *hasExtern,
                    int *hasEntry) {
    dataNode *dataImageTail = dataImageHead;
    codeNode *codeImageTail = codeImageHead;
    entriesList *entriesOrExternTail = entriesOrExternHead;
    long currentLine = 1;
    char inputLine[MAX_LINE_LENGTH];
    while (fgets(inputLine, MAX_LINE_LENGTH, fp)) {
        int pos = 0;
        advanceToStartOfValidInput(&pos, inputLine, currentLine, error);
        if (pos >= 0) {
            readLine(&pos, inputLine, DC, IC, currentLine, listOfCommands, listOfSymbols, error,
                     &dataImageTail, &codeImageTail, &entriesOrExternTail, hasExtern, hasEntry, entriesOrExternHead);
        }
        currentLine++;
    }
}
