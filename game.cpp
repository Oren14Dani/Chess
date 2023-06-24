#include "Chess.h"


Game_engine::Main_Data::Main_Data() {
		launcher_users_data();
		launcher_games_counter();
	}
Game_engine::Main_Data::~Main_Data() {
		list<pair<int, string>>::iterator it = hall_of_fame_data.begin();
		for (; it != hall_of_fame_data.end(); it++)
			delete players_data_base.at(it->second);
	}
//launch game initial data and read players data by names
void Game_engine::Main_Data::launcher_users_data() {
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
void Game_engine::Main_Data::launcher_games_counter() {
	if (!read_game_counter_file()) { 
		games_counter = 0;
		game_counter_file.open(CHESS_GAME_COUNTER_TXT, ios_base::out); //// didnt find file then create new file
		game_counter_file << 0 << endl;
		game_counter_file.close();
	}
}
// game_counter determain the number of total games ever played
bool Game_engine::Main_Data::read_game_counter_file() {
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
void Game_engine::Main_Data::write_games_counter_file() {
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
bool Game_engine::Main_Data::add_user_to_DB(const string& name, bool new_player ) {
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
bool Game_engine::Main_Data::add_user_to_file(const string& name) {
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
bool Game_engine::Main_Data::update_player_file(const string& name) {
	if (players_data_base.at(name)->save_static_file())
		return true;
	else
		return false;
}
void Game_engine::Main_Data::update_player_database(const string& name, const string& win_lose_tie_suur ) {
	players_data_base.at(name)->update_points(ENDGAME, win_lose_tie_suur);

	list<pair<int, string>>::iterator it = hall_of_fame_data.begin();
	for (; it != hall_of_fame_data.end(); it++) {
		if ((it->second) == name)
			(it->first) = players_data_base.at(name)->get_points();
			
	}

}
// sort by points 
void Game_engine::Main_Data::sort_hall_of_fame() { hall_of_fame_data.sort(); }
// use reverse iterator to print the plars ordered by their points from to high to low
void Game_engine::Main_Data::show_hall_of_fame() const {
	clrscr();
	cout << " HALL OF FAME: " << endl;
	if (!hall_of_fame_data.size())
		cout << IS_EMPTY <<endl;
	else {
		list<pair<int, string>> ::const_reverse_iterator rit;
		rit = hall_of_fame_data.rbegin();
		for (int order = 1; rit != hall_of_fame_data.rend(); rit++)
			cout << "Place No. " << order++ << ": player " << (rit->second) << " have " << (rit->first) << POINTS << endl;
	}
	system(PAUSE);
}
void Game_engine::Main_Data::delete_user(const string& name) {
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
		return ;
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
int& Game_engine::Main_Data::get_games_counter() { return games_counter; }
void Game_engine::Main_Data::inc_games_counter() {
	games_counter++;
	write_games_counter_file();
}
PlayerStatistics* Game_engine::Main_Data:: get_player_data(const string& name) {
	try {
		return  players_data_base.at(name);
	}
	catch (out_of_range) {
		cout << "this user not exsist" << endl;
		return nullptr;
	}
}
const list<pair<int, string>>& Game_engine::Main_Data::get_hall_of_fame()const { return hall_of_fame_data; }
void Game_engine::Main_Data::print_chess_game_rules() const {
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
	}while (1);
system("pause");
}


	
// run on every square on the board and record the piece parameters: type, color , moved
// line in the file can be one of the piece parameters OR "None" in case of empty square 
// notice -we rely on the file's line order while saveing and loading data
// notice - game log sa
void Game_engine::Save_Load_game::save_to_file(const int& num, Board& gameboard, const bool& player_last_turn) {
	string sp_data[3];
	Piece* sp;

	string file_game_name = m_file_game_name ;
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
void Game_engine::Save_Load_game::load_game(int num, Board& gameboard, bool* player_last_turn) {
	string sp_data[3]; // piece name, color, moved/not moved
	Piece* sp;
	if (!log.load_game_log(num))
		return;
	//check if the pieces list is already full or empty and ready to insert
	if (&gameboard != nullptr) {

	}
	string file_game_name= m_file_game_name;
	file_game_name.append(to_string(num) + TXT);

	file_save_game.open(file_game_name, ios_base::in);
	if (file_save_game.fail()) {
		cout << "Error opeing a file" << endl;
		file_save_game.close();
		return;
	}

	//read how turn now in the first line 
	string last_turn;
	getline(file_save_game, last_turn);
	if (last_turn == BLACK )
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
			for (int i = 1; i <= 2 ; i++)
				getline(file_save_game, sp_data[i]);

			bool color = sp_data[1] == BLACK ? YES : NO;
			sp = gameboard.get_set(color)->restore_one_piece(sp_data, gameboard.get_key(row, col));

			gameboard.insert_one_piece(sp_data[1], sp);
			gameboard.get_mapboard()[gameboard.get_key(row, col)] = sp;
		}
		
	}
	file_save_game.close();
}
game_log* Game_engine::Save_Load_game::get_game_log()  { return &log; }
 

	 
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
				cout << "invalid input" << endl;
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
				cout << "invalid input" << endl;
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
			reset_game_system();
		}
		else
			shut_down_system();// in that case a system  bug happened
	}

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
	pause_game = false;
	player_last_turn = false;


	reset_game_system(); // clean leftovers from old games 
		 

	gameboard = new Board;
	gameboard->fresh_new_board();

	if (!login_2players())
		return;
		
	MD.inc_games_counter();
	m_save_load_game.get_game_log()->start_new_game_log();
	game_mainloop();

	// here the game ends
	// now return to menu...
}
void Game_engine::reset_game_system() {
	if (gameboard)
		delete gameboard;
	if (black)
		delete black;
	if (white)
		delete white;
	defulte_parameters();
}
//return the winner name
void Game_engine::game_mainloop() {
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
	system("pause");
}
void Game_engine::save_game() {
	if(pause_game)
		m_save_load_game.save_to_file(*m_save_load_game.get_game_log()->get_game_number() ,*gameboard, player_last_turn);
	else
		cout << "There is no game in the memory to save" << endl;
	system(PAUSE);
}
bool Game_engine::load_game() {
	clrscr();
	do {
		if (MD.get_games_counter() <= 0) { // no games yet oucuuer 
			cout << IS_EMPTY << endl;
			system(PAUSE);
			return false;;
		}
		cout << "to load game enter the saved game's number (can be only form " << MD.get_games_counter() << " to 1 ):" << endl;
		cout << "enter 0 to go back menu" << endl;
		char game_number;
		cin >> game_number;
		game_number -= INT2CHAR;

		if (game_number > 0 && game_number <= MD.get_games_counter()) { // check valid game_number
			if (gameboard && pause_game) {// there is an open game already
				ask_if_to_save_the_cur_gmae();
			}
			reset_game_system();
			gameboard = new Board;
			pause_game = true; // assume that load game is always an interrupted game. important for mainloop later
			cur_game_num = (int)(game_number);
			m_save_load_game.get_game_log()->set_cur_game_num(&cur_game_num);
			m_save_load_game.load_game((int)game_number,*gameboard, &player_last_turn);
			return true; // load succsfuel
		}
		else if (game_number == 0) //go to menu
			return false;
		else
			cout << INVALID_INPUT << endl;
	} while (1);
}
void Game_engine::ask_if_to_save_the_cur_gmae() {
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
			cout << "invalid input" << endl;
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
			cout << "invalid input" << endl;
	} while (1);

}
	
void Game_engine::players_statistics(){
	clrscr();
		
	if(!MD.get_hall_of_fame().size())
		cout<< "users list is empty"<<endl;
	else {
		list<pair<int, string>>::const_iterator it = MD.get_hall_of_fame().begin();
		show_title_of_player_statistics(it->second);
		gotoxy(0, 1);
		for (int line = 1; it != MD.get_hall_of_fame().end(); line++, it++) {
			player_statistics(it->second, line);
		}
	}
	system("pause");
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
				cout << "invalid input" << endl;
		} while (true);
	}
	return;
}
	
ostream& operator<<(ostream& os,  Game_engine& game) {
	os << *(game.gameboard);
	game.m_save_load_game.get_game_log()->print_last_moves_and_game_num();
	return os;
}
	


