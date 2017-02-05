/*
 * line_manip.h
 *
 * This module is responsible for managing lines and their operations in the editor.
 */

#ifndef RIG_LINE_MANIP_H_
#define RIG_LINE_MANIP_H_

#include "editor.h"

// A struct representing lines of the editor screen
struct Line {
	Coordinate current_position, final_position;
};

//TODO
class LineManipUnit {
public:
	LineManipUnit() { line = new Line*; nlines = 0; }
	~LineManipUnit() { delete line; }
	
private:
	Line **line;	//Line record of the editor
	int nlines;		//Number of lines currently existing in the editor
	int current_line; //Line in which the cursor currently lies
};

#endif
