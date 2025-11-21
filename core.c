#include <stdlib.h>
#include <stdio.h>

#include "displayDir.h"
#include "compares.h"

char ** fileInDir(char * file, struct dirTab d){
    char ** result = (char **)malloc(sizeof(char *));
    struct dirTab tabRes = {result,0};
    for(int i = 0; i < d.size; i++){
    	if ((compare(file, d.dir[i])) || (cmpname(3, d.dir[i]))){
    	    tabRes.result = realloc(tabRes.result,sizeof(char*)* (tabRes.size +1));
    	    tabRes.result[tabRes.size] = d.dir[i];
    	    tabRes.size++;  	    
    	    }   
    }
    return result;
}

int main(int argc, char * argv[]){
    
    if (argc != 3)
    {
    	fprintf(stderr,"error : not enough/too many arguments\n");
    	return EXIT_FAILURE;
    }

    //char ** tabfiles = display(argv[1]);
    //int index = 0;
    
    /*Notes :
    - pour chaque fichier ayant effectué une vérification sur tout les autres, le prendre en 		
    compte pour le prochain fichier (ne pas faire des vérifications deux fois a.c -> b.c sans 
    faire b.c -> a.c)
    - faire de haut en bas, d'abord le premier fichier, puis celui d'après; si c'est un 	
    répertoire alors on regarde son premier fichier
    - faire fonction vérifiant si un fichier est dans un dossier
    */
    
    /*while (tabfiles != NULL)
    {
    	if (isFile(tabfiles[index]))
    	    
    }*/
    char ** result = (char **)malloc(sizeof(char *));
    struct dirTab tab = {result,0};
    tabFromDir(argv[2], &tab);
    tab = fileInDir(argv[1], tab);
    displayTab(tab);

    return EXIT_SUCCESS;
}
