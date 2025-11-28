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
<<<<<<< HEAD
void sel_remove(Selection *s, const char *path);
bool sel_contains(Selection *s, const char *path);
=======
void sel_add_dir(Selection *sel, const char *path);
void sel_remove(Selection *s, const char *path);
void sel_remove_dir(Selection *sel, const char *path);
bool sel_contains(Selection *s, const char *path);
bool sel_is_duplicated(Selection *s);
>>>>>>> 5a3730fd3636811137e6c01d773d1d320d7c9315

#endif
