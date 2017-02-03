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
#include <unistd.h>
#include <fcntl.h>
#include "ncurses_wrapper.h"
#include "editor.h"

extern Editor editor;

/********************************************* VI Mode ***************************************/

void ViMode :: init_interface() {
	//initscr();
	cbreak();
	keypad(stdscr, TRUE);
	noecho();
	move(InputMode_.cursor_position.y, InputMode_.cursor_position.x);
	refresh();
}

void ViMode :: end_interface() {
	endwin();
}

void ViMode :: rest() {
	//printf("Resting in %s\n", "Vi-Mode");
}

// Execution of the mode
void ViMode :: run() {
	move(2, 0);
	printw("Current mode: %s", modename_);
	refresh();
	init_interface();
	while(1) {
		char c = editor.io_input()->read_key();
		if(c == ESC) {
			end_interface();
			throw (ModeSwitchException("[ModeSwitchException] Request for mode switch", "vi-mode"));
		}
		else if(c == ':') {
			end_interface();
			throw (ModeSwitchException("[ModeSwitchException] Request for mode switch", "command-mode"));
		}
		else if(c == 'i') {
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
	move(20, 0);
	refresh();
}

void CommandMode :: end_interface() {
	endwin();
}

void CommandMode :: rest() {
	//printf("Resting in %s\n", "Command-Mode");
}

// Execution of the mode
void CommandMode :: run() {
	move(2, 0);
	printw("Current mode: %s", modename_);
	refresh();
	init_interface();
	while(1) {
		char c = editor.io_input()->read_key();
		if(c == ESC) {
			end_interface();
			throw (ModeSwitchException("[ModeSwitchException] Request for mode switch", "vi-mode"));
		}
		cursor_position.x++;
	}
}

/*************************************** Input Mode ****************************************/

void InputMode :: init_interface() {
	//initscr();
	cbreak();
	//raw();
	keypad(stdscr, TRUE);
	echo();
	move(ViMode_.cursor_position.y, ViMode_.cursor_position.x);
	refresh();
}

void InputMode :: end_interface() {
	endwin();
}

void InputMode :: rest() {
	//printf("Resting in %s\n", "Input-Mode");
}

// Execution of the mode
void InputMode :: run() {
	move(2, 0);
	printw("Current mode: %s", modename_);
	init_interface();
	while(1) {
		char c = editor.io_input()->read_key();
		
		//Detect control characters for special operations.
		switch (c) {
			case ESC:
				end_interface();
				throw (ModeSwitchException("[ModeSwitchException] Request for mode switch", "vi-mode"));
			
			//case KEY
			
			default:
				//Otherwise simply keep on buffering charaters read from keyboard
				cursor_position.x++;
		}
		
		
	}
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
	move(1, 10);
	printf("--- Welcome to Rigia-editor ---");
	refresh();
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
