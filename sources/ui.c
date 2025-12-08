#include "config.h"

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
	init_pair(1, COLOR_RED, -1);	// title
	init_pair(2, COLOR_CYAN, -1);   // directories
	init_pair(3, COLOR_BLACK, -1);  // files (unused explicitly)
	init_pair(4, COLOR_YELLOW, -1); // cursor
	init_pair(5, COLOR_GREEN, -1);	// bar

	getmaxyx(stdscr, screen_rows, screen_cols);
}

void ui_end(void)
{
    endwin();
}

void draw(int cursor, char *cwd, Items items, Selection *sel)
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
		int row = i - ui_scroll + HEADER_ROWS; /* first list row is HEADER_ROWS */

		char fullpath[PATH_MAX_LEN];
		snprintf(fullpath, sizeof(fullpath), "%s/%s", cwd, it->name);
		char mark = sel_contains(sel, fullpath) ? '*' : ' ';

		if (i == cursor)
			attron(COLOR_PAIR(4) | A_BOLD | A_REVERSE);

		if (it->is_dir)
			attron(COLOR_PAIR(2));

		mvprintw(row, 2, "[%c] %s%s", mark, it->name, it->is_dir ? "/" : "");

		if (it->is_dir)
			attroff(COLOR_PAIR(2));

		if (i == cursor)
			attroff(COLOR_PAIR(4) | A_BOLD | A_REVERSE);
	}

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
	refresh();
}

#define MIN_POPUP_WIDTH 30
#define POPUP_MARGIN 4

void display_duplicates(const Selection *same_name, const Selection* same_data)
{
	/* Safety: pointers must be valid */
	if (!same_name || !same_data) {
		show_popup("No duplicates found.");
		return;
	}

	/* If nothing to show */
	if (same_name->count == 0 && same_data->count == 0) {
		show_popup("No duplicates found.");
		return;
	}

	int rows, cols;
	getmaxyx(stdscr, rows, cols);

	/* Compute desired height: title + spacing + each section + margins */
	int desired_lines = 1         /* main title */
	                  + 1         /* blank */
	                  + 1         /* [Same Name] title */
	                  + (same_name->count ? same_name->count : 1) /* at least one line "None" */
	                  + 1         /* blank */
	                  + 1         /* [Same Data] title */
	                  + (same_data->count ? same_data->count : 1) /* at least one line "None" */
	                  + 2;        /* padding/border */

	/* Clamp height to terminal rows - keep at least 3 rows for border/title */
	int ph = desired_lines;
	if (ph > rows - 2)
		ph = rows - 2;
	if (ph < 6)
		ph = 6;

	/* Compute width: longest line in both lists (+margins), clamp to terminal width */
	int pw = MIN_POPUP_WIDTH;
	for (int i = 0; i < same_name->count; ++i) {
		int len = (int)strlen(same_name->paths[i]);
		if (len + POPUP_MARGIN > pw) pw = len + POPUP_MARGIN;
	}
	for (int i = 0; i < same_data->count; ++i) {
		int len = (int)strlen(same_data->paths[i]);
		if (len + POPUP_MARGIN > pw) pw = len + POPUP_MARGIN;
	}
	/* ensure width doesn't exceed terminal minus 2 for borders */
	if (pw > cols - 4) pw = cols - 4;
	if (pw < MIN_POPUP_WIDTH) pw = MIN_POPUP_WIDTH;

	int y = (rows - ph) / 2;
	int x = (cols - pw) / 2;

	/* Create window and check */
	WINDOW *popup = newwin(ph, pw, y, x);
	if (!popup) {
		/* Fallback: simple message */
		show_popup("Unable to open popup (terminal too small).");
		return;
	}
	box(popup, 0, 0);

	int row = 1;
	/* Title */
	wattron(popup, A_BOLD | A_UNDERLINE);
	mvwprintw(popup, row++, 2, "Duplicate Files");
	wattroff(popup, A_BOLD | A_UNDERLINE);
	row++; /* blank */

	/* Section: Same Name */
	wattron(popup, A_BOLD);
	mvwprintw(popup, row++, 2, "[Same Name]");
	wattroff(popup, A_BOLD);

	int lines_used = row;
	int available_inner = ph - 3; /* rows usable for lines inside popup (approx) */

	if (same_name->count == 0) {
		/* print "None" but clipped to window width-4 */
		int maxlen = pw - 4;
		mvwprintw(popup, row++, 4, "%.*s", maxlen, "None");
	} else {
		/* Print as many lines as fit; if not all fit, add indication */
		for (int i = 0; i < same_name->count; ++i) {
			if (row >= ph - 2) break; /* keep last line for margin */
			int maxlen = pw - 6; /* left margin inside popup */
			mvwprintw(popup, row++, 4, "%.*s", maxlen, same_name->paths[i]);
		}
		if (same_name->count > 0 && row >= ph - 2 && (same_name->count + same_data->count) > (ph - 6)) {
			/* not enough space in whole popup, indicate truncation */
			mvwprintw(popup, row++, 4, "... (output truncated)");
		}
	}

	row++; /* blank between sections if space allows */

	/* Section: Same Data */
	if (row < ph - 1) {
		wattron(popup, A_BOLD);
		mvwprintw(popup, row++, 2, "[Same Data]");
		wattroff(popup, A_BOLD);
	}

	if (same_data->count == 0) {
		if (row < ph - 1)
			mvwprintw(popup, row++, 4, "%.*s", pw - 4, "None");
	} else {
		for (int i = 0; i < same_data->count; ++i) {
			if (row >= ph - 1) break;
			mvwprintw(popup, row++, 4, "%.*s", pw - 6, same_data->paths[i]);
		}
		if (same_data->count > 0 && row >= ph - 1 && (same_data->count > (ph - 6))) {
			/* indicate truncation if needed */
			if (row < ph - 1)
				mvwprintw(popup, row++, 4, "... (truncated)");
		}
	}

	/* Footer hint if truncated: we could also show counts */
	if (row >= ph - 1) {
		/* nothing extra */
	}

	wrefresh(popup);
	wgetch(popup);

	delwin(popup);
	touchwin(stdscr);
	refresh();
}

void display_help(void)
{
	/*WINDOW *popup = newwin(0, 0, screen_rows, screen_cols);
	box(popup, 0, 0);

	mvprintw(popup, 0, 1, "%s", "r : Rename the selected file.");
	mvprintw(popup, 1, 1, "%s", "i : Gives additionnal information about the selected file.");
	mvprintw(popup, 2, 1, "%s", "h : Help providing informations about every different imput.");
	mvprintw(popup, 3, 1, "%s", "s : Search duplicates that matches with filename.");
	mvprintw(popup, 4, 1, "%s", "a : Select all the files in a repository if one is selected, every visible file otherwise.");
	mvprintw(popup, 5, 1, "%s", "d : Delete the selected file, asking confirmation when doing so.");
	mvprintw(popup, 6, 1, "%s", "x : Execute first loop to detect duplicates.");
	mvprintw(popup, 7, 1, "%s", "m : Move the selected file in a selected repository.");
	wrefresh(popup);

	wgetch(popup); // attendre une touche

	delwin(popup);
	touchwin(stdscr);

	refresh();*/
}

bool ui_handle_mouse(int *cursor, Items items, MEVENT *ev)
{
	if (!cursor) return false;

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
