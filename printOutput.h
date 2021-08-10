
#ifndef MAMAN_14_PRINTOUTPUT_H
#define MAMAN_14_PRINTOUTPUT_H
#include "codeImage.h"
#include "dataImage.h"
#include "entriesList.h"
#include <stdio.h>

void printOBFile(codeNode *codeImageHead, dataNode *dataImageHead, long IC, long DC, char *fileName);
FILE * generateExtFile(char *fileName);
FILE *generateEntFile(char *fileName);
void printEntOrExtLine(FILE **fp, long address, char *label);
void deleteEntExtFiles(char *fileName, FILE *ent, FILE *ext);
#endif
