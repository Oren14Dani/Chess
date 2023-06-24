
#include "Piece_set.h"
#include "Board.h"

Board::Board() {
		m_white_set = new Piece_set(WHITE);
		m_black_set = new Piece_set(BLACK);

		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				sq_keys[i][j].set_square(i, j);
	}
Board::~Board() {
	clear_all_piece();
}
void Board::clear_all_piece() {
	delete m_white_set;
	delete m_black_set;
}
void Board::insert_one_piece(const string set_clr, Piece* sp) {
	if (set_clr == BLACK)
		m_black_set->get_listOFpicese().push_back(sp);
	else if (set_clr == WHITE)
		m_white_set->get_listOFpicese().push_back(sp);
}
void Board::fresh_new_board() { // loads board from file 'filename'
	m_white_set->new_game_Piece_set();
	m_black_set->new_game_Piece_set();

	//fill the white set on the board
	list<Piece*>::const_iterator it;
	it = m_white_set->get_listOFpicese().begin();
	for (int i = 0; i <= 1; i++)
		for (int j = 0; j < 8; j++) {
			map_board[sq_keys[i][j]] = *it;
			(*it)->set_loc(sq_keys[i][j]);
			it++;
		}

	//fill the black set on the board
	it = m_black_set->get_listOFpicese().begin();
	for (int i = 7; i >= 6; i--)
		for (int j = 0; j < 8; j++) {
			map_board[sq_keys[i][j]] = *it;
			(*it)->set_loc(sq_keys[i][j]);
			it++;
		}

	// fill empty space
	for (int i = 2; i <= 5; i++)
		for (int j = 0; j < 8; j++)
			map_board[sq_keys[i][j]] = nullptr;

}
map<square, Piece*>& Board::get_mapboard() { return map_board; }
Piece_set* Board::get_set(bool color) { return color ? m_black_set : m_white_set; }
square& Board::get_key(const int row, const int col) { return sq_keys[row][col]; }
 ostream& operator<<(ostream& os, const Board& b) {
	clrscr();

	/* Prints cols title */
	setcolor(GREEN, WHITE_DEC);
	for (int x = 0; x < NUM_OF_COLS; x++) {
		gotoxy(x * 3 + 2, 1);
		os << (char)(x + 'A');
	}

	/* Prints rows title */
	for (int y = 1; y <= NUM_OF_ROWS; y++) {
		gotoxy(0, y + 1);
		os << y;
	}

	int cnt = 0;
	int clr;
	/* Prints board */
	for (int y = 1; y <= NUM_OF_ROWS; y++) {
		for (int x = 0; x < NUM_OF_COLS; x++) {
			//clr = cnt % 2 + 1;
			if (cnt % 2 == 0)
				clr = BROWN;
			else
				clr = LIGHTCYAN;


			gotoxy(x * 3 + 1, y + 1);
			const Piece* piece = b.map_board.at(b.sq_keys[y - 1][x]);

			if (piece) {
				setcolor((piece->get_color()) ? BLACK_DEC : WHITE_DEC, clr);
				os << " " << piece->who_am_i() << " ";
			}

			else {
				setcolor(clr, clr);
				string x_y = "   ";
				os << x_y;
			}

			cnt++;
		}
		cnt++;
	}

	// print title 
	setcolor(BLUE, WHITE_DEC);
	gotoxy(8, 0);
	os << PLAYCHESS;

	// print go menu instaction
	gotoxy(28, 0);
	os << "enter " << MENU << " to go for menu ";
	gotoxy(28, 1);
	os << "enter " << TIE << " if want to declare tie ";
	gotoxy(28, 2);
	os << "enter " << SURRNDOR << " if want to surrndor ";

	// print rest of the game
	gotoxy(0, 11);
	setcolor(GREEN, WHITE_DEC);
	os << endl << endl;
	return os;
}

