#include <string.h>

#include <ncurses.h>

#include "ui.h"

#define PATH_MAX_LEN 1024

int ui_scroll = 0;
static int screen_rows = 24, screen_cols = 80;

void ui_init(void)
{
	initscr();
	noecho();
	//cbreak();
	curs_set(0);
	keypad(stdscr, TRUE);
	mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
	mouseinterval(150);
	start_color();
	use_default_colors();

	/* color pairs */
	init_pair(1, COLOR_CYAN, -1);   // directories
	init_pair(2, COLOR_BLACK, -1);  // files (unused explicitly)
	init_pair(3, COLOR_YELLOW, -1); // cursor
	init_pair(4, COLOR_GREEN, -1); // bar

	getmaxyx(stdscr, screen_rows, screen_cols);
}

void ui_end(void)
{
    endwin();
}

void draw(int cursor, char *cwd, Items items, Selection *sel, bool status)
{
	clear();

	getmaxyx(stdscr, screen_rows, screen_cols);

	// Header
	attron(A_BOLD);
	mvprintw(0, 0, "Directory: %s", cwd);
	attroff(A_BOLD);

	int visible = screen_rows;
	if (visible < 1) visible = 1;

	if (cursor < ui_scroll) ui_scroll = cursor;
	if (cursor >= ui_scroll + visible) ui_scroll = cursor - visible + 1;

	// List
	for (int i=ui_scroll; i<items.count && i<ui_scroll+visible; ++i)
	{
		Item *it = items.arr[i];
		int row = i - ui_scroll + 1;

		char fullpath[PATH_MAX_LEN];
		snprintf(fullpath, sizeof(fullpath), "%s/%s", cwd, it->name);
		char mark = sel_contains(sel, fullpath) ? '*' : ' ';

		if (i == cursor)
			attron(COLOR_PAIR(3) | A_BOLD | A_REVERSE);

		if (it->is_dir)
			attron(COLOR_PAIR(1));

		int row = i-ui_scroll+2;
		mvprintw(row, 2, "[%c] %s%s", mark, it->name, it->is_dir ? "/" : "");

		if (it->is_dir)
			attroff(COLOR_PAIR(1));

		if (i == cursor)
			attroff(COLOR_PAIR(3) | A_BOLD | A_REVERSE);
	}

	if (status)
		mvprintw(screen_rows - 2, 0, "found");
	else
		mvprintw(screen_rows - 2, 0, "not found");
	int offset = 0;
	for (int i=0; i<sel->count; ++i)
	{ 
		char* name = strrchr(sel->paths[i], '/');
		mvprintw(screen_rows - 1, i+offset, "%s", name+1);
		offset += strlen(name+1);
	}

	refresh();
}

bool ui_handle_mouse(int *cursor, Items items, MEVENT *ev)
{
	if (!cursor) return false;
	int list_start = 2;
	int list_end = list_start+items.count-1;

	int rows, cols;
	getmaxyx(stdscr, rows, cols);

	int visible = rows-3;
	if (visible<1) visible = 1;

	if (ev->y >= list_start && ev->y < list_start+visible)
	{
		int idx = ui_scroll+(ev->y-list_start);
		if (idx >= 0 && idx < items.count)
		{
			*cursor = idx;
			return true; // cursor updated
		}
	}
	
	return false;
}
