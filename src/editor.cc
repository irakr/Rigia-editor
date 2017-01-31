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
//TODO... Perhaps we can make use of ioctl() or some other similar functions to control the behaviour of the terminal.
#include <sys/ioctl.h>
#include "editor.h"


/********************************************* VI Mode ***************************************/

void ViMode :: setup_io(InputHandler* in, OutputHandler* out) {
	if((in == NULL) || (out == NULL))
		throw EditorRuntimeException("NULL IOHandler parameters provided in ViMode::setup_io()");
	this->io_input_ = in;
	this->io_output_ = out;
}

void ViMode :: setup_filemanip(FileManipUnit* f) {
	if(f == NULL)
		throw EditorRuntimeException("NULL FileManipUnit parameters provided in ViMode::setup_filemanip()");
	this->file_manip_ = f;
}

void ViMode :: rest() {
	printf("Resting in %s\n", "Vi-Mode");
}

// Execution of the mode
void ViMode :: run() {
	while(read(STDIN_FILENO, &internal_buffer_.buff[internal_buffer_.offset], 1) > 0) {
		if(internal_buffer_.offset < MAX_BUFF_SIZE) {
			if(internal_buffer_.offset == MAX_BUFF_SIZE-1) {
				//flush buffer to a file
				internal_buffer_.offset = 0;
			}
			else {
				internal_buffer_.offset++;
			}
		}
		if(internal_buffer_.buff[internal_buffer_.offset-1] == ESC);
			//change_mode_to(VI_MODE);
		//else if('i') then rest() this mode and switch mode to INPUT_MODE TODO
	}
	
}

/*************************************** Command Mode **************************************/

void CommandMode :: setup_io(InputHandler* in, OutputHandler* out) {
	if((in == NULL) || (out == NULL))
		throw EditorRuntimeException("NULL IOHandler parameters provided in CommandMode::setup_io()");
	this->io_input_ = in;
	this->io_output_ = out;
}

void CommandMode :: setup_filemanip(FileManipUnit* f) {
	if(f == NULL)
		throw EditorRuntimeException("NULL FileManipUnit parameters provided in CommandMode::setup_filemanip()");
	this->file_manip_ = new FileManipUnit();
}

void CommandMode :: rest() {
	printf("Resting in %s\n", "Command-Mode");
}

// Execution of the mode
void CommandMode :: run() {
	while(read(STDIN_FILENO, &internal_buffer_.buff[internal_buffer_.offset], 1) > 0) {
		if(internal_buffer_.offset < MAX_BUFF_SIZE) {
			if(internal_buffer_.offset == MAX_BUFF_SIZE-1) {
				//flush buffer to a file
				internal_buffer_.offset = 0;
			}
			else {
				internal_buffer_.offset++;
			}
		}
		if(internal_buffer_.buff[internal_buffer_.offset-1] == ESC);
			//change_mode_to(VI_MODE);
		//else if('i') then rest() this mode and switch mode to INPUT_MODE TODO
	}
	
}

/*************************************** Input Mode ****************************************/

void InputMode :: setup_io(InputHandler* in, OutputHandler* out) {
	if((in == NULL) || (out == NULL))
		throw EditorRuntimeException("NULL IOHandler parameters provided in InputMode::setup_io()");
	this->io_input_ = in;
	this->io_output_ = out;
}

void InputMode :: setup_filemanip(FileManipUnit* f) {
	if(f == NULL)
		throw EditorRuntimeException("NULL FileManipUnit parameters provided in InputMode::setup_filemanip()");
	this->file_manip_ = new FileManipUnit();
}

void InputMode :: rest() {
	printf("Resting in %s\n", "Input-Mode");
}

// Execution of the mode
void InputMode :: run() {
	while(read(STDIN_FILENO, &internal_buffer_.buff[internal_buffer_.offset], 1) > 0) {
		if(internal_buffer_.offset < MAX_BUFF_SIZE) {
			if(internal_buffer_.offset == MAX_BUFF_SIZE-1) {
				//flush buffer to a file
				internal_buffer_.offset = 0;
			}
			else {
				internal_buffer_.offset++;
			}
		}
		if(internal_buffer_.buff[internal_buffer_.offset-1] == ESC);
			//change_mode_to(VI_MODE);
		//else if('i') then rest() this mode and switch mode to INPUT_MODE TODO
	}
	
}

/**************************************** Editor ************************************/

Editor :: Editor() {
	active_mode_ = &ViMode_;	//Default
}

void Editor :: init_active_files(int count, char *file_names[]) {
	if(count == 0)
		return;
	//Fills up the linked list 'active_files_'
	active_files_ = new NameList();
	NameList *ptr = active_files_;
	int i = 0;
	strcpy(ptr->name, file_names[i++]);
	while(i < count) {
		ptr->next = new NameList();
		ptr = ptr->next;
		strcpy(ptr->name, file_names[i++]);
	}
	ptr->next = NULL;
}

void Editor :: start() {
	printf("Starting editor...\n");
	printf("Current mode: %s\n", active_mode_->mode_);
}
