#include "Game_engine.h"

	 
Game_engine::Game_engine(){
	defulte_parameters();
	m_save_load_game.get_game_log()->set_cur_game_num(&MD.get_games_counter());
}
Game_engine::~Game_engine() {
	delete gameboard;
	delete 	white;
	delete 	black;
}
void Game_engine::defulte_parameters() {
	end_game = false;
	pause_game = false;
	players_are_login = false;
	player_last_turn = false;
	cur_game_num = NOT_MATCH;
	gameboard = nullptr;
	white = nullptr;
	black = nullptr;
}
void Game_engine::play_general() {
	while (1) {
		if (!make_action(menu()))
			break;
	}
	shut_down_system();
}
int Game_engine::menu() {
	char user_input;
	clrscr();
		
	if (!end_game) {
		do {
			cout << "Choose what to do next by entering the number:" << endl;
			cout << "1. start new game. " << endl;
			cout << "2. save curent game. " << endl;
			cout << "3. Load game. " << endl;
			cout << "4. Player statistics. " << endl;
			cout << "5. Hall of fame (Score table). " << endl;
			cout << "6. show game log. " << endl;
			cout << "7. back to game . " << endl;
			cout << "8. learn game rules . " << endl;
			cout << "9. Exit . " << endl;

			cin >> user_input;
			user_input = user_input - INT2CHAR;
			if (user_input_caser(user_input))
				return user_input;
			else
				cout << INVALID_INPUT << endl;
		} while (true);
	}
	else{
		do {
			cout << "1. if you want to play more game" << endl;
			cout << "2. disconnect and go to menu " << endl;
			cin >> user_input;
			user_input = user_input - INT2CHAR;
			if (user_input == 1 || user_input == 2)
				return user_input;
			else
				cout << INVALID_INPUT << endl;
		} while (true);
	}
}
bool Game_engine::user_input_caser(const int& user_input){
	if (user_input == 1 || user_input == 2 || user_input == 3 || user_input == 4 ||
				user_input == 5 || user_input == 6 || user_input == 7 || user_input == 8 || user_input == 9)
		return true;
	else
		return false;
}
bool Game_engine::make_action(const int action) {
	if (!end_game) {
		if ((action == 1))
			start_new_game(); //start new game
		else if (action == 2)
			save_game();
		else if (action == 3) {
			if (load_game())
				if (login_2players())
					game_mainloop();
		}
		else if (action == 4)
			players_statistics();
		else if (action == 5)
			hall_of_fame();
		else if (action == 6)
			show_game_log();
		else if (action == 7) {
			if (pause_game && gameboard)
				game_mainloop(); //countinue pasued game
			else
				cout << "couurent game " <<IS_EMPTY << endl;
		}
		else if (action == 8)
			print_game_rules();
		else if (action == 9) {
			exit_game();
			return false;
		}
		else
			shut_down_system(); // in that case a system  bug happened
	}

	else {
		if (action == 1)
			start_new_game(); //start new game
		else if (action == 2) {
			reset_game_system_memory();
		}
		else
			shut_down_system();// in that case a system  bug happened
	}
	system(PAUSE);
	return true; // to stay in infint game loop. can exit system only from exit_game() function.
}
// in case some system bug happened 
void Game_engine::shut_down_system() {
	cout << "already leaving? too bad..." << endl;
	cout << "we hope you had fun! and tell your friends :) " << endl;
	cout << "the game will shut down in a few seconds. " << endl;
	Sleep(1000);
	cout << "3 sec.." << endl;
	Sleep(1000);
	cout << "2 sec.." << endl;
	Sleep(1000);
	cout << "1 sec.." << endl;
	Sleep(1000);
	// BOOOOM
}
void Game_engine::print_game_rules()const {
	MD.print_chess_game_rules();
}
void Game_engine::start_new_game() {
	reset_game_system_memory(); // clean leftovers from old games 
	gameboard = new Board;
	gameboard->fresh_new_board();

	if (!login_2players())
		return;
		
	MD.inc_games_counter();
	cur_game_num = MD.get_games_counter();
	m_save_load_game.get_game_log()->start_new_game_log();
	game_mainloop();

	// here the game ends
	// now return to menu...
}
void Game_engine::reset_game_system_memory() {
	if (gameboard )
		delete gameboard;
	if (black)
		delete black;
	if (white)
		delete white;
	defulte_parameters();
}
//return the winner name
void Game_engine::game_mainloop() {
	m_save_load_game.get_game_log()->set_cur_game_num(&cur_game_num);
	if (pause_game) {
		cout << *this; 
		pause_game = false;
		if (player_last_turn) 
			player_turn(black);
	}
	if (pause_game||end_game)
		return;
		
	while (true) {// run untile one of the players lose
		cout << *this;
		player_turn(white);
		if (pause_game||end_game)
			break;
		cout << *this;
		player_turn(black);
		if (pause_game||end_game)
			break;
	}
}
bool Game_engine::login_2players() {
	clrscr();
	char user_input;
	int count = 1; // 1 for white player, 2 for black player
	string user_name;
	cout << "Wellcome to Play-Chess."<<endl;
	cout << "2 players need to login." << endl;
	do {
		cout << (count == 1 ? BLACK:WHITE) << " player , ";
		cout << "Do you already have Play-Chess user? "<<endl;
		cout << "1.Yes. "<<endl;
		cout << "2.No. "<<endl;
		cout << "3.exit to menu . "<<endl;

		cin >> user_input;
		user_input = user_input - INT2CHAR;
		if (user_input ==3)
			return false;

		else if (user_input == 1 || user_input == 2) {
			do {
				cout << "enter user name:";
				cin >> user_name;
			} while (!check_valid_name(user_name));

			if (user_input == 1) { // existing user
				if (login_user(count, user_name)) {// true for new user
					count++;
					cout << "your login was successful" << endl;
				}
			}
			else if (user_input == 2) {//new user
				if (!MD.add_user_to_file(user_name))
					continue;
				if (!MD.add_user_to_DB(user_name, true))
					continue;
				if (login_user(count, user_name)) {// true for new user
					count++;
					cout << "your registration was successful" << endl;
				}
			}
		}
		else
			cout << " invalid input" << endl;
	} while (count <= 2); //need 2 palyers for chess game

	players_are_login = true;
	return true;
}
bool Game_engine::check_valid_name(const string& name) { //'a' - 'z' || 'A' - 'Z'
	int idx = name.find_first_not_of(valid_charcters_for_name); // return the index of the unmach char
	if (idx == NOT_MATCH) // -1 mean that not found first_not_of
		return true;
	else {
		cout << name[idx] << "kind of this letter can not be excepted" << endl;
		return false;
	}
}
bool Game_engine::login_user(int num_of_player, const string& name) {
	if (!MD.get_player_data(name))
		return false;

	//check if the players have same name
	if (black) 
		if (black->get_name() == name) {
			cout << " you are already connected as black player" << endl;
			return false;
		}
	if (white){
		if (white->get_name() == name) {
			cout << " you are already connected as white player" << endl;
			return false;
		}
	}

	if (num_of_player == 1) { //black
		black = new Player(gameboard->get_set(YES), gameboard, true, MD.get_player_data(name), m_save_load_game.get_game_log());
		if (!black)
			return false;
		black->set_name(name);
		return true;
	}
	else if (num_of_player == 2) {  //white
		white = new Player(gameboard->get_set(NO), gameboard, false, MD.get_player_data(name), m_save_load_game.get_game_log());
		if (!white)
			return false;
		white->set_name(name);
		return true;
	}
	return false;
}
void Game_engine::player_turn(Player* plr) {
	char ch;
	string what_to_do;
	square cur_sqr, dest_sqr;
	Piece* sp = nullptr;

	if (plr->make_move(what_to_do, sp, cur_sqr, dest_sqr)) { // valid move
		if (what_to_do != VALID_SQUARE_INPUT) {
			player_last_turn = plr->get_color();
			pause_game = true;
			if (what_to_do == TIE) {
				cout << "Hey amigo, lets declare tie!" << endl;
				end_game_formarl(plr, TIE, TIE);
			}
			else if (what_to_do == SURRNDOR) {
				cout << ((plr == white) ? black->get_name() : white->get_name()) <<" is the king, I surrndor!" << endl;
				end_game_formarl(plr, SURRNDOR, WIN);
			}
		}
	}
	else { // case of gameover - current player lose
		cout << ((plr == white) ? black->get_name() : white->get_name()) << " is the winner!" << endl;
		end_game_formarl(plr, LOSE, WIN);
	}
}
void Game_engine::end_game_formarl(Player* plr, const string& plr_res ,const string& other_res){
	MD.update_player_database(plr->get_name(), plr_res);
	MD.update_player_database((plr == white) ? black->get_name() : white->get_name(), other_res);
	MD.update_player_file(plr->get_name());
	MD.update_player_file((plr == white) ? black->get_name() : white->get_name());
	end_game = true;
	MD.sort_hall_of_fame();
	
}
void Game_engine::save_game() {
	if(pause_game)
		m_save_load_game.save_to_file(*m_save_load_game.get_game_log()->get_game_number() ,*gameboard, player_last_turn);
	else
		cout << "There is no game in the memory to save" << endl;
}
bool Game_engine::load_game() {
	clrscr();
	do {
		if (MD.get_games_counter() <= 0) { // no games yet oucuuer 
			cout << IS_EMPTY << endl;
			return false;;
		}
		cout << "to load game enter the saved game's number (can be only form " << MD.get_games_counter() << " to 1 ):" << endl;
		cout << "enter 0 to go back menu" << endl;
		string game_number_stirng;
		cin >> game_number_stirng;
		if (game_number_stirng.find_first_not_of(VALID_NUMBERS) != NOT_MATCH) { // NOT_MATCH mean that didnt find non numbers chars
			cout << INVALID_INPUT << endl;
			continue;
		}
		int game_number = stoi(game_number_stirng);

		if (game_number > 0 && game_number <= MD.get_games_counter()) { // check valid game_number
			if(gameboard && pause_game)// there is an open game already
				if (gameboard->get_mapboard().size() ) {// and the gmae board is not empty
					ask_if_to_save_the_cur_game();
				}
			reset_game_system_memory(); //check if the pieces list is already full or empty and ready to insert
			gameboard = new Board;
			pause_game = true; // assume that load game is always an interrupted game. important for mainloop later
			if(	!m_save_load_game.load_game(game_number,*gameboard, &player_last_turn)) //didnt find saved game file
				return false;
			cur_game_num = game_number;
			m_save_load_game.get_game_log()->set_cur_game_num(&cur_game_num);
			return true; // load succsfuel
		}
		else if (game_number == 0) //go to menu
			return false;
		else
			cout << INVALID_INPUT << endl;
	} while (1);
}
void Game_engine::ask_if_to_save_the_cur_game() {
	if (!gameboard)
		return;
	if (!gameboard->get_mapboard().size())
		return;
	do {
		
		char user_choice;
		cout << "do you want to save the cur game?" << endl;
		cout << "1.  Yes" << endl;
		cout << "2.  No" << endl;

		cin >> user_choice;
		user_choice -= INT2CHAR;

		if (user_choice == 1) { //save and go load
			save_game();
			break;
		}
		else if (user_choice == 2)//go to load
			break;
		else
			cout << INVALID_INPUT << endl;
	} while (1);
}
void Game_engine::show_game_log() {
	clrscr();
	do {
		char user_choice;
		cout << " which game log you whish ?" << endl;
		cout << "1.  cur game " << endl;
		cout << "2.  previous game " << endl;
		cout << "0.  go back to menu" << endl;
		cin >> user_choice;
		user_choice -= INT2CHAR;

		if (user_choice == 1) { // cur game
			m_save_load_game.get_game_log()->print_cur_game_log();
			break;
		}
		if (user_choice == 2) { //previous game 
			m_save_load_game.get_game_log()->print_previous_game_log();
			break;
		}
		else if (user_choice == 0)//go to menu
			break;
		else
			cout << INVALID_INPUT << endl;
	} while (1);

}
	
void Game_engine::players_statistics(){
	clrscr();
		
	if(!MD.get_hall_of_fame().size())
		cout<< "users list "<< IS_EMPTY <<endl;
	else {
		list<pair<int, string>>::const_iterator it = MD.get_hall_of_fame().begin();
		show_title_of_player_statistics(it->second);
		gotoxy(0, 1);
		for (int line = 1; it != MD.get_hall_of_fame().end(); line++, it++) {
			player_statistics(it->second, line);
		}
	}

}
void Game_engine::player_statistics(const string& name ,int line) {
	gotoxy(0, line);
	PlayerStatistics* ps = MD.get_player_data(name);
	const map<string, int> p_stat = ps->get_statsic_data();
	cout << name;
	gotoxy(7, line);
	cout<<ps->get_rank();
	for (int x = 0; x < 8; x++) {
		gotoxy((x + 1) * 7 + 7, line);
		cout << p_stat.at(ps->get_key(x));
	}
	cout << endl;
}
void Game_engine::show_title_of_player_statistics(const string& name){
	cout << "name";
	gotoxy(7, 0);
	cout << "rank";
	PlayerStatistics* ps = MD.get_player_data(name);
	for (int x = 0; x < 8; x++) {
		gotoxy((x + 1) * 7 + 7, 0);
		cout << ps->get_key(x);
	}
}
void Game_engine::hall_of_fame() {
	MD.show_hall_of_fame();
}
void Game_engine::exit_game() {
	int user_input;
	clrscr();
	if (end_game){
		return ;
	}
			
	if (pause_game) {
		do {
			cout << "do you want to save the last game?" << endl;
			cout << "1. Yes. " << endl;
			cout << "2. No. " << endl;
			cin >> user_input;

			if (user_input == 1) {
				m_save_load_game.save_to_file(MD.get_games_counter(),*gameboard, player_last_turn);
				return ; 
			}
			if (user_input == 2)
				return ; 
			else
				cout << INVALID_INPUT << endl;
		} while (true);
	}
	return;
}
	
ostream& operator<<(ostream& os,  Game_engine& game) {
	os << *(game.gameboard);
	game.m_save_load_game.get_game_log()->print_last_moves_and_game_num();
	return os;
}
	


