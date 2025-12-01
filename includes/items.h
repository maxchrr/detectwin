#ifndef ITEMS_H
#define ITEMS_H

// Abstract type for file or directory
typedef struct
{
	char *name;
	int is_dir;
} Item;

// Dynamic array
typedef struct
{
	Item** arr;
	int count;
} Items;

Items load_dir(const char *path);
void free_dir(Items *items);

#endif
