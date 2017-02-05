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
	noecho(); //Echoing is done in a controlled manner in all modes
	
	if(!window) {
		window = newwin(LINES-1, COLS, 0, 0);
		keypad(window, TRUE);
	}
	
	//Take the cursor position the inputmode
	//cursor_position.x = InputMode_.cursor_position.x;
	//cursor_position.y = InputMode_.cursor_position.y;
	cursor_position = InputMode_.cursor_position;
	wmove(window, cursor_position.y, cursor_position.x);
	wrefresh(window);
	final_cursor_position = InputMode_.final_cursor_position;
}
//
void ViMode :: end_interface() {
	//destroy_win(window);
	//endwin();
}

void ViMode :: rest() {
	//printf("Resting in %s\n", "Vi-Mode");
}

// Execution of the mode
void ViMode :: run() {
	init_interface();
	while(1) {
		int c;
		try {
			c = editor.io_input()->read_key(window);
		}
		catch(IOException e) {
			//mvwprintw(window, COLS/2, LINES/2, e.what());
			editor.file_manip()->log(editor.file_manip()->get_fileinfo(1)->name, modename_, e.what());
			continue;
		}
		
		switch (c) {
			case ESC:
				end_interface();
				//editor.file_manip()->log(editor.file_manip()->get_fileinfo(1)->name, modename_, "Attempted switch to VI_MODE\n");
				throw (ModeSwitchException("[ModeSwitchException] Request for mode switch", "vi-mode"));
			case ':':
				end_interface();
				//editor.file_manip()->log(editor.file_manip()->get_fileinfo(1)->name, modename_, "Attempted switch to COMMAND_MODE\n");
				throw (ModeSwitchException("[ModeSwitchException] Request for mode switch", "command-mode"));
			case 'i':
				end_interface();
				//editor.file_manip()->log(editor.file_manip()->get_fileinfo(1)->name, modename_, "Attempted switch to INPUT_MODE\n");
				throw (ModeSwitchException("[ModeSwitchException] Request for mode switch", "input-mode"));
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
			
		}
		
	}
	
	//end_interface();
}

/*************************************** Command Mode **************************************/

void CommandMode :: init_interface() {
	//noecho();
	
	if(!window) {
		window = newwin(1, COLS, LINES-1, 0);
		keypad(window, TRUE);
	}
	//wprintw(window, "%c", editor.io_input_->buffer()->buff_out());
	mvwaddch(window, 0, 0, ':');
	wrefresh(window);
}

void CommandMode :: end_interface() {
	//destroy_win(window);
	//endwin();
	delwin(window);
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
		int c;
		try {
			c = editor.io_input()->read_key(window);
		}
		catch(IOException e) {
			//editor.file_manip()->log(editor.file_manip()->get_fileinfo(1)->name, modename_, e.what());
			continue;
		}
		switch (c) {
			case ESC:
				end_interface();
				//editor.file_manip()->log(editor.file_manip()->get_fileinfo(1)->name, modename_, "Attempted switch to VI_MODE\n");
				throw (ModeSwitchException("[ModeSwitchException] Request for mode switch", "vi-mode"));
			
			case KEY_ENTER:
				exec_command();
				break;
			
			default:
				cursor_position.x++;
				mvwaddch(window, cursor_position.y, cursor_position.x, c);
		}
		
	}
}

void CommandMode :: exec_command() {
	//TODO...
}

/*************************************** Input Mode ****************************************/

void InputMode :: init_interface() {
	//noecho();
	if(!window)
		window = ViMode_.window;
	cursor_position = ViMode_.cursor_position;
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
	
	while(1) {
		
		try {
			c = editor.io_input()->read_key(window);
		}
		catch(IOException e) {
			//editor.file_manip()->log(editor.file_manip()->get_fileinfo(1)->name, modename_, e.what());
			continue;
		}
		
		internal_buffer_.buff_in(c); //Will later be buff_out() if control character
		
		//Different operations for different characters...
		switch (c) {
			case ESC:			
				internal_buffer_.buff_out();	//Remove the ESC from buffer coz we dont want it as a text.
				//editor.file_manip()->log(editor.file_manip()->get_fileinfo(1)->name, modename_, "Attempted switch to VI_MODE\n");
				//end_interface();
				throw (ModeSwitchException("[ModeSwitchException] Request for mode switch", "vi-mode"));
				
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
				//Otherwise simply echo and buffer charater read from keyboard
				mvwaddch(window, cursor_position.y, cursor_position.x, c);
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
	//flushinp();
	cbreak;
	keypad(stdscr, TRUE);
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
