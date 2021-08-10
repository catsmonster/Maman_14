#ifndef MAMAN_14_SECONDITERATION_H
#define MAMAN_14_SECONDITERATION_H
#include "dataTable.h"
#include "codeImage.h"
#include "entriesList.h"
#include "printOutput.h"
#include "fileErrors.h"

void secondIteration(dataTable *listOfSymbols, errorCodes *error, codeNode *, long ICF,
                     entriesList *entriesHead, FILE **extFile, FILE **entFile);

#endif
