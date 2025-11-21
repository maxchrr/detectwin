#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "compares.h"
#include "delete_file.c"
#include "move_file.c"

int main(int argc,char * argv[]){

    if(argc != 3){
	printf("erreur");
	return -1;
    }
    
    bool jsp = false;
    
    if(compare(argv[1],argv[2])){
        jsp = true;
        printf("true, les fichiers ont le même contenu \n");
    }
    
    if(jsp == true){
    	char ch;
    	printf("que souhaitez vous faire avec les deux fichier ? \nappuyez sur d pour les supprimer, ou sur m pour les bouger \n");
    	scanf(" %c",&ch);
    
    	if(ch=='d'){
    	    int num;
    	    printf("tapez 1 pour supprimer le premier fichier, 2 pour le second\n");
    	    scanf(" %d",&num);
    	    if(num==1){
    	    	char * nomFich = argv[1];
	    	suppr(nomFich);
	    	printf("le fichier %s a bien été supprimé \n", nomFich);
	    } 
	    else if(num==2){
	    	char * nomFich = argv[2];
	    	suppr(nomFich);
	    	printf("le fichier %s a bien été supprimé \n", nomFich);
	    } 
	    else {
	    	printf("aucune action entreprise\n");
	    }
    	}
    	else if(ch == 'm'){
    	    int num;
    	    printf("tapez 1 pour deplacer le premier fichier, 2 pour le second\n");
    	    scanf(" %d",&num);
    	    if(num==1){
    	    	char * src = argv[1];
    	    	char dest[256];
    	    	printf("entrez le chemin relatif vers l'endroit ou vous voulez déplacer le dossier INCLUANT le nom de fichier à la fin du chemin \n");
    	    	scanf("%255s",dest);
	    	int dep = deplace(src,dest);
	    	printf("fichier déplacé avec succès\n");
	    } 
	    else if(num==2){
	    	char * src = argv[2];
	    	char dest[256];
    	    	printf("entrez le chemin relatif vers l'endroit ou vous voulez déplacer le dossier INCLUANT le nom de fichier à la fin du chemin \n");
    	    	scanf("%255s",dest);
	    	int dep = deplace(src,dest);
	    	printf("fichier déplacé avec succès\n");
	    } 
	    else {
	    	printf("aucune action entreprise\n");
	    }
    	}
        else{
    	    printf("aucune action entreprise\n");
        };
    } else {
        printf("les fichiers ont un contenu different \n");
    }
    return 0;
}
