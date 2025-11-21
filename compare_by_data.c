#include <assert.h>
#include <stdbool.h>

#include "compares.h"

unsigned char checksum (const char * filename){

    FILE * f = fopen(filename,"r");
    assert(f != 0);

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
    return c1 == c2;
}
