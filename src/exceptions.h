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
 * Exceptions.h
 *
 * Exception handling classes are defined in this module.
 */

#ifndef RIG_EXCEPTIONS_H_
#define RIG_EXCEPTIONS_H_

#define EXC_MESSAGE_LEN 256

#include <string.h>
#include <exception>
#include <stdexcept>

#define DEFAULT_EXCEPTION_MESSAGE "Rigia-editor exception generated"

using namespace std;

// Just an exception specifying a runtime error
class EditorRuntimeException : public runtime_error {
public:
	EditorRuntimeException() : runtime_error(DEFAULT_EXCEPTION_MESSAGE) { }
	EditorRuntimeException(const char *msg) : runtime_error(msg) { }
};

// Memory related exceptions
class BufferException : public EditorRuntimeException {
public:
	BufferException() : EditorRuntimeException(DEFAULT_EXCEPTION_MESSAGE) { }
	BufferException(const char *msg) : EditorRuntimeException(msg) { }

};

// IO exceptions
class IOException : public EditorRuntimeException {
public:
	IOException() : EditorRuntimeException() { }
	IOException(const char *msg) : EditorRuntimeException(msg) { }
};

/* NOTE: The below defined exception class is not an erroneous exception. Its just a trick I used to switch modes. Just
 *			using the interrupt like behaviour of exception generation and handling.
 * Anyway this might only be a temporary implementation but lets see.
 */
 
// Exception for changing mode
class ModeSwitchException : public exception {
public:
	ModeSwitchException() { }
	ModeSwitchException(const char* msg, const char* modename) {
		strcpy(message, msg);
		strcpy(modename_, modename);
	}
	const char* what() const noexcept { return message; }
	const char* modename() { return modename_; }
private:
	char message[EXC_MESSAGE_LEN], modename_[25];
};

#endif
