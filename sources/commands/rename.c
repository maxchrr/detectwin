#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void renommer(const char * old, const char* new){
	int rn= rename(old,new);
	assert(rn ==0);
	printf("bien renommé\n");
}
