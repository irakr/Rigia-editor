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
 * editor.h
 *
 * This module is the main station of the program.
 * It provides the user interface environment and connects all other modules together.
 */
 
#ifndef RIG_EDITOR_H_
#define RIG_EDITOR_H_

#include "io.h"
#include "file_manip.h"
#include "exceptions.h"
#include "ascii.h"

// User interface modes
#define 	COMMAND_MODE	1
#define 	INPUT_MODE		2
#define 	VI_MODE			0
#define 	DEFAULT_MODE 	VI_MODE

// Abstract class for each type of modes of the vi-editor
class ModeInterface {
public:
	ModeInterface(const char* mode) {
		mode_ = mode;
		internal_buffer_.buff = new char[MAX_BUFF_SIZE];
		internal_buffer_.offset = 0;
	}
	~ModeInterface() {
		mode_ = NULL;
		delete io_input_;
		delete io_output_;
		delete file_manip_;
		delete[] internal_buffer_.buff;
	}
	
	virtual void run() = 0;										//Start running the current mode after setting all the parameters correctly
	virtual void rest() = 0; 									//Rest the current mode so that the new mode can start without any overlaps
	virtual void setup_io(InputHandler*, OutputHandler*) = 0; 	//Sets up the io_handler parameters for a mode
	virtual void setup_filemanip(FileManipUnit*) = 0; 			//Sets up the file_manipulator parameters for a mode
	
	//Since the Editor class will control all operations externally it need access to all members
	friend class Editor;											
	
protected:
	const char* mode_;
	InputHandler *io_input_; //Handles the input characters
	OutputHandler *io_output_; //Handles the output characters
	FileManipUnit *file_manip_; //Handles the file operations
	Buffer internal_buffer_;	//Internal buffer (Different from io buffer).
								//This buffer stores the characters that are echoed to the screen while the user is typing.
};

// VI mode
static class ViMode : public ModeInterface {
public:
	ViMode() : ModeInterface("vi-mode") { }
	~ViMode() { }
	void run();
	void rest();
	void setup_io(InputHandler*, OutputHandler*);
	void setup_filemanip(FileManipUnit*);
} ViMode_;

// Command mode
static class CommandMode : public ModeInterface {
public:
	CommandMode() : ModeInterface("command-mode") { }
	~CommandMode() { }
	
	void run();
	void rest();
	void setup_io(InputHandler* in, OutputHandler* out);
	void setup_filemanip(FileManipUnit*);
} CommandMode_;

// Input mode
static class InputMode : public ModeInterface {
public:
	InputMode() : ModeInterface("input-mode") { }
	~InputMode() { }
	
	void run();
	void rest();
	void setup_io(InputHandler* in, OutputHandler* out);
	void setup_filemanip(FileManipUnit*);
} InputMode_;

#define FILENAME_LEN_MAX 128
// A linked list node to store names of every opened file
typedef struct _NameList_ {
	char name[FILENAME_LEN_MAX];
	struct _NameList_ *next;
} NameList;

/* The main Editor module */
class Editor {
public:
	Editor();	
	
	void switch_mode(char*);
	void start();
	void init_active_files(int, char*[]);
private:
	ModeInterface *active_mode_;
	NameList *active_files_; //Implemented as a linked list.
};


#endif
