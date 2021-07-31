
#ifndef MAMAN_14_FIRSTITERATION_H
#define MAMAN_14_FIRSTITERATION_H
#include <stdio.h>
#include "listOfCommands.h"
#include "dataTable.h"
#include "dataImage.h"
#include "entriesOrExternList.h"
#include "fileErrors.h"
void firstIteration(FILE *fp, long * DC, long * IC, errorCodes *error, void(*[])(int*, const char[],long*, errorCodes *,
        dataNode**, long), CMD *, dataTable*, dataNode *, codeNode *, entriesOrExternList *);

#endif
