/*
 * This is free and unencumbered software released into the public domain.
 * 
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 * 
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 * 
 * For more information, please refer to <http://unlicense.org>
 */

/*
 * editor.cc
 *
 * This module is the main station of the program.
 * It provides the user interface environment and connects all other modules together.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>
//#include <fcntl.h>
//#include <endian.h>
#include "editor.h"

extern Editor editor;

// The KEY_ENTER macro from 'ncurses.h' was not working correctly so I replaced it manually.
#if defined(KEY_ENTER)
#undef KEY_ENTER
#endif
#define KEY_ENTER 10

/********************************************* VI Mode ***************************************/

void ViMode :: init_interface() {
	//initscr();
	cbreak();
	//keypad(stdscr, TRUE);
	noecho();
	window = newwin(LINES, COLS, 0, 0);
	keypad(window, TRUE);
	
	//Take the cursor position the inputmode
	cursor_position.x = InputMode_.cursor_position.x;
	cursor_position.y = InputMode_.cursor_position.y;
	wmove(window, cursor_position.y, cursor_position.x);
	wrefresh(window);
}

void ViMode :: end_interface() {
	//destroy_win(window);
	//endwin();
}

void ViMode :: rest() {
	//printf("Resting in %s\n", "Vi-Mode");
}

// Execution of the mode
void ViMode :: run() {
	//move(2, 0);
	//printw("Current mode: %s", modename_);
	//refresh();
	init_interface();
	while(1) {
		int c = editor.io_input()->read_key(window);
		
		switch (c) {
			case ESC:
				end_interface();
				throw (ModeSwitchException("[ModeSwitchException] Request for mode switch", "vi-mode"));
			case ':':
				end_interface();
				throw (ModeSwitchException("[ModeSwitchException] Request for mode switch", "command-mode"));
			case 'i':
				end_interface();
				throw (ModeSwitchException("[ModeSwitchException] Request for mode switch", "input-mode"));
		}
		
	}
	
	//end_interface();
}

/*************************************** Command Mode **************************************/

void CommandMode :: init_interface() {
	//initscr();
	cbreak();
	keypad(stdscr, TRUE);
	echo();
	window = newwin(1, COLS, LINES-1, 0);
	keypad(window, TRUE);
	//wmove(window, 0, 0);
	wprintw(window, "%c", editor.io_input_->buffer()->buff_out());
	wrefresh(window);
}

void CommandMode :: end_interface() {
	//destroy_win(window);
	//endwin();
	cursor_position.x = cursor_position.y = 0;
}

void CommandMode :: rest() {
	//printf("Resting in %s\n", "Command-Mode");
}

// Execution of the mode
void CommandMode :: run() {
	//move(2, 0);
	//printw("Current mode: %s", modename_);
	//refresh();
	init_interface();
	while(1) {
		int c = editor.io_input()->read_key(window);
		switch (c) {
			case ESC:
				end_interface();
				throw (ModeSwitchException("[ModeSwitchException] Request for mode switch", "vi-mode"));
			
			case KEY_ENTER:
				run_command();
				break;
		}
		
		cursor_position.x++;
	}
}

void CommandMode :: run_command() {
	//TODO...
}

/*************************************** Input Mode ****************************************/

void InputMode :: init_interface() {
	//initscr();
	cbreak();
	//raw();
	keypad(stdscr, TRUE);
	echo();
	//window = create_newwin(LINES, COLS, 0, 0);
	window = ViMode_.window;
	wmove(window, ViMode_.cursor_position.y, ViMode_.cursor_position.x);
	wrefresh(window);
}

void InputMode :: end_interface() {
	//destroy_win(window);
	//endwin();
}

void InputMode :: rest() {
	//printf("Resting in %s\n", "Input-Mode");
}

// Execution of the mode
void InputMode :: run() {
	//move(2, 0);
	//printw("Current mode: %s", modename_);
	int c;
	init_interface();
	while((c = editor.io_input()->read_key(window)) != ESC) {
		
		internal_buffer_.buff_it(c);
		
		//Different operations for different characters...
		switch (c) {
			case KEY_ENTER:
				cursor_position.y++;
				cursor_position.x = 0;
				final_cursor_position = cursor_position;
				while(wmove(window, cursor_position.y, cursor_position.x) == ERR) {
					//Then probably resize the window...TODO
					;
				}
				break;
			
			case KEY_LEFT:
				if(wmove(window, cursor_position.y, cursor_position.x-1) != ERR)
					cursor_position.x--;
				break;
			case KEY_RIGHT: //Block-able by final_cursor_position
				cursor_position.x = (cursor_position.x < final_cursor_position.x) ? (cursor_position.x+1) : (cursor_position.x) ;
				wmove(window, cursor_position.y, cursor_position.x);
				break;
			case KEY_UP:
				if(wmove(window, cursor_position.y-1, cursor_position.x) != ERR)
					cursor_position.y--;
				break;
			case KEY_DOWN: //Block-able by final_cursor_position
				cursor_position.y = (cursor_position.y < final_cursor_position.y) ? (cursor_position.y+1) : (cursor_position.y) ;
				wmove(window, cursor_position.y, cursor_position.x);
				break;
				
			default:
				//Otherwise simply keep on buffering charaters read from keyboard
				#ifdef AUTONEWLINE_ENABLED
				if(cursor_position.x == COLS) {
					cursor_position.x = 0;
					cursor_position.y++;
				}
				else
				#endif
				cursor_position.x++;
				final_cursor_position = cursor_position;
		}
		wrefresh(window);
	}//while()
	
	internal_buffer_.buff_out();	//Remove the ESC from buffer coz we dont want it as a text.
	//end_interface();
	throw (ModeSwitchException("[ModeSwitchException] Request for mode switch", "vi-mode"));

}

/**************************************** Editor ************************************/

void Editor :: setup_io(InputHandler* in, OutputHandler* out) {
	if((in == NULL) || (out == NULL))
		throw EditorRuntimeException("NULL IOHandler parameters provided in editor::setup_io()");
	//Setup in and out first... TODO
	
	this->io_input_ = in;
	this->io_output_ = out;
}

void Editor :: setup_filemanip(FileManipUnit* f) {
	if(f == NULL)
		throw EditorRuntimeException("NULL FileManipUnit parameters provided in editor::setup_filemanip()");
	//Setup f first... TODO
	
	this->file_manip_ = f;
}

void Editor :: switch_mode(const char *modename) {
	if(!strcmp(modename, "vi-mode")) {
		active_mode_ = &ViMode_;
	}
	else if(!strcmp(modename, "command-mode")) {
		active_mode_ = &CommandMode_;
	}
	else if(!strcmp(modename, "input-mode")) {
		active_mode_ = &InputMode_;
	}
}

void Editor :: start() {
	
	initscr();
	while(1) {
		
		try {
			active_mode_->run();
		}
		catch(ModeSwitchException e) {
			active_mode_->rest();
			switch_mode(e.modename());
		}
		
	}
}

void Editor :: terminate() {
	endwin();
}

int Editor :: current_mode() {
	if(!strcmp(active_mode_->modename_, "vi-mode"))
		return VI_MODE;
	else if(!strcmp(active_mode_->modename_, "command-mode"))
		return COMMAND_MODE;
	else if(!strcmp(active_mode_->modename_, "input-mode"))
		return INPUT_MODE;
	return -1; //Invalid. Just to satisfy the -Wreturn-type warning
}
