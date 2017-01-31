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
 * io.h
 *
 * This module is used for handling input-output mechanism.
 * This module is intended to use the input/output system calls efficiently.
 *
 */
 
#ifndef RIG_IO_H_
#define RIG_IO_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_BUFF_SIZE 1024
// A simple struct for a buffer
typedef struct {
	char* buff;
	int offset; //Position in which the next incoming character will be pushed. Buffer will be flushed when this reaches MAX_BUFF_SIZE-1.
} Buffer;

//Abstract class for IO handlers
class IOHandler {
public:
	IOHandler() {
		internal_buffer_.buff = new char[MAX_BUFF_SIZE];
		internal_buffer_.offset = 0;
	}
protected:
	Buffer internal_buffer_;
};

// This class handles all incoming characters entered by the user in the editor environment.
class InputHandler : public IOHandler {
public:
	InputHandler() : IOHandler() { }
private:
};

// This class handles how characters will be displayed on the terminal and their behaviour.
class OutputHandler  : public IOHandler {
public:
	OutputHandler() : IOHandler() { }
private:
};

#endif
