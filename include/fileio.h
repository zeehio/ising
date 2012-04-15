
#ifndef FILEIO_H
#define FILEIO_H

#include <stdlib.h>
#include <stdio.h>

#include <sys/stat.h>

int makedir (const char * const dirname);

void Fclose(FILE *fd);

/** Open a file */
FILE * Fopen(const char *filename, const char *mode);

#endif

