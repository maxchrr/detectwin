#ifndef UI_H
#define UI_H

#include <stdbool.h>

#include "items.h"
#include "selection.h"

extern int ui_scroll;

void ui_init(void);
void ui_end(void);

/* draw shows the current directory listing (uses Selection to mark global selection) */
<<<<<<< HEAD
void draw(int cursor, char *cwd, Items items, Selection *sel);
=======
<<<<<<< HEAD
void draw(int cursor, char *cwd, Items items, Selection *sel);
void show_popup(const char *msg);
=======
void draw(int cursor, char *cwd, Items items, Selection *sel, bool status);
>>>>>>> 794891d (Merge branch)
>>>>>>> 5a3730fd3636811137e6c01d773d1d320d7c9315

/* return 1 if mouse handled (updates cursor), 0 otherwise */
bool ui_handle_mouse(int *cursor, Items items, MEVENT *ev);

#endif
