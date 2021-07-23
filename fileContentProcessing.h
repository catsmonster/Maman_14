
#ifndef MAMAN_14_FILECONTENTPROCESSING_H
#define MAMAN_14_FILECONTENTPROCESSING_H
#include "listOfCommands.h"
#include "dataTable.h"
#include "dataImage.h"
#include "entriesOrExternList.h"
/*
 * API of input related processing
 */
void readFile(FILE *, void(*[])(int*, const char[], long*, int*, dataNode**, long), CMD *, dataTable *,
              dataNode *dataImageHead, codeNode *codeImageHead, entriesOrExtern*
              ,char *fileName);
#endif
