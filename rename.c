#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void renommer(const char * old, const char* new){
	int rn= rename(old,new);
	assert(rn ==0);
	printf("bien renommé\n");
}


int main(int argc, char *argv[]){
	if (argc!=3){
		printf("veiller donner un fichier à  renommer et commment le renommer");
	}
	char * oldname = argv[1];
	char * newname = argv[2];
	renommer(oldname, newname);
	
	return EXIT_SUCCESS;
}
