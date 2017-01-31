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
 * main.cc
 *
 * This is the entry point for the 'Rigia-editor' program.
 * This module invokes the 'editor' module and everything later is taken care by that module.
 */

#include "io.h"
#include "file_manip.h"
#include "exceptions.h"
#include "editor.h"

/* Initialize components of the system	*/
void init_system();

/* Main	*/
int main(int argc, char *argv[]) {
	
	printf("\t\t--- Welcome to Rigia-editor ---\n");
	
	init_system();
	
	// Create and set-up editor
	Editor editor;
	editor.init_active_files(argc-1, argv+1); //Since 0th argv is the command's name itself
	editor.start();
	
	return 0;
}

void init_system() {
	ViMode_.setup_io((new InputHandler()), (new OutputHandler()));
	ViMode_.setup_filemanip(new FileManipUnit());
	CommandMode_.setup_io((new InputHandler()), (new OutputHandler()));
	CommandMode_.setup_filemanip(new FileManipUnit());
	InputMode_.setup_io((new InputHandler()), (new OutputHandler()));
	InputMode_.setup_filemanip(new FileManipUnit());
}
