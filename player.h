#ifndef PLAYER_H
#define PLAYER_H

#include "board.h"
#include "game_log.h"
#include "PlayerStatistics.h"

//void test(string str);
class Player {
private:
	string m_name; // player's name
	const bool m_color; //True is black.
	Board* gameboard;
	map<square, Piece*>* map_board;
	Piece_set* my_set;
	King* my_king;
	King* rival_king;
	game_log* log;
	PlayerStatistics *m_plr_stat;

	void pawn_promotion(Piece* sp);
	Piece* choose_promotion(Piece* sp);
	// find out if the king can esc or one of my pieces can sace the king   
	bool run_esc_simulation(map<Piece*, map<square, bool >>& sp_vaild_map);
	void castling(const square& cur_sqr, const square& dest_sqr);
	// calculate the unit vector from start square to end square
	void clc_unit_vec(const square& start, const square& end, square& unit_vector);
	bool deep_check(Piece* sp, const square& dest);
	bool castling_simulation(const square& dest);
	// do virtual moving on the board and make shur that my_king is safe
	bool king_safe_simulation(Piece* sp, square cur_sqr, const square& dest_sqr);
	//put the rivel king in check
	void check_rival_king(Piece* sp, const square cur_sqr, const square dest_sqr);
	// if the move was attack then returns true as attack_log for later use
	bool move_pis_on_board(Piece* sp, const square cur_sqr, const square dest_sqr);
	// here the cur_sqr was the dest_sqr befor, and perv_sqr was the cur_sqr befor
	void revers_move(const square cur_sqr, const square perv_sqr, Piece* moving_sp, Piece* dead_sp = nullptr);
	bool check_free_way(const square cur_sqr, const square dest_sqr);
	bool select_valid_destination(square& dest_sqr, string& go_to_menu);
	Piece* select_piece(square& cur_sqr, string& go_to_menu);
	// get coor for player's attack
	// key words :
	//		exit = player give up (surrender)
	//		save = save game for later use
	//		load = load previous game
	//		menu = go to menu
	string input(square& sqr);
	bool castNvalid_square_input(char row, char col, square& tmp_sqr);

public:
	Player(Piece_set* set, Board* Gboard, const bool clr, PlayerStatistics* plr_stat, game_log* gl);
	const string& get_name() const ;
	void set_name(string name) {m_name = name;}
	bool make_move(string& go_to_menu, Piece* sp , square& cur_sqr, square& dest_sqr) ;
	bool get_color() const;
	PlayerStatistics* get_PlayerStat() { return m_plr_stat; }

};
#endif // ! PLAYER_H