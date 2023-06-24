

#include "Piece_set.h"

Piece_set::Piece_set(const string set_clr) : color(set_clr) {}
//creat new piece and set his : color, cur_loc , moved or not
Piece* Piece_set::restore_one_piece(const string sp_data[], square& sqr) {
	bool clr = (sp_data[1] == BLACK); //

	Piece* new_piece = nullptr;
	if (sp_data[0] == KING)
		new_piece = new King(clr);
	else if (sp_data[0] == QUEEN)
		new_piece = new Queen(clr);
	else if (sp_data[0] == KNIGHT)
		new_piece = new Knight(clr);
	else if (sp_data[0] == BISHOP)
		new_piece = new Bishop(clr);
	else if (sp_data[0] == ROOK)
		new_piece = new Rook(clr);
	else if (sp_data[0] == PAWN)
		new_piece = new Pawn(clr);

	new_piece->set_loc(sqr);
	new_piece->set_piece_moved((sp_data[2] == "MOVED") ? YES : NO);
	return new_piece;
}
void Piece_set::new_game_Piece_set() {
	bool clr = (color == BLACK);
	m_pieces.emplace_back(new Rook(clr));
	m_pieces.emplace_back(new Knight(clr));
	m_pieces.emplace_back(new Bishop(clr));
	m_pieces.emplace_back(new Queen(clr));
	m_pieces.emplace_back(new King(clr));
	m_pieces.emplace_back(new Bishop(clr));
	m_pieces.emplace_back(new Knight(clr));
	m_pieces.emplace_back(new Rook(clr));
	for (int i = 0; i < 8; i++)
		m_pieces.emplace_back(new Pawn(clr));
}
Piece_set::~Piece_set() {
	list<Piece*>::iterator it = m_pieces.begin();
	for (; it != m_pieces.end(); it++)
		delete* it;
}
list<Piece*>& Piece_set::get_listOFpicese() { return m_pieces; }
list<Piece*>* Piece_set::get_listOFpicese_pointer() { return &m_pieces; }
// pawn that reaches the last row can be replaced by the player's choice of a bishop, knight, rook, or queen
void Piece_set::promote_pawn(Piece* soldier, Piece* pis_to_prmote) {
	pis_to_prmote->set_loc(soldier->get_loc());
	pis_to_prmote->set_piece_moved(YES);
	list<Piece*>::const_iterator it = m_pieces.begin();
	for (; it != m_pieces.end(); it++)
		if ((*it)->get_loc() == soldier->get_loc()) {
			delete(*it);
			m_pieces.erase(it);
			m_pieces.emplace_back(pis_to_prmote);
			break;
		}
}
