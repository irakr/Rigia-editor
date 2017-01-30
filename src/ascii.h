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
 * ascii.h
 *
 * ASCII codes as macros.
 */

#ifndef RIG_ASCII_H_
#define RIG_ASCII_H_

// ASCII control codes. ASCII character codes are not included since they can be directly understood.
#define 	NUL		0x00
#define 	SOH		0x01
#define 	STX		0x02
#define 	ETX 	0x03
#define 	EOT 	0x04
#define 	ENQ 	0x05
#define 	ACK 	0x06
#define 	BEL 	0x07
#define 	BS 		0x08
#define 	HT 		0x09
#define 	LF 		0x0A
#define 	VT 		0x0B
#define 	FF 		0x0C
#define 	CR 		0x0D
#define 	SO 		0x0E
#define 	SI 		0x0F
#define 	DLE		0x10
#define 	DC1		0x11
#define 	DC2		0x12
#define 	DC3		0x13
#define 	DC4		0x14
#define 	NAK		0x15
#define 	SYN		0x16
#define 	ETB		0x17
#define 	CAN		0x18
#define 	EM		0x19
#define 	SUB		0x1A
#define 	ESC		0x1B
#define 	FS		0x1C
#define 	GS		0x1D
#define 	RS		0x1E
#define 	US		0x1F
#define 	SPACE	0x20

#endif
