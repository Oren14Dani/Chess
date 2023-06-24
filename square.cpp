/*~~~~~~~~~~~~~~~~~~~ square class  ~~~~~~~~~~~~~~~~~~~*/
#include "square.h"


	square::square(int row , int col ) :m_row(row), m_col(col) {}//CTOR
	square::square(const square& src){ //CC
		 m_row = src.get_row();
		 m_col = src.get_col();
	}
	void square::set_square(int new_row, int new_col) { m_row = new_row; m_col = new_col; };
	int square::get_row() const { return m_row; }
	int square::get_col() const { return m_col; }
	bool square::operator< (const square& sqr) const {//MADE FOR THE MAP DATA STRCTURE.
		if (m_row < sqr.get_row())
			return true;
		else if (m_row > sqr.get_row())
			return false;
		else if (m_col < sqr.get_col()) // rows are equal
			return true;
		else if (m_col > sqr.get_col())
			return false;
		else  // rows and cols are equal
			return false;
	}
	square square::operator-(const square& sqr) const {
		return square(abs(sqr.get_row() - m_row), abs(sqr.get_col() - m_col));
	}
	square square::operator+(const square& sqr) const {
		return square(sqr.get_row() + m_row , sqr.get_col() + m_col);
	}
	square& square::operator +=(const square& sqr) {
		m_row += sqr.get_row();
		m_col += sqr.get_col();
		return *this;
	}
	square& square::operator -=(const square& sqr) {
		m_row -= sqr.get_row();
		m_col -= sqr.get_col();
		return *this;
	}
	ostream& operator<<(ostream& os, const square& sqr) {
		os << " (" << sqr.get_row() << "," << sqr.get_col() << ") ";
		return os;
	}
	//friend istream& operator>>(istream& is, const square& p);
	square& square::operator=(const square& sqr) {
		m_row = sqr.get_row();
		m_col = sqr.get_col();
		return *this;
	}
	bool square::operator ==(const square& sqr) const {
		if ( (m_row == sqr.get_row()) && (m_col == sqr.get_col()) )
			return true;
		return false;
	}
	bool square::operator !=(const square& sqr) const {
		if ((m_row != sqr.get_row()) || (m_col != sqr.get_col()))
			return true;
		return false;
	}
	square square::operator*(int scaling) {
		m_col *= scaling;
		m_row *= scaling;
		return *this;
	}
	const string square::sqr_to_string()const {
		char s[5] = { get_row() + '1', get_col() + 'A','\0'};
		return string(s);
	}