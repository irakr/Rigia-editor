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
 * file_manip.cc
 */
 
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include "file_manip.h"
#include "exceptions.h"

FileManipUnit :: FileManipUnit() {
	if((log_file_.fd = open("logs", O_CREAT | O_RDWR, S_IRWXU | S_IRUSR)) == -1) {
		throw IOException("[IOException] Could not open log file");
	}
	write(log_file_.fd, "LOG FILE\n", 9);
}

FileManipUnit :: ~FileManipUnit() {
	close(log_file_.fd);
}

void FileManipUnit :: init_active_files(int count, char *file_names[]) {
	if(count == 0)
		return;
	//Fills up the linked list 'active_files_'
	active_files_ = new FileList();
	FileList *ptr = active_files_;
	int i = 0;
	strcpy(ptr->name, file_names[i++]);
	while(i < count) {
		ptr->next = new FileList();
		ptr = ptr->next;
		strcpy(ptr->name, file_names[i++]);
	}
	ptr->next = NULL;
}

//Returns the pointer to the nth active_files_ node
FileList* FileManipUnit :: get_fileinfo(int n) {
	//TODO...Implement it fully later
	return active_files_;
}

int FileManipUnit :: log(const char *filename, const char *modename, const char *errmsg) {
	
	if(!filename || !modename || !errmsg)
		return -1;
	char header[200] = "-------------------------------------------------------------\n";
	char *text = new char[strlen(filename) + strlen(modename) + strlen(errmsg) + 1024];
	if(!text)
		return -1;
	
	struct timespec tp;
	clock_gettime(CLOCK_REALTIME, &tp);
	strcat(strcat(header, ctime(&tp.tv_sec)), "\n");
	write(log_file_.fd, header, strlen(header));
	
	strcat(strcat(strcpy(text, "Filename: "), filename), "\n");
	strcat(strcat(strcat(text, "In mode: "), modename), "\n");
	strcat(strcat(strcat(text, "Error: "), errmsg), "\n");
	write(log_file_.fd, text, strlen(text));
	
	delete text;
	return 0;
}
