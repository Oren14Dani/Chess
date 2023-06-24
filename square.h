#ifndef SQUARE_H
#define SQUARE_H

#include "general.h"


/*~~~~~~~~~~~~~~~~~~~ square class  ~~~~~~~~~~~~~~~~~~~*/

class square {
private:
	int m_row;
	int m_col;
	
public:
	square(int row = NOT_MATCH, int col = NOT_MATCH);//CTOR
	square(const square& src); //CC
	void set_square(int new_row, int new_col);
	int get_row() const;
	int get_col() const;
	bool operator< (const square& sqr) const; //MADE FOR THE MAP DATA STRCTURE.
	
	square operator-(const square& sqr) const ;
	square operator+(const square& sqr) const;
	square& operator +=(const square& sqr);
	square& operator -=(const square& sqr);
	friend ostream& operator<<(ostream& os, const square& sqr);
	square& operator=(const square& sqr);
	bool operator ==(const square& sqr) const;
	bool operator !=(const square& sqr) const;
	square operator*(int scaling);
	const string sqr_to_string() const;
};

#endif