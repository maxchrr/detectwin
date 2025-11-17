#ifndef TITLE_H
#define TITLE_H

#include <stdio.h>
#include <stdlib.h>

DIR * myOpenDir(char * nomDir);

void myCloseDir(DIR * d);

int display(int argc, char * argv[]);

#endif
