#ifndef SELECTION_H
#define SELECTION_H

#include <stdbool.h>

typedef struct {
	char **paths;  // absolute paths of selected items
	int count;
	int capacity;
} Selection;

void sel_init(Selection *s);
void sel_free(Selection *s);
void sel_add(Selection *s, const char *path);
void sel_add_dir(Selection *s, const char *path);
void sel_add_all(Selection *s, const Items items, const char *path);
void sel_remove(Selection *s, const char *path);
void sel_remove_dir(Selection *s, const char *path);
void sel_remove_all(Selection *s);
bool sel_contains(Selection *s, const char *path);
void sel_find_duplicates(Selection *s, const char *cwd, Selection *same_name, Selection *same_data);

#endif
