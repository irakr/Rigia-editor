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
 * file_manip.h
 *
 * This module is used for managing file operations like - 
 * - Opening a new file for writing to.
 * - Save during runtime according to user's command.
 * - so on....
 */
 
#ifndef RIG_FILE_MANIP_H_
#define RIG_FILE_MANIP_H_

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

// This class is used for providing file management functionalities.
class FileManipUnit {
public:
	FileManipUnit() : write_fd_(STDOUT_FILENO), read_fd_(STDIN_FILENO), write_filename_("stdout"), read_filename_("stdin") { }
	//FileManipUnit(int fd, char *name);
private:
	int write_fd_;	//Descriptor of the file that is currently being edited
	int read_fd_;	//Descriptor of the file from which the editor is reading characters
	char *write_filename_, *read_filename_;
};

#endif
