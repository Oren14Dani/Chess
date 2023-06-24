#ifndef PIECE_SET_H
#define PIECE_SET_H

#include "Piece.h"

/*~~~~~~~~~~~~~~~~~~~ Piece_set class  ~~~~~~~~~~~~~~~~~~~*/

class Piece_set {
private:
	list<Piece*> m_pieces;
	const string color;

public:
	Piece_set(const string set_clr);
	//creat new piece and set his : color, cur_loc , moved or not
	Piece* restore_one_piece(const string sp_data[], square& sqr);
	void new_game_Piece_set();
	~Piece_set();
	list<Piece*>& get_listOFpicese();
	list<Piece*>* get_listOFpicese_pointer();
	// pawn that reaches the last row can be replaced by the player's choice of a bishop, knight, rook, or queen
	void promote_pawn(Piece* soldier, Piece* pis_to_prmote);
};



#endif