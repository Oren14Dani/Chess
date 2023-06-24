#include "game_log.h"


void game_log::set_cur_game_num(int* games_num) { cur_game_num = games_num; }
// open new empty game log file 
void game_log::start_new_game_log() {
	string new_file_name = GAME_LOG_FILE_FORM;
	new_file_name += to_string(*cur_game_num) + TXT;
	m_file.open(new_file_name, ios_base::out); // start at the end of the file and continue
	if (m_file.fail()) {
		cout << "Error createing game log file" << endl;
		m_file.close();
		return;
	}
	m_file.close();

	m_game_log.clear(); // reset and get ready for new game
}

// if it is an existing game_log then load his file data to the list in memory
bool game_log::load_game_log(int& game_number_to_load) {
	reset_game_log();
	set_cur_game_num(&game_number_to_load);

	string new_file_name(GAME_LOG_FILE_FORM);
	new_file_name += to_string(game_number_to_load) + TXT;
	m_file.open(new_file_name, ios_base::in);
	if (m_file.fail()) {
		cout << "Error opeing a log file" << endl;
		m_file.close();
		return false;
	}
	string line;

	while (1) {
		getline(m_file, line);
		if (m_file.eof()) // eof() turn true after 1 more itraction then needed 
			break;
		m_game_log.emplace_back(line); // last move will be the first in the list
	}
	m_file.close();
	return true;

}

string game_log::clc_string_for_game_log(Piece* sp, const square cur_loc, const square dest, bool attack , bool check ) {
	string tmp;
	tmp = (sp->who_am_i()) + " " + (sp->get_color() ? BLACK : WHITE) + " " + (cur_loc.sqr_to_string()) + " " + (dest.sqr_to_string());
	if (attack)
		tmp = tmp + " " + ATTACKS;
	if (check)
		tmp = tmp + " " + CHECK;

	return tmp;
}

void game_log::print_previous_game_log() const {
	do {
		string user_choice;
		cout << " enter the number of the wanted game log (same as game number)" << endl;
		cout << " or enter 0 to go back to menu" << endl;
		cin >> user_choice;

		if (user_choice == "0") //go to menu
			break;

		else if (user_choice.find_first_not_of(VALID_NUMBERS) != NOT_MATCH) // -1 mean that didnt find 
			cout << INVALID_INPUT << endl;

		else {
			ifstream temp_game_log;
			string new_file_name = GAME_LOG_FILE_FORM;
			new_file_name += user_choice + TXT;
			temp_game_log.open(new_file_name, ios_base::in);
			if (temp_game_log.fail()) {
				cout << "can't find a log file" << endl;
				temp_game_log.close();
				break;
			}
			string line;
			clrscr();
			cout << "the current game number is:" << user_choice << endl;
			cout << "the game log is:" << endl;
			while (1) {
				getline(temp_game_log, line);
				if (temp_game_log.eof()) // eof() turn true after 1 more itraction then needed 
					break;
				cout << line << endl;
			}
			temp_game_log.close();
			break;
		}
	} while (1);
}

void game_log::print_cur_game_log() const {
	cout << "the current game number is:" << *cur_game_num << endl;
	cout << "the game log is:" << endl;

	if (m_game_log.size() == 0)
		cout << IS_EMPTY << endl;

	else {
		list<string>::const_iterator it;
		it = m_game_log.begin();
		for (int count = 0; it != m_game_log.end(); it++, count++) {
			gotoxy(30, count + 2);
			cout << "move No." << count << ": " << *(it) << endl;
		}
	}
}

void game_log::print_last_moves_and_game_num() const {
	gotoxy(28, 3);
	cout << "the current game number is:" << *cur_game_num;

	gotoxy(30, 4);
	cout << "Last " << NUM_OF_MOVES_TO_PRINT << " moves are:" << endl;
	list<string>::const_reverse_iterator it;
	it = m_game_log.rbegin();
	for (int count = 0; (it != m_game_log.rend()) && (count < NUM_OF_MOVES_TO_PRINT); it++, count++) {
		gotoxy(30, count + 5);
		cout << "move No." << (m_game_log.size() - count) << ": " << *(it) << endl;
	}

	gotoxy(0, 11); // get back to rest game printing
}

// save the last movement to file of game log
bool game_log::save_last_move(const string& last_move_log) {
	m_file.open(GAME_LOG_FILE_FORM + to_string(*cur_game_num) + TXT, ios_base::app); // start at the end of the file and continue
	if (m_file.fail()) {
		cout << "Error opeing a file" << endl;
		m_file.close();
		return false;
	}
	m_game_log.push_back(last_move_log);
	m_file << last_move_log << endl; //OR  m_game_log.back()

	m_file.close();
	return true;
}

void game_log::save_game_log() {
	string new_file_name(GAME_LOG_FILE_FORM);
	new_file_name += to_string(*cur_game_num) + TXT;
	m_file.open(new_file_name, ios_base::app); // start at the end of the file and continue
	if (m_file.fail()) {
		cout << "Error opeing a file" << endl;
		m_file.close();
		exit(1);
	}

	list<string>::const_iterator it;
	for (it = m_game_log.begin(); it != m_game_log.end(); it++) {
		m_file << *(it) << endl;
	}

	m_file.close();
}

list<string>& game_log::get_list() { return m_game_log; }
const int* game_log::get_game_number() const { return cur_game_num; }
void game_log::reset_game_log() {

	if (m_game_log.size())
		m_game_log.clear();
}