#ifndef PIECE_H
#define PIECE_H

#include "square.h"

/*~~~~~~~~~~~~~~~~~~~ Pieces classes  ~~~~~~~~~~~~~~~~~~~*/

class Piece {
protected:
	square m_cur_loc;//current_location
	const bool m_color; /* true is black, false is white */
	const string m_type;
	bool is_alive; //dead or alive
	bool moved;
	map<square, bool> mapOfValidMove; // list of valid moce of every piece
public:
	Piece(const bool clr, const string type);
	Piece(const Piece* src);
	virtual ~Piece() = 0;
	virtual bool valid_move(const square dest, map<square, Piece*>* map_board);
	void move_to(const square& next_loc);
	const string& who_am_i() const;
	square& get_loc();
	void set_loc(const square& next_loc);
	//m_color ? BLACK : WHITE
	const bool get_color() const;
	void set_status(const bool status);
	//return true if the piece is alive
	bool get_status();
	map<square, bool> get_vaild_move();
		void set_piece_moved(bool on_off);
		bool get_piece_moved() const;

};

class Knight : public Piece {
public:
	Knight(const bool clr);

};

class King : public Piece {
private:
	bool m_is_check;
	list<Piece*> incoming_attcacks; // list of pieces that do 
	bool m_castling;
	Piece* m_partner_rook; // partner rook for castling

public:
	King(const bool clr);
	bool valid_move(const square dest, map<square, Piece*>* map_board);
	void set_check(const bool on_off);
	const bool& get_check() const;
	list<Piece*>& get_attack_list();
	void set_castling(const bool new_castling);
	const bool& get_castling() const;
	void set_partner_rook(Piece* new_partner_rook);
	Piece* get_partner_rook() const;

};

class Queen : public Piece {
public:
	Queen(const bool clr);
};

class Bishop : public Piece {
public:
	Bishop(const bool clr);
};

class Rook : public Piece {
private:
	square m_castling_loc;
public:
	Rook(const bool clr) ;
	void set_castling_loc(const square& new_castling_loc);
	square& get_castling_loc();
};

class Pawn : public Piece {
private:
	square atk, one_step, dobule_step;
	bool reach_board_limit; // need to attch to the board in a kind of way

public:
	Pawn(const bool clr);
	bool valid_move(const square dest, map<square, Piece*>* map_board);
};

#endif