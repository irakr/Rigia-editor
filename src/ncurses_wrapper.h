/*
 * ncurses_wrapper.h
 *
 * This file contains wrapper function for the ncurses library.
 */

#include <ncurses.h>

// Wrapper for newwin()
WINDOW *create_newwin(int height, int width, int starty, int startx);

// Wrapper for delwin()
void destroy_win(WINDOW*);

// A wrapper for the move() function of the ncurses library.
void Move(int y, int x);
