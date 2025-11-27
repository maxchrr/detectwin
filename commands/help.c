#include <ncurses.h>

void help()
{
	printw("r : Rename the selected file.\n\n");
	printw("i : Gives additionnal information about the selected file.\n\n");
	printw("h : Help providing informations about every different imput.\n\n");
	printw("s : Search duplicates that matches with filename.\n\n");
	printw("a : Select all the files in a repository if one is selected, every visible file otherwise.\n\n");
	printw("d : Delete the selected file, asking confirmation when doing so.");
	printw("x : Execute first loop to detect duplicates.");
	printw("m : Move the selected file in a selected repository.");
}
