#ifndef CHESS_H
#define CHESS_H

#include "player.h"
#include "Main_Data.h"
#include "Save_Load_game.h"

class Game_engine {
private:
	Player* white;
	Player* black;
	Board* gameboard;
	Main_Data MD; // launching all needed data to start the program
	Save_Load_game m_save_load_game;

	bool end_game;
	bool pause_game;
	bool player_last_turn;//if true the game pause on black turn
	bool players_are_login;
	int cur_game_num;
	
	void defulte_parameters();
	int menu();
	bool user_input_caser(const int& user_input);
	bool make_action(const int action);
	void shut_down_system();
	void print_game_rules()const;
	void start_new_game();
	void reset_game_system_memory();
	//return the winner name
	void game_mainloop();
	bool login_2players();
	bool check_valid_name(const string& name);
	bool login_user(int num_of_player, const string& name);
	void player_turn(Player* plr);
	void end_game_formarl(Player* plr, const string& plr_res, const string& other_res);
	void save_game();
	bool load_game();
	void ask_if_to_save_the_cur_game();
	void show_game_log();
	void players_statistics();
	void player_statistics(const string& name, int line);
	void show_title_of_player_statistics(const string& name);
	void hall_of_fame();
	void exit_game();
	friend ostream& operator<<(ostream& os, Game_engine& game);

public:
	Game_engine();
	~Game_engine();
	void play_general();

};

#endif
