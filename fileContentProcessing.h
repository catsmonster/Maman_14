
#ifndef MAMAN_14_FILECONTENTPROCESSING_H
#define MAMAN_14_FILECONTENTPROCESSING_H
#include "listOfCommands.h"
#include "dataTable.h"
#include "dataImage.h"
#include "entriesOrExternList.h"
/*
 * API of input related processing. accepts a FILE pointer, list of commands pointer, table of symbols pointer,
 * data image linked list, code image linked list, entries/extern linked list, and a string representing the
 * input file name
 */
void readFile(FILE *, CMD *, dataTable *, dataNode *dataImageHead, codeNode *codeImageHead, entriesOrExternList*,
              char *fileName);
#endif
