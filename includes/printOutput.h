
#ifndef MAMAN_14_PRINTOUTPUT_H
#define MAMAN_14_PRINTOUTPUT_H
#include "codeImage.h"
#include "dataImage.h"
#include "entriesList.h"
#include <stdio.h>

/*
 * type of output files
 */
typedef enum {
    OB_FILE = 700,
    EXT_FILE,
    ENT_FILE
}fileTypes;

/*
 * prints the .ob file with the same name as the input .as file
 * accepts the code image, the data image, the IC value, the DC value and the file name
 */
void printOBFile(codeNode *codeImageHead, dataNode *dataImageHead, long IC, long DC, char *fileName);
/*
 * creates a new .ext or .ent file according to the specified type, with the same name as the input .as file
 * accepts the file name string, and a specified file type
 */
FILE * generateExtOrEntFile(char *fileName, fileTypes type);
/*
 * prints to the given .ent or .ext file pointer a line
 * accepts the address of the file pointer, the address of the label, and the name of the label
 */
void printEntOrExtLine(FILE **fp, long address, char *label);
/*
 * deletes any .ent or .ext files in case of an error.
 * accepts the file name, and the .ent and .ext file pointers
 */
void deleteEntExtFiles(char *fileName, FILE *ent, FILE *ext);
#endif
