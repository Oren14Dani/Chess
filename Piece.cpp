
#include "square.h"
#include "piece.h"

/*~~~~~~~~~~~~~~~~~~~ Pieces classes  ~~~~~~~~~~~~~~~~~~~*/


Piece::Piece(const bool clr, const string type) : m_color(clr),
	m_type(type), is_alive(YES), moved(NO) { }
Piece::Piece(const Piece* src) : m_type(src->who_am_i()), m_color(src->m_color) {
	m_cur_loc = src->m_cur_loc;
	is_alive = src->is_alive;
	moved = src->moved;
}
Piece::~Piece()  { }

bool Piece::valid_move(const square dest, map<square, Piece*>* map_board) {
	try { return mapOfValidMove.at(dest - m_cur_loc); }
	catch (out_of_range) { return NO; }
}

void Piece::move_to(const square& next_loc) {
	m_cur_loc = next_loc;
	moved = YES;
	cout << "moved" << endl;
}
const string& Piece:: who_am_i() const { return m_type; }
square& Piece::get_loc() { return m_cur_loc; }
void Piece::set_loc(const square& next_loc) { m_cur_loc = next_loc; }
//m_color ? BLACK : WHITE
const bool Piece::get_color() const { return m_color; }
void Piece::set_status(const bool status) { is_alive = status; }
//return true if the piece is alive
bool Piece::get_status() { return is_alive; }
map<square, bool> Piece::get_vaild_move() { return mapOfValidMove; }
void Piece::set_piece_moved(bool on_off) { moved = on_off; }
bool Piece::get_piece_moved() const { return moved; }


Knight::Knight(const bool clr) : Piece(clr, KNIGHT) {
	// Knight can move 1 to sides and 2 up/down OR 2 to sides and 1 up/down
	mapOfValidMove[square(2, 1)] = YES;
	mapOfValidMove[square(1, 2)] = YES;
}


King::King(const bool clr) : Piece(clr, KING), m_is_check(NO), m_castling(NO) {
	// can move onely 1 square to every direction
	mapOfValidMove[square(1, 1)] = YES;
	mapOfValidMove[square(1, 0)] = YES;
	mapOfValidMove[square(0, 1)] = YES;

	m_partner_rook = nullptr;
}
bool King::valid_move(const square dest, map<square, Piece*>* map_board) {
	// castling
	square tmp = dest - get_loc();
	if (!moved && !m_is_check)
		if (tmp.get_row() == 0 && tmp.get_col() == 2 || tmp.get_col() == 3) {
			m_castling = true;
			return true;
		}
	try { return mapOfValidMove.at(dest - m_cur_loc); }
	catch (out_of_range) { return NO; }
}
void King::set_check(const bool on_off) { m_is_check = on_off; }
const bool& King::get_check() const { return m_is_check; }
list<Piece*>& King::get_attack_list() { return incoming_attcacks; }
void King::set_castling(const bool new_castling) { m_castling = new_castling; }
const bool& King::get_castling() const { return m_castling; }
void King::set_partner_rook(Piece* new_partner_rook) { m_partner_rook = new_partner_rook; }
Piece* King::get_partner_rook() const { return m_partner_rook; }


Queen::Queen(const bool clr) : Piece(clr, QUEEN) {
	// can move to the sides or up/down or diagonally (8 square maximum) 
	for (int i = 1; i <= 8; i++) {
		mapOfValidMove[square(0, i)] = YES;
		mapOfValidMove[square(i, 0)] = YES;
		mapOfValidMove[square(i, i)] = YES;
	}
}


Bishop::Bishop(const bool clr) : Piece(clr, BISHOP) {
	// can move onely diagonally (8 square maximum) 
	for (int i = 1; i <= 8; i++)
		mapOfValidMove[square(i, i)] = YES;
}


Rook::Rook(const bool clr) : Piece(clr, ROOK) {
	//can move onely to the sides or up/down (8 square maximum)
	for (int i = 1; i <= 8; i++) {
		mapOfValidMove[square(0, i)] = YES;
		mapOfValidMove[square(i, 0)] = YES;
	}
}
void Rook::set_castling_loc(const square& new_castling_loc) { m_castling_loc = new_castling_loc; }
square& Rook::get_castling_loc() { return m_castling_loc; }


Pawn::Pawn(const bool clr) : Piece(clr, PAWN), reach_board_limit(NO) {
	// can move 1 up OR 2 up in the first move
	atk.set_square(1, 1);
	one_step.set_square(1, 0);
	dobule_step.set_square(2, 0);
}
bool Pawn::valid_move(const square dest, map<square, Piece*>* map_board) {
	square tmp = dest - m_cur_loc;//1,1 1,0 2,0
	int tmp_row = dest.get_row() - m_cur_loc.get_row();
	if ((tmp_row < 0 && !m_color) || (tmp_row > 0 && m_color)) //Pawn white can go only UP, black can go only DOWN
		return false;

	if (map_board->at(dest) == nullptr) {
		if (tmp == one_step)
			return true;
		else if (tmp == dobule_step && !moved)
			return true;
	}
	else if (map_board->at(dest)->get_color() != m_color && tmp == atk)//attack only diagnoaly form
		return true;

	return false;
}

