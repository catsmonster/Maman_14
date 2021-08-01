
#ifndef MAMAN_14_FILECONTENTPROCESSING_H
#define MAMAN_14_FILECONTENTPROCESSING_H
#include "listOfCommands.h"
#include "dataTable.h"
#include "dataImage.h"
#include "entriesOrExternList.h"
/*
 * API of input related processing
 */
void readFile(FILE *, CMD *, dataTable *, dataNode *dataImageHead, codeNode *codeImageHead, entriesOrExternList*,
              char *fileName);
#endif
