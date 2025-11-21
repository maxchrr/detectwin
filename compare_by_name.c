#include <string.h>

#include "compares.h"

int cmpname(const char * file1, const char * file2){

    if(strcmp(file1,file2)==0){
        printf("true\n");
    }
    return 0;
}
