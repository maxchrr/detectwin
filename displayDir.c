#include <assert.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdbool.h>

#include "displayDir.h"

/* Fonction permettant d'ouvrir un répertoire */
DIR * myOpenDir(char * nomDir){
    DIR * d = opendir(nomDir);
    assert(d != NULL);
    return d;
}

/* Fonction permettant de vérifier qu'un fichier est ou non un répertoire */
bool isDir(char * filename){
    DIR * d = opendir(filename);
    if (d != NULL)  {
    	myCloseDir(d);
    	printf("%s est un DOSSIER\n", filename);
    	return true;
    	}
    else {
    	printf("%s est un FICHIER\n", filename);
    	return false; 
    	}
}

/* Fonction pour fermer un répertoire */
void myCloseDir(DIR * d){
    int ret = closedir(d);
    assert(ret != -1);
}

/* Fonction pour afficher un tableau de fichiers */
void fileTab(int tabLength, char * tab[]){
    for(int i = 0; i < tabLength; i++)
    	printf("%s\n", tab[i]);
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
    
    fileTab(nb,result);

    myCloseDir(d);
    free(result);
    
    return EXIT_SUCCESS;
}

int main (int argc, char * argv[]){

	display(argc,argv);
	return EXIT_SUCCESS;
}
