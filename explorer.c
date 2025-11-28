<<<<<<< HEAD
=======
#include "config.h"

>>>>>>> 5a3730fd3636811137e6c01d773d1d320d7c9315
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <unistd.h>

#include <ncurses.h>

#include <assert.h>

<<<<<<< HEAD
=======
#include "compares.h"
>>>>>>> 5a3730fd3636811137e6c01d773d1d320d7c9315
#include "items.h"
#include "selection.h"

#include "ui.h"

<<<<<<< HEAD
#define PATH_MAX_LEN 1024

=======
>>>>>>> 5a3730fd3636811137e6c01d773d1d320d7c9315
static void usage(const char *exe)
{
	fprintf(stderr, "Usage : %s [directory]\n", exe);
	fprintf(stderr, "If no argument is provided, default directory is parsed\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char* argv[])
{
	Items items;
	char cwd[PATH_MAX_LEN];
	if (argc > 2) usage(argv[0]);
	/* Read given directory if provided,
	 * else, fallback to current directory.
	 */
	if (argc == 2)
	{
		chdir(argv[1]);
		getcwd(cwd,sizeof(cwd));
		items = load_dir(cwd);
	}
	else
	{
		getcwd(cwd,sizeof(cwd));
		items = load_dir(cwd);
	}

	int cursor = 0;

	Selection sel;
	sel_init(&sel);

	ui_init();
	MEVENT mev;

<<<<<<< HEAD
	int ch;
	while (1)
	{
		draw(cursor, cwd, items, &sel);
=======
	bool status = false;

	int ch;
	while (1)
	{
		draw(cursor, cwd, items, &sel, status);
>>>>>>> 5a3730fd3636811137e6c01d773d1d320d7c9315
		Item *it = items.count > 0 ? items.arr[cursor] : NULL;
		ch = getch();
		switch (ch)
		{
		case KEY_MOUSE:
<<<<<<< HEAD
			if (getmouse(&mev) == OK && mev.bstate & BUTTON1_CLICKED)
				ui_handle_mouse(&cursor, items, &mev);
=======
			if (getmouse(&mev) == OK)
			{
				/* Click selection */
				if (mev.bstate & BUTTON1_CLICKED)
					ui_handle_mouse(&cursor, items, &mev);
			}
>>>>>>> 5a3730fd3636811137e6c01d773d1d320d7c9315
			break;

		case 'q':  // Quit
			sel_free(&sel);
			free_dir(&items);
			ui_end();
			return 0;

		case 'h':
			

		case KEY_UP:
			if (items.count > 0)
				cursor = (cursor - 1 + items.count) % items.count;
			break;

		case KEY_DOWN:
			if (items.count > 0)
				cursor = (cursor + 1) % items.count;
			break;

<<<<<<< HEAD
=======
		case 'x':
<<<<<<< HEAD
		{
			bool duplicated = sel_is_duplicated(&sel);
			if (duplicated)
				show_popup("Duplicate files !");
			else
				show_popup("No duplicated detected.");
			break;
		}
=======
			if (sel.count >= 2)
			{
				if (sel_is_duplicated(&sel))
					status = true;
				else
					status = false;
			}
			status = false;
			break;
>>>>>>> 794891d (Merge branch)

>>>>>>> 5a3730fd3636811137e6c01d773d1d320d7c9315
		case ' ':  // Toggle selected mark
			if (it)
			{
				char fullpath[PATH_MAX_LEN];
				snprintf(fullpath, sizeof(fullpath), "%s/%s", cwd, it->name);
				
				if (sel_contains(&sel, fullpath))
<<<<<<< HEAD
					sel_remove(&sel, fullpath);
				else
					sel_add(&sel, fullpath);
=======
				{
					sel_remove(&sel, fullpath);

					if (it->is_dir)
						sel_remove_dir(&sel, fullpath);
				}
				else
				{
					sel_add(&sel, fullpath);

					if (it->is_dir)
						sel_add_dir(&sel, fullpath);
				}
>>>>>>> 5a3730fd3636811137e6c01d773d1d320d7c9315
			}
			break;

		case '\n':  // Enter directory
			if (it && it->is_dir)
			{
				char next[PATH_MAX_LEN];
				snprintf(next, sizeof(next), "%s/%s", cwd, it->name);

				if (chdir(next) == 0)
				{
					if (!getcwd(cwd, sizeof(cwd)))
						strncpy(cwd, next, sizeof(cwd)-1);
					free_dir(&items);
					items = load_dir(cwd);
					cursor = 0;
				}
			}
			break;

		case KEY_BACKSPACE:
		case 127:  // Go up directory
			if (chdir("..") == 0)
			{
				if (!getcwd(cwd, sizeof(cwd)))
					strncpy(cwd, "..", sizeof(cwd)-1);
				free_dir(&items);
				items = load_dir(cwd);
				cursor = 0;
			}
			break;
		}
	}

	sel_free(&sel);
	free_dir(&items);
	ui_end();
	return 0;
}
