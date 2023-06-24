#ifndef GAME_LOG_H
#define GAME_LOG_H

#include "piece.h"


// ~~~~~~~~~~ game_log ~~~~~~~~~ //
class game_log {
private:
	fstream m_file;
	list<string> m_game_log; // save a log of all players movments 
	int* cur_game_num;
public:
	void set_cur_game_num(int* games_num);
	// open new empty game log file 
	void start_new_game_log();
	// if it is an existing game_log then load his file data to the list in memory
	bool load_game_log(int& game_number_to_load);
	string clc_string_for_game_log(Piece* sp, const square cur_loc, const square dest, bool attack = false, bool check = false);
	void print_previous_game_log() const;
	void print_cur_game_log() const;
	void print_last_moves_and_game_num() const;
	// save the last movement to file of game log
	bool save_last_move(const string& last_move_log);
	void save_game_log();
	list<string>& get_list();
	const int* get_game_number() const;
	void reset_game_log();
};



#endif