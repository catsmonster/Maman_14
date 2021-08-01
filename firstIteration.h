
#ifndef MAMAN_14_FIRSTITERATION_H
#define MAMAN_14_FIRSTITERATION_H
#include <stdio.h>
#include "listOfCommands.h"
#include "dataTable.h"
#include "dataImage.h"
#include "entriesOrExternList.h"
#include "fileErrors.h"

/*
 * handles the building of the symbols table, the data image and most of the code image (minus the final addresses).
 * also creates a list of labels flagged as entry or extern.
 */
void firstIteration(FILE *fp, long * DC, long * IC, errorCodes *error, CMD *, dataTable*, dataNode *, codeNode *,
                    entriesOrExternList *);

#endif
