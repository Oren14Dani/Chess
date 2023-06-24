#ifndef UTILS_H
#define UTILS_H
#include <utility>

void clrscr(void);

// Position the cursor at column 'x', row 'y'
// The coodinates of upper left corner of the screen are (x,y)=(0,0)
void gotoxy(int x, int y);

// Set text color & background
void setcolor(unsigned int color, unsigned int background_color);


#endif