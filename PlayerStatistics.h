#ifndef PLAYERSTATISTICS_H
#define PLAYERSTATISTICS_H

#include "rank.h"

class PlayerStatistics {
private:
	map<string, int> player_database;
	string keys[KEYS] = { POINTS, GAME_PLAYED, WIN ,LOSE ,TIE , SURRNDOR, CHECK, ATTACKS };
	Rank* m_rank_class;
	string m_rank;// define for 150 points for rank //rookie < beginer < averge < advance < expert < god
	const string player_file_name;
	const string player_name;
	fstream m_file;

public:
	const string& get_rank()const;
	const string& get_key(int idx)const;
	// creat new statistics or extract existent player statistics info form file   
	// file name form is: player_name.txt
	PlayerStatistics(Rank* rank_class, const string& plr_name);
	const int& get_points();
	const string& get_player_file_name();
	const string& get_player_name();
	bool fill_memory(const string& plr_name, bool new_player = false);
	void inc_attack();
	void inc_check();
	void edit_data_end_game(string win_lose_tie_surrndor);
	void calc_rank(int& value);
	// update data and points by the results of last game  
	// win == +30 points
	// lose == -10 points
	// tie == +10 point
	// surrndor = -5 points
	// check = +2 points
	// attacks == +1 points
	void update_points(const string& action, const string& win_lose_tie_suur = "0");
	const map<string, int>& get_statsic_data() const;

	bool save_static_file();
	// update rank if the new rank is higher then the old rank 
	void update_rank(const int& new_rank_value, const int& old_rank_value = NOT_MATCH);

	friend ostream& operator<<(ostream& os, const PlayerStatistics& stat);
};

#endif