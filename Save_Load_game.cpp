#include "Save_Load_game.h"

// run on every square on the board and record the piece parameters: type, color , moved
// line in the file can be one of the piece parameters OR "None" in case of empty square 
// notice -we rely on the file's line order while saveing and loading data
// notice - game log sa
void Save_Load_game::save_to_file(const int& num, Board& gameboard, const bool& player_last_turn) {
	if (!gameboard.get_mapboard().size()) //no game to save
		return;
		
	string sp_data[3];
	Piece* sp;

	string file_game_name = m_file_game_name;
	file_game_name.append(to_string(num) + TXT);

	file_save_game.open(file_game_name, ios_base::out);
	if (file_save_game.fail()) {
		cout << "Error opeing a file" << endl;
		file_save_game.close();
		return;
	}

	//write how turn now in the first line 
	string last_turn = player_last_turn ? BLACK : WHITE;
	file_save_game << last_turn << endl;

	// run on every square on the board and record the piece parameters
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			sp = gameboard.get_mapboard().at(gameboard.get_key(row, col));
			if (sp != nullptr) {
				sp_data[0] = sp->who_am_i();
				sp_data[1] = sp->get_color() ? BLACK : WHITE;
				sp_data[2] = sp->get_piece_moved() ? "MOVED" : "NOT_MOVED";
				for (int i = 0; i < 3; i++)
					file_save_game << sp_data[i] << endl;
			}
			else {
				file_save_game << "None" << endl;
			}
		}
	}
	file_save_game.close();
}
// run line by line in the file and get piece parameters : type, color , moved
// put the piece in the board and in the same color piece set
// notice that we rely on the file's line order while saveing and loading data
bool Save_Load_game::load_game(int num, Board& gameboard, bool* player_last_turn) {
	string sp_data[3]; // piece name, color, moved/not moved
	Piece* sp;
	if (!log.load_game_log(num))
		return false;
	
	string file_game_name = m_file_game_name;
	file_game_name.append(to_string(num) + TXT);

	file_save_game.open(file_game_name, ios_base::in);
	if (file_save_game.fail()) {
		cout << "Error opeing a file" << endl;
		file_save_game.close();
		return false;
	}

	//read how turn now in the first line 
	string last_turn;
	getline(file_save_game, last_turn);
	if (last_turn == BLACK)
		*player_last_turn = true;
	else
		*player_last_turn = false;

	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			getline(file_save_game, sp_data[0]);
			if (sp_data[0] == "None") {
				gameboard.get_mapboard()[gameboard.get_key(row, col)] = nullptr;
				continue;
			}
			for (int i = 1; i <= 2; i++)
				getline(file_save_game, sp_data[i]);

			bool color = sp_data[1] == BLACK ? YES : NO;
			sp = gameboard.get_set(color)->restore_one_piece(sp_data, gameboard.get_key(row, col));

			gameboard.insert_one_piece(sp_data[1], sp);
			gameboard.get_mapboard()[gameboard.get_key(row, col)] = sp;
		}

	}
	file_save_game.close();
	return true;
}
game_log* Save_Load_game::get_game_log() { return &log; }

