#include <assert.h>
#include <stdbool.h>

#include <stdio.h>

#include <string.h>

#include "compares.h"

static unsigned char checksum(const char *filename)
{
	FILE* f = fopen(filename, "rb");
	if (!f) return 0;

	unsigned char check = 0;
	int c;
	while ((c = fgetc(f)) != EOF )
		check -= (unsigned char)c;

	fclose(f);
	return check;
}

bool cmpdata(const char *path1, const char *path2)
{
	unsigned char c1 = checksum(path1);
	unsigned char c2 = checksum(path2);
	if (c1 == 0 || c2 == 0) return false;

	return c1 == c2;
}

bool cmpname(const char *path1, const char *path2)
{
	const char *name1 = strrchr(path1, '/');
	const char *name2 = strrchr(path2, '/');

	if (!name1 || !name2) return false;

	return strcmp(name1+1, name2+1) == 0;
}
