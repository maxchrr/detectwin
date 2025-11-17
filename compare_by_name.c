#include <stdio.h>
#include <string.h>

int main(int argc,char * argv[]){

    if(argc != 3){
        printf("erreur");
        return -1;
    }

    char * name1 = argv[1];
    char * name2 = argv[2];

    if(strcmp(name1,name2)==0){
        printf("true\n");
    }
    return 0;
}
