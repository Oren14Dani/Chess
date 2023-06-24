#ifndef MAIN_DATA_H
#define MAIN_DATA_H

#include "player.h"

class Main_Data {
private:
	map<string, PlayerStatistics*> players_data_base;
	fstream file_data_base;
	int num_of_players;
	list<pair<int, string>> hall_of_fame_data; //have a list of all users name. 
	fstream game_counter_file;
	int games_counter; // determain the number of total games ever played
	Rank general_rank;
public:
	Main_Data();
	~Main_Data();
	//launch game initial data and read players data by names
	void launcher_users_data();
	// first line in file determain the number of total games ever played
	void launcher_games_counter();
	bool read_game_counter_file();
	void write_games_counter_file();
	//insert_player_name_to_MD_file
	bool add_user_to_DB(const string& name, bool new_player = false);
	bool add_user_to_file(const string& name);
	bool update_player_file(const string& name);
	void update_player_database(const string& name, const string& win_lose_tie_suur = "0");
	// sort by points 
	void sort_hall_of_fame();
	// use reverse iterator to print the plars ordered by their points from to high to low
	void show_hall_of_fame() const;
	void delete_user(const string& name);
	int& get_games_counter();
	void inc_games_counter();
	PlayerStatistics* get_player_data(const string& name);
	const list<pair<int, string>>& get_hall_of_fame()const;
	void print_chess_game_rules() const;

};

#endif