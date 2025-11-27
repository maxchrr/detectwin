#include <stdio.h>
#include <stdlib.h>

#include <dirent.h>
#include <string.h>

#include <sys/stat.h>

#include "items.h"

/* --- Sorting function -------------------------------------------------- */

static int compare_items(const void *a, const void *b)
{
	const Item *ia = *(const Item **)a;
	const Item *ib = *(const Item **)b;

	// Directories first
	if (ia->is_dir != ib->is_dir)
		return ib->is_dir - ia->is_dir;   // dirs (1) come before files (0)

	// Alphabetical (case insensitive)
	return strcasecmp(ia->name, ib->name);
}

/* --- Load directory ---------------------------------------------------- */

Items load_dir(const char *path)
{
	Items items;
	items.arr = NULL;
	items.count = 0;

	DIR *dir = opendir(path);
	if (!dir) return items;

	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL)
	{
		// Skip "." and ".."
		if (strcmp(entry->d_name, ".") == 0 ||
		    strcmp(entry->d_name, "..") == 0)
		    continue;

		/* allocate new item */
		Item* it = malloc(sizeof(Item));
		it->name = strdup(entry->d_name);
		it->is_dir = (entry->d_type == DT_DIR);

		/* append */
		items.arr = realloc(items.arr, sizeof(Item*) * (items.count+1));
		items.arr[items.count++] = it;
	}

	closedir(dir);

	if (items.count > 0)
		qsort(items.arr, items.count, sizeof(Item*), compare_items);
	return items;
}

/* --- Free directory ----------------------------------------------------- */

void free_dir(Items *items)
{
	if (!items) return;
	for (int i=0; i<items->count; ++i)
	{
		free(items->arr[i]->name);
		free(items->arr[i]);
	}
	free(items->arr);
	items->arr = NULL;
	items->count = 0;
}
