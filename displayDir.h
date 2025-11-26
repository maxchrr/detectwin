#ifndef TITLE_H
#define TITLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>

struct dirTab{
    char ** dir;
    int size;
};

DIR * myOpenDir(char * nomDir);

bool isDir(char * filename);

void myCloseDir(DIR * d);

void displayTab(struct dirTab d);

void tabFromDir(char * namedir, struct dirTab *d);

#endif
