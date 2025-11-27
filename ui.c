<<<<<<< HEAD
#include "config.h"

=======
>>>>>>> 794891d (Merge branch)
#include <string.h>

#include <ncurses.h>

#include "ui.h"

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
<<<<<<< HEAD
	init_pair(1, COLOR_RED, -1);	// title
	init_pair(2, COLOR_CYAN, -1);   // directories
	init_pair(3, COLOR_BLACK, -1);  // files (unused explicitly)
	init_pair(4, COLOR_YELLOW, -1); // cursor
	init_pair(5, COLOR_GREEN, -1);	// bar
=======
	init_pair(1, COLOR_CYAN, -1);   // directories
	init_pair(2, COLOR_BLACK, -1);  // files (unused explicitly)
	init_pair(3, COLOR_YELLOW, -1); // cursor
<<<<<<< HEAD
	init_pair(4, COLOR_GREEN, -1); // bar
>>>>>>> 794891d (Merge branch)
=======
	init_pair(4, COLOR_GREEN, -1);	// bar
>>>>>>> 2aec38a (Update)

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

	/* Layout constants */
	const int HEADER_ROWS = 4;   // rows 0..3 reserved for header
	const int GAP_ROWS    = 1;   // empty line between list and footer
	const int FOOTER_ROWS = 1;   // bottom bar at last row

	/* Header (centered title + cwd) */
	attron(A_BOLD | A_STANDOUT | COLOR_PAIR(1));
	char* title = "Detectwin";
	int tx = (screen_cols - (int)strlen(title)) / 2;
	if (tx < 0) tx = 0;
	mvprintw(0, tx, "%s", title);
	attroff(A_STANDOUT | COLOR_PAIR(1));
	mvprintw(2, 1, "Current directory: %s", cwd);
	attroff(A_BOLD);

	/* Visible list area (leaves one blank line before footer) */
	int visible = screen_rows - HEADER_ROWS - GAP_ROWS - FOOTER_ROWS;
	if (visible < 1) visible = 1;

	/* Keep cursor visible: adjust ui_scroll */
	if (cursor < ui_scroll)
		ui_scroll = cursor;
	if (cursor >= ui_scroll + visible)
		ui_scroll = cursor - visible + 1;

	/* Clamp ui_scroll into valid range */
	int max_scroll = items.count - visible;
	if (max_scroll < 0)
		max_scroll = 0;
	if (ui_scroll < 0)
		ui_scroll = 0;
	if (ui_scroll > max_scroll)
		ui_scroll = max_scroll;

	/* Draw list starting at HEADER_ROWS */
	for (int i=ui_scroll; i<items.count && i<ui_scroll+visible; ++i)
	{
		Item *it = items.arr[i];
<<<<<<< HEAD
		int row = i - ui_scroll + HEADER_ROWS; /* first list row is HEADER_ROWS */
=======
		int row = i - ui_scroll + 2;
>>>>>>> 2aec38a (Update)

		char fullpath[PATH_MAX_LEN];
		snprintf(fullpath, sizeof(fullpath), "%s/%s", cwd, it->name);
		char mark = sel_contains(sel, fullpath) ? '*' : ' ';

		if (i == cursor)
			attron(COLOR_PAIR(4) | A_BOLD | A_REVERSE);

		if (it->is_dir)
			attron(COLOR_PAIR(2));

<<<<<<< HEAD
<<<<<<< HEAD
=======
		int row = i-ui_scroll+2;
>>>>>>> 794891d (Merge branch)
=======
>>>>>>> 2aec38a (Update)
		mvprintw(row, 2, "[%c] %s%s", mark, it->name, it->is_dir ? "/" : "");

		if (it->is_dir)
			attroff(COLOR_PAIR(2));

		if (i == cursor)
			attroff(COLOR_PAIR(4) | A_BOLD | A_REVERSE);
	}

<<<<<<< HEAD
	/* Explicitly clear the gap row so it's visually empty */
	int gap_row = HEADER_ROWS + visible;
	if (gap_row >= 0 && gap_row < screen_rows - FOOTER_ROWS)
	{
		/* Clear the gap row */
		mvhline(gap_row, 0, ' ', screen_cols);

		/* Scroll hint in the gap row */
		const char *hint = "Use <up> <down> or mouse wheel to scroll";
		int hx = (screen_cols - (int)strlen(hint)) / 2;
		if (hx < 0) hx = 0;
		mvprintw(gap_row, hx, "%s", hint);
	}

	/* Bottom bar (footer) */
	attron(COLOR_PAIR(5));
	int footer_row = screen_rows - 1;
	mvprintw(footer_row, 0, "Please select all files to be parsed. Currently %d files are selected.", sel->count);
	attroff(COLOR_PAIR(5));

	refresh();
}

void show_popup(const char *msg)
{
	int rows, cols;
	getmaxyx(stdscr, rows, cols);

	int ph = 5;			// popup height
	int pw = strlen(msg) + 6;	// popup width

	int y = (rows - ph) / 2;
	int x = (cols - pw) / 2;

	WINDOW *popup = newwin(ph, pw, y, x);
	box(popup, 0, 0);

	mvwprintw(popup, 2, 3, "%s", msg);
	wrefresh(popup);

	wgetch(popup); // attendre une touche

	delwin(popup);
	touchwin(stdscr);
=======
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

>>>>>>> 794891d (Merge branch)
	refresh();
}

bool ui_handle_mouse(int *cursor, Items items, MEVENT *ev)
{
	if (!cursor) return false;
<<<<<<< HEAD
=======
	int list_start = 1;
>>>>>>> 2aec38a (Update)

	int rows, cols;
	getmaxyx(stdscr, rows, cols);

	const int HEADER_ROWS = 4;   // must match draw()
	const int GAP_ROWS    = 1;   // must match draw()
	const int FOOTER_ROWS = 1;

	int list_start = HEADER_ROWS;
	int visible = rows - HEADER_ROWS - GAP_ROWS - FOOTER_ROWS;
	if (visible < 1) visible = 1;

	if (ev->y >= list_start && ev->y < list_start+visible)
	{
		int idx = ui_scroll + (ev->y - list_start);
		if (idx >= 0 && idx < items.count)
		{
			*cursor = idx;
			return true; // cursor updated
		}
	}
	
	return false;
}
