#include "config.h"

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "items.h"
#include "compares.h"
#include "selection.h"

/* internal grow helper */
static void sel_grow(Selection *s)
{
	s->capacity *= 2;
	s->paths = realloc(s->paths, sizeof(char*) * s->capacity);
}

/* public API */

void sel_init(Selection *s)
{
	s->count = 0;
	s->capacity = 8;
	s->paths = malloc(sizeof(char*) * s->capacity);
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

void sel_add_dir(Selection *s, const char *path)
{
	Items items = load_dir(path); // your existing function to list directory
	if (items.count == 0 || items.arr == NULL) return;

	for (int i=0; i<items.count; i++)
	{
		Item *cur = items.arr[i];
		if (!cur || !cur->name) continue;

		char fullpath[PATH_MAX_LEN];
		snprintf(fullpath, sizeof(fullpath), "%s/%s", path, cur->name);

		sel_add(s, fullpath);

		// If it is a directory, recurse
		if (cur->is_dir)
			sel_add_dir(s, fullpath);
	}
	free_dir(&items); // free the Items array
}

void sel_add_all(Selection *s, const Items items, const char *path)
{
	if (items.count == 0 || items.arr == NULL) return;

	for (int i=0; i<items.count; ++i)
	{
		Item *cur = items.arr[i];
		if (!cur || !cur->name) continue;

		char fullpath[PATH_MAX_LEN];
		snprintf(fullpath, sizeof(fullpath), "%s/%s", path, cur->name);

		if (!sel_contains(s, fullpath))
			sel_add(s, fullpath);

		if (cur->is_dir)
			sel_add_dir(s, fullpath);
    	}
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

void sel_remove_dir(Selection *s, const char *path)
{
	Items items = load_dir(path);
	if (items.count == 0 || items.arr == NULL) return;

	for (int i = 0; i < items.count; i++)
	{
		Item *cur = items.arr[i];
		if (!cur || !cur->name) continue;

		char fullpath[PATH_MAX_LEN];
		snprintf(fullpath, sizeof(fullpath), "%s/%s", path, cur->name);

		sel_remove(s, fullpath);

		if (cur->is_dir)
		sel_remove_dir(s, fullpath);
	}

	free_dir(&items);
}

void sel_remove_all(Selection *s)
{
	if (!s || s->count == 0) return;

	for (int i=0; i<s->count; ++i)
		free(s->paths[i]);

	s->count = 0;
}

static const char* to_relative(const char *cwd, const char *fullpath)
{
	size_t cwd_len = strlen(cwd);
	if (strncmp(fullpath, cwd, cwd_len) == 0) {
		// Skip cwd + '/'
		if (fullpath[cwd_len] == '/')
			return fullpath + cwd_len + 1;
		return fullpath + cwd_len;
	}
	return fullpath; // fallback
}

void sel_find_duplicates(Selection *s, const char *cwd, Selection *same_name, Selection *same_data)
{
	sel_init(same_name);
	sel_init(same_data);

	if(!s || s->count < 2) return;

	for (int i=0; i<s->count; ++i)
	{
		for (int j=i+1; j<s->count; ++j)
		{
			const char *r1 = to_relative(cwd, s->paths[i]);
			const char *r2 = to_relative(cwd, s->paths[j]);

			char buf[PATH_MAX_LEN * 2 + 16];
			snprintf(buf, sizeof(buf), "%s  <-->  %s", r1, r2);

			if (cmpname(s->paths[i], s->paths[j])) {
				sel_add(same_name, buf);
			}
			else if (cmpdata(s->paths[i], s->paths[j])) {
				sel_add(same_data, buf);
			}
		}
	}
}
