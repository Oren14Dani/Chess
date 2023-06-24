#ifndef SAVE_LOAD_GAME_H
#define SAVE_LOAD_GAME_H


#include "player.h"

class Save_Load_game {
private:
	fstream file_save_game;
	string m_file_game_name = { CHESS_SAVEGAME_FILE_FORM };
	game_log log; // follow the movments on the board. notice- saved automaticlly in flayer::make_move()
public:
	// run on every square on the board and record the piece parameters: type, color , moved
	// line in the file can be one of the piece parameters OR "None" in case of empty square 
	// notice -we rely on the file's line order while saveing and loading data
	void save_to_file(const int& num, Board& gameboard, const bool& player_last_turn);
	// run line by line in the file and get piece parameters : type, color , moved
	// put the piece in the board and in the same color piece set
	// notice that we rely on the file's line order while saveing and loading data
	bool load_game(int num, Board& gameboard, bool* player_last_turn);
	game_log* get_game_log();
};

#endif // !SAVE_LOAD_GAME_H