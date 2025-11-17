#include <assert.h>
#include <sys/types.h>
#include <dirent.h>

#include "displayDir.h"

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


int display(int argc, char * argv[]){

    // Vérifie qu'il n'y ait qu'une chaine de caractère passée en paramètre
    if (argc != 2)
    {	
    	fprintf(stderr,"error : not enough/too many arguments\n");
    	return EXIT_FAILURE;
    }    	
    	
    struct dirent *dir;
    char ** result =(char **)malloc(sizeof(char *));
    int nb = 0;
    DIR * d = myOpenDir(argv[1]);
    
    while((dir = readdir(d)) != NULL){
    	result = realloc(result,sizeof(char*)* (nb +1));
    	result[nb] = dir->d_name;
    	nb ++;
    }
    
    for (int i=0; i<nb;i++){
    	printf("%s\n",result[i]);
    }
    myCloseDir(d);
    free(result);
    
    return EXIT_SUCCESS;
}

int main (int argc, char * argv[]){

	display(argc,argv);
	return EXIT_SUCCESS;
}
