
#include "PlayerStatistics.h"

const string& PlayerStatistics::get_rank()const { return m_rank; }

const string& PlayerStatistics::get_key(int idx)const {
	if (idx >= 0 && idx <= 7)
		return keys[idx];
}
// creat new statistics or extract existent player statistics info form file   
// file name form is: player_name.txt
PlayerStatistics::PlayerStatistics(Rank* rank_class, const string& plr_name) : m_rank_class(rank_class), m_rank("empty"),
	player_file_name(plr_name + TXT), player_name(plr_name) {
	for (int i = 0; i < KEYS; i++)
		player_database[keys[i]] = 0;
	m_rank = rank_class->rank_type[0];
}
const int& PlayerStatistics::get_points() { return player_database.at(POINTS); }
const string& PlayerStatistics::get_player_file_name() { return player_file_name; }
const string& PlayerStatistics::get_player_name() { return player_name; }
bool PlayerStatistics::fill_memory(const string& plr_name, bool new_player ) {
	if (new_player) {
		return save_static_file(); // open new file for this player
	}

	else { // existent player

		m_file.open(player_file_name, ios_base::in);
		if (m_file.fail()) {
			cout << "Error opeing a file" << endl;
			m_file.close();
			return false;
		}
		string  line;

		for (int i = 0; i < KEYS; i++) {
			getline(m_file, line);// get player's data form the file line by line 	
			player_database[keys[i]] = stoi(line); // turn string to int values 
		}

		int clc_rank = player_database.at(POINTS) / SCORE_PER_RANK;
		update_rank(clc_rank);

		m_file.close();
		return true;
	}
}

void PlayerStatistics::inc_attack() {
	player_database.at(ATTACKS)++;
	player_database.at(POINTS)++;
}
void PlayerStatistics::inc_check() {
	player_database.at(CHECK)++;
	player_database.at(POINTS) += CHECH_SCORE;
}

void PlayerStatistics::edit_data_end_game(string win_lose_tie_surrndor) {
	player_database.at(GAME_PLAYED)++;
	int old_rank_value, new_rank_value;
	calc_rank(old_rank_value);
	update_points(ENDGAME, win_lose_tie_surrndor);
	calc_rank(new_rank_value);

	if (new_rank_value != old_rank_value)  // if equal then rank didnt changed -->  stay on the same level
		update_rank(new_rank_value, old_rank_value);
}
void PlayerStatistics::calc_rank(int& value) {
	value = (player_database.at(POINTS) / SCORE_PER_RANK);
	if (value < MIN_LEVEL_INDEX)
		value = MIN_LEVEL_INDEX;
}

// update data and points by the results of last game  
// win == +30 points
// lose == -10 points
// tie == +10 point
// surrndor = -5 points
// check = +2 points
// attacks == +1 points
void PlayerStatistics::update_points(const string& action, const string& win_lose_tie_suur ) {
	if (action == ENDGAME) {
		player_database.at(win_lose_tie_suur)++;
		if (win_lose_tie_suur == WIN)
			player_database.at(POINTS) += WIN_SCORE;
		else if (win_lose_tie_suur == LOSE)
			player_database.at(POINTS) += LOSE_SCORE;
		else if (win_lose_tie_suur == TIE)
			player_database.at(POINTS) += TIE_SCORE;
		else if (win_lose_tie_suur == SURRNDOR)
			player_database.at(POINTS) += SURRNDOR_SCORE;
	}
	//that update happend for between turns 
	else if (action == CHECK)
		inc_check();
	else if (action == ATTACKS)
		inc_attack();
	this->save_static_file();
}
const map<string, int>& PlayerStatistics::get_statsic_data() const { return player_database; }

bool PlayerStatistics::save_static_file() {
	m_file.open(player_file_name, ios_base::out); // if it is a new player then open new file
	if (m_file.fail()) {
		cout << "Error opeing a file" << endl;
		m_file.close();
		return false;
	}

	for (int i = 0; i < KEYS; i++) {
		m_file << player_database.at(keys[i]) << endl;
	}
	m_file.close();
	return true;
}

// update rank if the new rank is higher then the old rank 
void PlayerStatistics::update_rank(const int& new_rank_value, const int& old_rank_value ) {
	if (new_rank_value <= TOP_LEVEL_INDEX && new_rank_value >= MIN_LEVEL_INDEX) {
		m_rank = m_rank_class->rank_type[new_rank_value]; // from the string array

		if (old_rank_value != NOT_MATCH) { // use only when old_rank a
			if (old_rank_value < new_rank_value)
				cout << "LEVEL UP!" << endl;
			else
				cout << "LEVEL DOWN!" << endl;
		}
	}
	else if (new_rank_value > TOP_LEVEL_INDEX)
		m_rank = m_rank_class->rank_type[TOP_LEVEL_INDEX];
	else if (new_rank_value < MIN_LEVEL_INDEX)
		m_rank = m_rank_class->rank_type[MIN_LEVEL_INDEX];
}

	ostream& operator<<(ostream& os, const PlayerStatistics& stat) {
	for (int i = 0; i < KEYS; i++)
		os << stat.keys[i] << ": " << stat.player_database.at(stat.keys[i]) << endl;

	return os;
}

