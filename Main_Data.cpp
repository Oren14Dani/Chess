#include "Main_Data.h"


Main_Data::Main_Data() {
	launcher_users_data();
	launcher_games_counter();
}
Main_Data::~Main_Data() {
	list<pair<int, string>>::iterator it = hall_of_fame_data.begin();
	for (; it != hall_of_fame_data.end(); it++)
		delete players_data_base.at(it->second);
}
//launch game initial data and read players data by names
void Main_Data::launcher_users_data() {
	num_of_players = 0;

	file_data_base.open(CHESS_GAME_USERS_TXT, ios_base::in);
	if (file_data_base.fail()) {// mean that no have players on data and games been played!!
		file_data_base.close();
		file_data_base.open(CHESS_GAME_USERS_TXT, ios_base::out); //create new file
		file_data_base.close();
		return;
	}

	string name;
	while (1) {
		getline(file_data_base, name); // read file of players names
		if (file_data_base.eof()) // eof() turn true after 1 more itraction then needed 
			break;
		if (!add_user_to_DB(name))
			cout << "Error was appear while geting memory of player" << name << "statistics " << endl;
	}
	file_data_base.close();

	hall_of_fame_data.sort(); // sort by points 
}
// first line in file determain the number of total games ever played
void Main_Data::launcher_games_counter() {
	if (!read_game_counter_file()) {
		games_counter = 0;
		game_counter_file.open(CHESS_GAME_COUNTER_TXT, ios_base::out); //// didnt find file then create new file
		game_counter_file << 0 << endl;
		game_counter_file.close();
	}
}
// game_counter determain the number of total games ever played
bool Main_Data::read_game_counter_file() {
	string games_count;
	game_counter_file.open(CHESS_GAME_COUNTER_TXT, ios_base::in);
	if (game_counter_file.fail()) {
		cout << "Error opeing a file" << endl;
		game_counter_file.close();
		return false;
	}
	getline(game_counter_file, games_count);
	games_counter = stoi(games_count);
	game_counter_file.close();
	return true;
}
void Main_Data::write_games_counter_file() {
	game_counter_file.open(CHESS_GAME_COUNTER_TXT, ios_base::out);
	if (game_counter_file.fail()) {
		cout << "Error opeing a file" << endl;
		game_counter_file.close();
		return;
	}
	game_counter_file << games_counter;
	game_counter_file.close();
}
//insert_player_name_to_MD_file
bool Main_Data::add_user_to_DB(const string& name, bool new_player) {
	try {
		players_data_base.at(name);
		cout << "user already exsist" << endl;
		return false;
	}
	catch (out_of_range) {
		PlayerStatistics* tmp = new PlayerStatistics(&general_rank, name);
		if (tmp->fill_memory(name, new_player)) { // read from player file
			players_data_base[tmp->get_player_name()] = tmp;
			hall_of_fame_data.push_back(make_pair(tmp->get_points(), tmp->get_player_name()));
			num_of_players++;
			return true;
		}
		else {
			cout << "Error was appear while geting memory of player" << name << "statistics " << endl;
			return false;
		}
	}
}
bool Main_Data::add_user_to_file(const string& name) {
	try {
		players_data_base.at(name);
		cout << "user already exsist" << endl;
		return false;
	}
	catch (out_of_range) {
		file_data_base.open(CHESS_GAME_USERS_TXT, ios_base::app);
		if (file_data_base.fail()) {
			cout << "Error opeing a file" << endl;
			file_data_base.close();
			return false;
		}
		file_data_base << name << endl;
		file_data_base.close();
	}
	return true;
}
bool Main_Data::update_player_file(const string& name) {
	if (players_data_base.at(name)->save_static_file())
		return true;
	else
		return false;
}
void Main_Data::update_player_database(const string& name, const string& win_lose_tie_suur) {
	players_data_base.at(name)->update_points(ENDGAME, win_lose_tie_suur);

	list<pair<int, string>>::iterator it = hall_of_fame_data.begin();
	for (; it != hall_of_fame_data.end(); it++) {
		if ((it->second) == name)
			(it->first) = players_data_base.at(name)->get_points();

	}

}
// sort by points 
void Main_Data::sort_hall_of_fame() { hall_of_fame_data.sort(); }
// use reverse iterator to print the plars ordered by their points from to high to low
void Main_Data::show_hall_of_fame() const {
	clrscr();
	cout << " HALL OF FAME: " << endl;
	if (!hall_of_fame_data.size())
		cout << IS_EMPTY << endl;
	else {
		list<pair<int, string>> ::const_reverse_iterator rit;
		rit = hall_of_fame_data.rbegin();
		for (int order = 1; rit != hall_of_fame_data.rend(); rit++)
			cout << "Place No. " << order++ << ": player " << (rit->second) << " have " << (rit->first) <<" "<< POINTS << endl;
	}
}
void Main_Data::delete_user(const string& name) {
	try {
		//erase from data base
		delete players_data_base.at(name); //μαγεχ
		players_data_base.erase(name);

	}
	catch (out_of_range) {
		cout << "this user not exsist" << endl;
		return;
	}

	//erase from hall of fame and bulid new file of users names
	file_data_base.open(CHESS_GAME_USERS_TXT, ios_base::out);
	if (file_data_base.fail()) {
		cout << "Error opeing a file" << endl;
		file_data_base.close();
		return;
	}
	list<pair<int, string>>::iterator it = hall_of_fame_data.begin();
	for (; it != hall_of_fame_data.end(); it++) {
		if ((it->second) == name)
			hall_of_fame_data.erase(it);
		else
			file_data_base << (it->second) << endl;
	}

	file_data_base.close();
	return;
}
int& Main_Data::get_games_counter() { return games_counter; }
void Main_Data::inc_games_counter() {
	games_counter++;
	write_games_counter_file();
}
PlayerStatistics* Main_Data::get_player_data(const string& name) {
	try {
		return  players_data_base.at(name);
	}
	catch (out_of_range) {
		cout << "this user not exsist" << endl;
		return nullptr;
	}
}
const list<pair<int, string>>& Main_Data::get_hall_of_fame()const { return hall_of_fame_data; }
void Main_Data::print_chess_game_rules() const {
	do {
		CHESS_RULES_TXT;
		ifstream temp_game_rules;
		temp_game_rules.open(CHESS_RULES_TXT, ios_base::in);
		if (temp_game_rules.fail()) {
			cout << "can't find a rules file" << endl;
			temp_game_rules.close();
			break;
		}
		string line;
		clrscr();
		while (1) {
			getline(temp_game_rules, line);
			if (temp_game_rules.eof()) // eof() turn true after 1 more itraction then needed 
				break;
			cout << line << endl;
		}
		temp_game_rules.close();
		break;
	} while (1);
}

