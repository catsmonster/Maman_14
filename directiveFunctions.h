
#ifndef MAMAN_14_GUIDANCEFUNCTIONS_H
#define MAMAN_14_GUIDANCEFUNCTIONS_H
#define NUM_OF_DIRECTIVE_CMDS 6
#define EXTERNAL_DIRECTIVE 5
#define ENTRY_DIRECTIVE 4
#include "dataImage.h"
#include "dataTable.h"
#include "entriesList.h"

/*
 * handles reading a .dh directive line
 * accepts the position pointer, the input line string, a pointer to the data counter, an error codes pointer,
 * data image tail and the current line pointer
 */
void dh(int *, const char [], long *, errorCodes *, dataNode**, long);
/*
 * handles reading a .dw directive line
 * accepts the position pointer, the input line string, a pointer to the data counter, an error codes pointer,
 * data image tail and the current line pointer
 */
void dw(int *, const char[], long *, errorCodes *, dataNode**, long);
/*
 * handles reading a .db directive line
 * accepts the position pointer, the input line string, a pointer to the data counter, an error codes pointer,
 * data image tail and the current line pointer
 */
void db(int *, const char [], long *, errorCodes *, dataNode**, long);
/*
 * handles reading an .asciz directive line
 * accepts the position pointer, the input line string, a pointer to the data counter, an error codes pointer,
 * data image tail and the current line pointer
 */
void asciz(int *, const char[], long *,  errorCodes *, dataNode**, long);
/*
 * handles reading an .entry directive line
 * accepts the position pointer, the input line string, an error code pointer, the current line counter, and the
 * entries linked list
 */
void entry(int *, const char[], errorCodes *, long, entriesList**, entriesList *);
/*
 * handles reading an .external directive line
 * accepts the position pointer, the input line string, the table of symbols, an error code pointer and the current line counter
 */
void external(int *, const char[], dataTable *, errorCodes *, long);
#endif
