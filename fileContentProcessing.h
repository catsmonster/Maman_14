
#ifndef MAMAN_14_FILECONTENTPROCESSING_H
#define MAMAN_14_FILECONTENTPROCESSING_H
#include "listOfCommands.h"
#include "dataTable.h"
#include "dataImage.h"
#include "entriesList.h"
/*
 * API of input related processing. accepts a FILE pointer, list of commands pointer, table of symbols pointer,
 * data image linked list, code image linked list, entries linked list, and a string representing the input file name
 */
void readFileAndGenerateOutput(FILE *fp, CMD *listOfCommands, dataTable *listOfSymbols, dataNode *dataImageHead,
                               codeNode *codeImageHead, entriesList *entriesHead, char *fileName);
#endif
