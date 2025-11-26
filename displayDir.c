#include <assert.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdbool.h>
#include <string.h>

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
void displayTab(struct dirTab d){
    for(int i = 2; i < d.size; i++)
    	printf("%s\n", d.dir[i]);
}

/* Foncton qui transforme un répertoire en tableau de chaines de caractères */
void tabFromDir(char * namedir, struct dirTab *d){   	
    	
    struct dirent *dir;
    
    DIR * file = myOpenDir(namedir);
    
    while((dir = readdir(file)) != NULL){
    	d->dir = realloc(d->dir,sizeof(char*)* (d->size +1));
    	d->dir[d->size] = malloc(strlen(dir->d_name));
    	strcpy(d->dir[d->size], dir->d_name);
    	d->size++;
    }
	
    myCloseDir(file); 
}
