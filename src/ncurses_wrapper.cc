/*
 * ncurses_wrapper.cc
 *
 * This file contains wrapper function for the ncurses library.
 */

#include "ncurses_wrapper.h"

WINDOW *create_newwin(int height, int width, int starty, int startx) {
	WINDOW *local_win;
	local_win = newwin(height, width, starty, startx);
	box(local_win, 0, 0);
	wrefresh(local_win);
	return local_win;
}

void destroy_win(WINDOW *local_win) {
	wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	wrefresh(local_win);
	delwin(local_win);
}

inline void Move(int y, int x) {
	move(y, x);
}
