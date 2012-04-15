#include "fileio.h"
#include <errno.h>

// Create directories:
int makedir(const char * const dirname){
    if ( mkdir(dirname,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH  | S_IXOTH) != 0)
	{
		switch (errno)
		{
			case EACCES:
				fprintf(stderr,"Write permission is denied for the parent directory in which the new directory is to be added.\n");
				abort();
				break;
			case EEXIST:
				fprintf(stderr,"\"%s\" already exists.\n",dirname);
				break;
			case EMLINK:
				fprintf(stderr,"The parent directory has too many links (entries).\n");
				abort();
				break;
			case ENOSPC:
				fprintf(stderr,"The file system doesn't have enough room to create the new directory\n");
				abort();
				break;
			case EROFS:
				fprintf(stderr,"The parent directory of the directory being created is on a read-only file system and cannot be modified.\n");
				abort();
				break;
		} 
	}
return 0;
}

/** Close a file */
void Fclose(FILE *fd) {
    if (fd!=NULL) fclose(fd);
    return;
}

/** Open a file */
FILE * Fopen(const char *filename, const char *mode) {
    FILE * fd=fopen(filename,mode);
    if ( fd == NULL )
    {
		fprintf(stderr,"Could not open file \"%s\" in \"%s\" mode\n",filename,mode);
		exit(-1);
	}
    return fd;
}
