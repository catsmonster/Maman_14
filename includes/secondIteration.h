#ifndef MAMAN_14_SECONDITERATION_H
#define MAMAN_14_SECONDITERATION_H
#include "dataTable.h"
#include "codeImage.h"
#include "entriesList.h"
#include "printOutput.h"
#include "fileErrors.h"

/*
 * handles filling in the gaps in the code image and printing the .ent and .ext files if necessary.
 * accepts the table of symbols, an error code pointer, the image head, the ICF value, the entries list, the address
 * of an .ext file pointer and the address of an .ent file pointer.
 */
void secondIteration(dataTable *listOfSymbols, errorCodes *error, codeNode *, long ICF, entriesList *entriesHead, char *fileName);

#endif
