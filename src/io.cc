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
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
//TODO... Perhaps we can make use of ioctl() or some other similar functions to control the behaviour of the terminal.
#include <sys/ioctl.h>
#include "io.h"

// TODO... This is still not an optimal implementation. Need to have access to kernel space and have an internal buffer there,
// maybe within a self written read() like system call.
char* InputHandler :: read_stream() {
	char *ptr;
	
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

char* InputHandler :: read_key() {
	static char c;
	if(read(STDIN_FILENO, &c, 1) == 1)
		return &c;
	return NULL;
}
