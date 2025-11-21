#include <stdio.h>
#include <assert.h>

int deplace(const char * source, const char * destination){
	int mv = rename(source, destination);
	assert (mv == 0);
	printf("fichier deplacé avec succès \n");
}

int main(int argc, char * argv[]){
	assert(argc == 3);
	char * src = argv[1];
	char * dest = argv[2];
	int dep = deplace(src,dest);
	return 0;
}
