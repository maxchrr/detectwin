#include <stdio.h>
#include <stdlib.h>

#include <string.h>

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
