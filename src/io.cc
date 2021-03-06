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
 * io.cc
 */
 
//#include <stdio.h>
//#include <stdlib.h>
//#include <unistd.h>
//#include <fcntl.h>
//TODO... Perhaps we can make use of ioctl() or some other similar functions to control the behaviour of the terminal.
//#include <sys/ioctl.h>
#include "io.h"
#include "editor.h"
#include "exceptions.h"
#include "file_manip.h"
#include "ascii.h"
#include "ncurses_wrapper.h"
#include <unistd.h>
#include <ctype.h>

/***************** InputHandler ************************/

// TODO... This is still not an optimal implementation. Need to have access to kernel space and have an internal buffer there,
// maybe within a self written read() like system call.
int* InputHandler :: read_stream() {
	int *ptr;
	
	//TODO...XXX Still pending
	if((internal_buffer_.offset == -1) || (internal_buffer_.offset == MAX_BUFF_SIZE-1)) //For initialization or renewal of buffer
		internal_buffer_.offset = 0;
	ptr = &internal_buffer_.buff[internal_buffer_.offset];
	
	if(buffer_empty() == true) {
		//The STDIN_FILENO maybe changed to a pts/x soon, for a better operation. TODO
		while(read(STDIN_FILENO, &internal_buffer_.buff[internal_buffer_.offset], MAX_BUFF_SIZE) > 0) {
			internal_buffer_.offset++;
		}
	}
	return ptr;
}

int InputHandler :: read_key(WINDOW *w) {
	int c;
	if((c=wgetch(w)) != ERR) {
		internal_buffer_.buff_in(c);
		return c;
	}
	throw (IOException("Invalid key detected by 'InputHandler::read_key()'"));
}

/************ Buffer **********************/

void Buffer :: buff_in(int c) {
	if(offset == MAX_BUFF_SIZE-1) //For renewal of buffer
		offset = 0;
		// TODO... It may be a better idea to write to a file after the buffer gets full.
	else
		offset++;
	
	buff[offset] = c;
}

int Buffer :: buff_out() {
	if(offset == -1)
		throw BufferException("Buffer underflow in 'InputHandler::buff_out()'");
	return buff[offset--];
}
