#ifndef BOARD_H
#define BOARD_H

#include "Piece_set.h"


class Board {
private:
	square sq_keys[NUM_OF_ROWS][NUM_OF_ROWS]; // use for easy access to square key 
	map<square, Piece*> map_board;
	Piece_set* m_black_set;
	Piece_set* m_white_set;

public:
	Board();
	~Board();
	void clear_all_piece();
	void insert_one_piece(const string set_clr, Piece* sp);
	void fresh_new_board();
	map<square, Piece*>& get_mapboard();
	Piece_set* get_set(bool color);
	square& get_key(const int row, const int col);
	friend ostream& operator<<(ostream& os, const Board& b);
};

#endif