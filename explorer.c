#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <unistd.h>

#include <ncurses.h>

#include <assert.h>

#include "compares.h"
#include "items.h"
#include "selection.h"

#include "ui.h"

#define PATH_MAX_LEN 1024

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

	int ch;
	while (1)
	{
		draw(cursor, cwd, items, &sel);
		Item *it = items.count > 0 ? items.arr[cursor] : NULL;
		ch = getch();
		switch (ch)
		{
		case KEY_MOUSE:
			if (getmouse(&mev) == OK && mev.bstate & BUTTON1_CLICKED)
				ui_handle_mouse(&cursor, items, &mev);
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

		case 'x':
		{
			bool duplicated = sel_is_duplicated(&sel);
			if (duplicated)
				show_popup("Duplicate files !");
			else
				show_popup("No duplicated detected.");
			break;
		}

		case ' ':  // Toggle selected mark
			if (it)
			{
				char fullpath[PATH_MAX_LEN];
				snprintf(fullpath, sizeof(fullpath), "%s/%s", cwd, it->name);
				
				if (sel_contains(&sel, fullpath))
					sel_remove(&sel, fullpath);
				else
					sel_add(&sel, fullpath);
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
