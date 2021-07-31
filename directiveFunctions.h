
#ifndef MAMAN_14_GUIDANCEFUNCTIONS_H
#define MAMAN_14_GUIDANCEFUNCTIONS_H
#define NUM_OF_DIRECTIVE_CMDS 6
#define EXTERNAL_DIRECTIVE 5
#define ENTRY_DIRECTIVE 4
#include "dataImage.h"
#include "dataTable.h"
#include "entriesOrExternList.h"

void dh(int *, const char [], long *, errorCodes *, dataNode**, long);
void dw(int *, const char[], long *, errorCodes *, dataNode**, long);
void db(int *, const char [], long *, errorCodes *, dataNode**, long);
void asciz(int *, const char[], long *,  errorCodes *, dataNode**, long);
void entry(int *, const char[], errorCodes *, long, entriesOrExternList**);
void external(int *, const char[], dataTable *, errorCodes *, long, entriesOrExternList**);
#endif
