#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "items.h"
#include "compares.h"
#include "selection.h"

#define PATH_MAX_LEN 1024

/* internal grow helper */
static void sel_grow(Selection *s)
{
	s->capacity *= 2;
	s->paths = realloc(s->paths, sizeof(char*) * s->capacity);
}

/* public API */

void sel_init(Selection *s)
{
	s->paths = malloc(sizeof(char*) * s->capacity);
	s->count = 0;
	s->capacity = 8;
}

void sel_free(Selection *s)
{
	if (!s) return;
	for (int i=0; i<s->count; ++i)
		free(s->paths[i]);
	free(s->paths);
	s->paths = NULL;
	s->count = 0;
	s->capacity = 0;
}

bool sel_contains(Selection *s, const char *path)
{
	if (!s || !path) return false;
	for (int i=0; i<s->count; ++i)
		if (strcmp(s->paths[i], path) == 0)
			return true;
	return false;
}

void sel_add(Selection *s, const char *path)
{
	if (!s || !path) return;
	if (sel_contains(s, path)) return;

	if (s->count >= s->capacity)
		sel_grow(s);

	s->paths[s->count++] = strdup(path);
}

void sel_add_dir(Selection *sel, const char *path)
{
	Items items = load_dir(path); // your existing function to list directory
	for (int i = 0; i < items.count; i++)
	{
		Item *it = items.arr[i];
		char fullpath[PATH_MAX_LEN];
		snprintf(fullpath, sizeof(fullpath), "%s/%s", path, it->name);

		sel_add(sel, fullpath);

		// If it is a directory, recurse
		if (it->is_dir)
			sel_add_dir(sel, fullpath);
	}
	free_dir(&items); // free the Items array
}

void sel_remove(Selection *s, const char *path)
{
	if (!s || !path) return;
	for (int i=0; i<s->count; ++i)
	{
		if (strcmp(s->paths[i], path) == 0)
		{
			free(s->paths[i]);
			s->paths[i] = s->paths[s->count - 1];
			s->count--;
			return;
		}
	}
}

void sel_remove_dir(Selection *sel, const char *path)
{
	Items items = load_dir(path);
	for (int i = 0; i < items.count; i++)
	{
		Item *it = items.arr[i];
		char fullpath[PATH_MAX_LEN];
		snprintf(fullpath, sizeof(fullpath), "%s/%s", path, it->name);

		sel_remove(sel, fullpath);

		if (it->is_dir)
		sel_remove_dir(sel, fullpath);
	}
	free_dir(&items);
}

bool sel_is_duplicated(Selection *s)
{
	if(!s || s->count < 2) return false;

	for (int i=0; i<s->count; ++i)
	{
		for (int j=i+1; j<s->count; ++j)
		{
			if (cmpname(s->paths[i], s->paths[j]))
				return true;
			else if (cmpdata(s->paths[i], s->paths[j]))
				return true;
		}
	}
	return false;
}
