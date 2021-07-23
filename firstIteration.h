
#ifndef MAMAN_14_FIRSTITERATION_H
#define MAMAN_14_FIRSTITERATION_H
#include <stdio.h>
#include "listOfCommands.h"
#include "dataTable.h"
#include "dataImage.h"
#include "entriesOrExternList.h"
void firstIteration(FILE *fp, long * DC, long * IC, int *error, void(*[])(int*, const char[],long*, int*,
        dataNode**, long), CMD *, dataTable*, dataNode *, codeNode *, entriesOrExtern *);

#endif
