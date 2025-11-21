#ifndef COMPARES
#define COMPARES

#include <stdio.h>
#include <stdbool.h>

unsigned char checksum (const char * filename);

bool compare(const char * file1, const char * file2);

int cmpname(const char * file1, const char * file2);

#endif
