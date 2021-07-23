#ifndef MAMAN_14_SECONDITERATION_H
#define MAMAN_14_SECONDITERATION_H
#include "dataTable.h"
#include "codeImage.h"
#include "entriesOrExternList.h"
#include "printOutput.h"

void secondIteration(dataTable *listOfSymbols, int * error, codeNode *, long ICF,
                     entriesOrExtern *entriesOrExternHead, FILE **extFile, FILE **entFile);

#endif
