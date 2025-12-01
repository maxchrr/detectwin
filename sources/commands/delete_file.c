#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void suppr(const char * filename){
	int rem = remove(filename);
	assert(rem == 0);
}
