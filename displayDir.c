#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <dirent.h>

/* Fonction permettant d'ouvrir un répertoire */
DIR * myOpenDir(char * nomDir){
    DIR * d = opendir(nomDir);
    assert(d != NULL);
    return d;
}

/* Fonction pour fermer un répertoire */
void myCloseDir(DIR * d){
    int ret = closedir(d);
    assert(ret != -1);
}


int main(int argc, char * argv[]){

    // Vérifie qu'il n'y ait qu'une chaine de caractère passée en paramètre
    if (argc != 2)
    {	
    	fprintf(stderr,"error : not enough/too many arguments\n");
    	return EXIT_FAILURE;
    }    	
    	
    struct dirent *dir;
    
    DIR * d = myOpenDir(argv[1]);
    
    while((dir = readdir(d)) != NULL)
    	printf("%s\n", dir->d_name);
    	
    myCloseDir(d);
    
    return EXIT_SUCCESS;
}


