
#ifndef MAMAN_14_GUIDANCEFUNCTIONS_H
#define MAMAN_14_GUIDANCEFUNCTIONS_H
#define NUM_OF_DIRECTIVE_CMDS 6
#define EXTERNAL_DIRECTIVE 5
#define ENTRY_DIRECTIVE 4
#include "dataImage.h"
#include "dataTable.h"
#include "entriesOrExternList.h"

void dh(int *, const char [], long *, int*, dataNode**, long);
void dw(int *, const char[], long *, int*, dataNode**, long);
void db(int *, const char [], long *, int*, dataNode**, long);
void asciz(int *, const char[], long *,  int*, dataNode**, long);
void entry(int *, const char[], int*, long, entriesOrExtern**);
void external(int *, const char[], dataTable *, int *, long, entriesOrExtern**);
#endif
