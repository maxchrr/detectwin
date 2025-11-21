#include <stdio.h>
#include <assert.h>

int deplace(const char * source, const char * destination){
	int mv = rename(source, destination);
	assert (mv == 0);
}
