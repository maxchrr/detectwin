#include <stdio.h>
#include <stdlib.h>

#include "displayDir.h"
#include "compares.h"

struct dirTab fileInDir(char * file, struct dirTab d){
    char ** result = (char **)malloc(sizeof(char *));
    struct dirTab tabRes = {result,0};
    for(int i = 0; i < d.size; i++){
	bool by_data = compare(file, d.dir[i]);
	bool by_name = cmpname(file, d.dir[i]);
    	if (by_data || by_name)
	{
    	    tabRes.dir = realloc(tabRes.dir,sizeof(char*)* (tabRes.size +1));
    	    tabRes.dir[tabRes.size] = d.dir[i];
    	    tabRes.size++;  	    
    	};
    }
    return tabRes;
}

int main(int argc, char* argv[])
{
    	if (argc != 3) return EXIT_FAILURE;


    char ** result = (char **)malloc(sizeof(char *));
    struct dirTab tab = {result,0};
    tabFromDir(argv[2], &tab);
    tab = fileInDir(argv[1], tab);
    displayTab(tab);

    return EXIT_SUCCESS;
}
