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
#include "line_manip.h"
#include "exceptions.h"
#include "ascii.h"
#include "ncurses_wrapper.h"

// User interface modes. These macros are used by the current_mode() function as return type.
#define 	COMMAND_MODE	1
#define 	INPUT_MODE		2
#define 	VI_MODE			0
#define 	DEFAULT_MODE 	VI_MODE

// Abstract class for each type of modes of the vi-editor
class ModeInterface {

public:
	ModeInterface(const char* mode) {
		modename_ = mode;
		cursor_position.x = cursor_position.y = 0;
		final_cursor_position.x = final_cursor_position.y = 0;
		window = NULL;
	}
	virtual ~ModeInterface() {
		modename_ = NULL;
		delete window;
	}
	
	virtual void run() = 0;	 //Start running the current mode after setting all the parameters correctly
	virtual void rest() = 0; //Rest and also save state of the current mode so that the new mode can start without any overlaps
	virtual void init_interface() = 0;	//Initialize interface type. (The 'ncurses' library is used here)
	virtual void end_interface() = 0;	//Ends the interface type. ('')
	
	//Since the Editor class will control all operations externally it need access to all members
	friend class Editor;											
	
protected:
	const char* modename_;
	Buffer internal_buffer_;
	
	WINDOW *window; //Work window of the mode
	
	//All modes have control over the cursor in their own way
	Coordinate cursor_position;	// Current cursor position
	Coordinate final_cursor_position; // The largest cursor position that the mode has gone through and which is the boundary
};

// VI mode
static class ViMode : public ModeInterface {

public:
	ViMode() : ModeInterface("vi-mode") { }
	~ViMode() { }
	void run();
	void rest();
	void init_interface();
	void end_interface();
	
	friend class InputMode;
} ViMode_;

// Command mode
static class CommandMode : public ModeInterface {

public:
	CommandMode() : ModeInterface("command-mode") { }
	~CommandMode() { }
	
	void run();
	void rest();
	void init_interface();
	void end_interface();
	
	void exec_command(); //Runs the command stored in the internal_buffer_
} CommandMode_;

// Input mode
static class InputMode : public ModeInterface {

public:
	InputMode() : ModeInterface("input-mode") { }
	~InputMode() { }
	
	void run();
	void rest();
	void init_interface();
	void end_interface();
	
	//Since ViMode should be able to access and perhaps modify the cursor_position of InputMode.
	//(You can investigate it on 'vi' editor.)
	friend class ViMode;
//XXX... Maybe a buffer will be needed for this class object
//private:
	//Buffer buff;
} InputMode_;

/* The main Editor module */
class Editor {

public:
	Editor() {
		active_mode_ = &ViMode_; //Default
		line_manip_ = new LineManipUnit();
	} 
	~Editor() {
		delete active_mode_;
		delete io_input_;
		delete io_output_;
		delete file_manip_;
		delete line_manip_;
	}
	
	void switch_mode(const char*);
	void start();
	void terminate();
	
	//Below two functions may be removed and their logic may be added to the constructor. TODO
	void setup_io(InputHandler*, OutputHandler*);
	void setup_filemanip(FileManipUnit*);
	
	// Accessors
	ModeInterface* active_mode() { return active_mode_; }
	InputHandler* io_input() { return io_input_; }
	OutputHandler* io_output() { return io_output_; }
	FileManipUnit* file_manip() { return file_manip_; }
	LineManipUnit* line_manip() { return line_manip_; }
	
	int current_mode();	// Return the macro constant of the currently active mode identified by 'active_mode_'.
	
	//These functions uses the buffer of the io_input module.
	//friend void ViMode :: run();
	//friend void CommandMode :: run();
	//friend void InputMode :: run();
	friend class ViMode;
	friend class CommandMode;
	friend class InputMode;
private:
	ModeInterface *active_mode_;
	InputHandler *io_input_; //Handles the input characters
	OutputHandler *io_output_; //Handles the output characters
	FileManipUnit *file_manip_; //Handles file operations
	LineManipUnit *line_manip_; //Handles line operations
};


#endif
