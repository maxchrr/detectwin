#include <assert.h>
#include <stdbool.h>

#include "compares.h"

unsigned char checksum (const char * filename){

    FILE* f = fopen(filename,"r");
    if (f == 0)
	{
		printf("Ce fichier n'existe pas\n");
		return 0;
	}

    unsigned char check = 0;
    int c;

    while ((fgetc(f)) != EOF ){
	c = fgetc(f);
	check -= c;
    }

    int ret = fclose(f);
    assert(ret == 0);
    return check;
}

bool compare(const char * file1, const char * file2){

    unsigned char c1 = checksum(file1);
    unsigned char c2 = checksum(file2);
	if (c1 == 0 || c2 == 0) return false;
    return c1 == c2;
}
