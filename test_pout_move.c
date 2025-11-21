#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void suppr(const char * filename){
	char ch;
	printf("appuyez sur d pour supprimer le fichier %s \n", filename);
	scanf(" %c", &ch);
	
	if (ch == 'd'){
		int rem = remove(filename);
		assert(rem == 0);
		printf("le fichier a été supprimé \n");
	} else {
		printf(" d non pressé, suppression annulée \n");
	}
}

int main(int argc, char * argv[]){
	assert(argc == 2);
	char * nomFich = argv[1];
	suppr(nomFich);
	return 0;
}
