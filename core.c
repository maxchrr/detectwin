#include <stdlib.h>
#include <stdio.h>

#include "displayDir.h"
#include "compare_by_data.h"
#include "compare_by_name.h"

char ** fileInDir(char * file, char ** dir){

}

int main(int argc, char * argv[]){
    
    if (argc != 2)
    {
    	fprintf(stderr,"error : not enough/too many arguments\n");
    	return EXIT_FAILURE;
    }

    char ** tabfiles = display(argv[1]);
    int index = 0;
    
    /*Notes :
    - pour chaque fichier ayant effectué une vérification sur tout les autres, le prendre en 		
    compte pour le prochain fichier (ne pas faire des vérifications deux fois a.c -> b.c sans 
    faire b.c -> a.c)
    - faire de haut en bas, d'abord le premier fichier, puis celui d'après; si c'est un 	
    répertoire alors on regarde son premier fichier
    - faire fonction vérifiant si un fichier est dans un dossier
    */
    
    while (tabfiles != NULL)
    {
    	if (isFile(tabfiles[index]))
    	    
    }
    

    return EXIT_SUCCESS;
}
