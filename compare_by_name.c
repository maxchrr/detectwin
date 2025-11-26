#include <string.h>

#include "compares.h"

bool cmpname(const char * file1, const char * file2){
	return (strcmp(file1,file2) == 0);
}
