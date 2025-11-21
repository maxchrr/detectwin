#ifndef TITLE_H
#define TITLE_H

#include <stdio.h>
#include <stdlib.h>

DIR * myOpenDir(char * nomDir);

bool isDir(char * filename);

void myCloseDir(DIR * d);

void fileTab(int tabLength, char * tab[]);

int display(int argc, char * argv[]);

#endif
