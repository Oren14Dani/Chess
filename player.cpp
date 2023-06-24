#include "player.h"

Player::Player(Piece_set* set, Board* Gboard, const bool clr, PlayerStatistics* plr_stat, game_log* gl) :
	m_color(clr), gameboard(Gboard), my_set(set), m_plr_stat(plr_stat), log(gl)
{
	map_board = &Gboard->get_mapboard();
	//cout << "enter the " << (m_color ? BLACK : WHITE) << " player's name : ";

	list<Piece*> my_setlist = my_set->get_listOFpicese();
	list<Piece*>::const_iterator it = my_setlist.begin();
	for (; it != my_setlist.end(); it++)
		if ((*it)->who_am_i() == KING) {
			my_king = static_cast<King*>(*it);
			break;
		}
	list<Piece*> rival_set = gameboard->get_set(!m_color)->get_listOFpicese();
	it = rival_set.begin();
	for (; it != rival_set.end(); it++)
		if ((*it)->who_am_i() == KING) {
			rival_king = static_cast<King*>(*it);
			break;
		}
}
const string& Player::get_name() const { return m_name; }
bool Player::make_move(string& what_to_do_if_not_square, Piece* sp , square& cur_sqr, square& dest_sqr ) {
	cout << (m_color ? BLACK : WHITE) << " turn now. " << endl;
	cout << "Enter your next move by piece square and destention square or one of the key words." << endl;
	cout << "square(ROW: 1 - 8, COL : a - h or A - H)"<<endl;
	sp = nullptr;
	bool attack_log=false;

	if (!my_king->get_check()) {//casual or attack
		do {
			sp = select_piece(cur_sqr, what_to_do_if_not_square);
			if (!sp && what_to_do_if_not_square != VALID_SQUARE_INPUT)
				return true; // go to menu  
			if (select_valid_destination(dest_sqr, what_to_do_if_not_square))
				if (deep_check(sp, dest_sqr)) {
					if (my_king->get_castling())
						castling(cur_sqr, dest_sqr);
					else {
						attack_log = move_pis_on_board(sp, cur_sqr, dest_sqr);
						if (!(sp->get_piece_moved()))
							sp->set_piece_moved(YES);
					}
					break;
				}
			if (what_to_do_if_not_square != VALID_SQUARE_INPUT)
				return true;
			cout << "This move is not allowed for that piece, choose another move..." << endl;
		} while (1);
	}

	else { // defence style 
		map<Piece*, map<square, bool >> sp_vaild_map;
		if (!run_esc_simulation(sp_vaild_map)) { //if you can't esacpe - you lose
			cout << "game over" << endl;
			return false;
		}
		cout << "your king is in CHECK! " << endl;

		do {
			sp = select_piece(cur_sqr, what_to_do_if_not_square);
			if (!sp && what_to_do_if_not_square != VALID_SQUARE_INPUT)
				return true;
			if (select_valid_destination(dest_sqr, what_to_do_if_not_square)) {
				try {
					if (sp_vaild_map.at(sp).at(dest_sqr)) {
						attack_log = move_pis_on_board(sp, cur_sqr, dest_sqr);
						my_king->set_check(NO);
						cout << "the " << (my_king->get_color() ? BLACK : WHITE) << " king is not in check any more. " << endl;
						my_king->get_attack_list().clear();
						break;
					}
				}
				catch (out_of_range) {}
			}
			if (what_to_do_if_not_square != VALID_SQUARE_INPUT)
				return true;
			cout << "This move is not allowed, choose another move..." << endl;
		} while (1);
	}
	
	check_rival_king(sp, cur_sqr, dest_sqr);
	pawn_promotion(sp);
	log->save_last_move(log->clc_string_for_game_log(sp, cur_sqr, dest_sqr, attack_log, rival_king->get_check() ));
	if(rival_king->get_check())
		m_plr_stat->update_points(CHECK); // player get 2 points 
	if(attack_log)
		m_plr_stat->update_points(ATTACKS); // player get 1 points 
	return true;
}
void Player::pawn_promotion(Piece* sp) {
	if (sp->who_am_i() == PAWN) {
		if ((sp->get_loc().get_row() == 7) || (sp->get_loc().get_row() == 0)) {
			my_set->promote_pawn(sp, map_board->at(sp->get_loc()) = choose_promotion(sp));
		}
	}
}
Piece* Player::choose_promotion(Piece* sp) {
	char ch = 0;
	do {
		cout << "the pawn is promoted. enter a leter to select a piece: Q = queen , N=knight , R=rook , B=bishop " << endl;
		cin >> ch;

		if (ch == 'Q')
			return new Queen(sp->get_color());
		else if (ch == 'N')
			return new Knight(sp->get_color());
		else if (ch == 'R')
			return new Rook(sp->get_color());
		else if (ch == 'B')
			return new Bishop(sp->get_color());
		else
			cout << "invalid input, choose again" << endl;

	} while (true);
}
// find out if the king can esc or one of my pieces can save the king 
// if false the there is nothing to do and the player lose.
bool Player:: run_esc_simulation(map<Piece*, map<square, bool >>& sp_vaild_map) { 
	//notice! we make use of the map that we fill here below later on the move
	square unit_vector(0, 0);
	square kingloc = my_king->get_loc();
	square next_kingloc = kingloc;
	Piece* tmp_p;

	// all king move options 
	sp_vaild_map[my_king][square(-1, -1) += kingloc] = YES;
	sp_vaild_map[my_king][square(-1, 0) += kingloc] = YES;
	sp_vaild_map[my_king][square(-1, 1) += kingloc] = YES;
	sp_vaild_map[my_king][square(0, -1) += kingloc] = YES;
	sp_vaild_map[my_king][square(0, 1) += kingloc] = YES;
	sp_vaild_map[my_king][square(1, -1) += kingloc] = YES;
	sp_vaild_map[my_king][square(1, 0) += kingloc] = YES;
	sp_vaild_map[my_king][square(1, 1) += kingloc] = YES;

	int cnt = sp_vaild_map.at(my_king).size();

	sp_vaild_map[my_king][kingloc] = NO; // just in case unit_vector return (0,0) (only for programer testing option)

	// elimination of move in threat's directions
	list<Piece*>::iterator it_threat = my_king->get_attack_list().begin();
	for (; it_threat != my_king->get_attack_list().end(); it_threat++) {
		clc_unit_vec((*it_threat)->get_loc(), my_king->get_loc(), unit_vector);
		sp_vaild_map.at(my_king).at(unit_vector * (-1) += kingloc) = NO; // unit_vector*(-1) because we need the oposite vector- from the king to the treat
		cnt--;
	}

	//chek if the king can esacpe by imself
	map<square, bool>::iterator it = sp_vaild_map.at(my_king).begin();
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++, next_kingloc = kingloc) {
			tmp_p = nullptr;
			square sqr_ij(i, j);

			if (!i && !j) // the king himselfe is on square(0,0) 
				continue;

			next_kingloc += sqr_ij;
			try {
				if (sp_vaild_map.at(my_king).at(next_kingloc)) //if YES then check what is there , else if already NO then continue
					tmp_p = map_board->at(next_kingloc);
				else
					continue;
			}
			catch (out_of_range) { //out of map boarders
				sp_vaild_map.at(my_king).at(next_kingloc) = NO;
				cnt--;
				continue;
			}
			if (tmp_p != nullptr && tmp_p->get_color() == m_color) { //king blocked by my soliders 
				sp_vaild_map.at(my_king).at(next_kingloc) = NO;
				cnt--;
				continue;
			}

			if (!king_safe_simulation(my_king, my_king->get_loc(), next_kingloc)) { //in case of empty sqaure or enemy 
				sp_vaild_map.at(my_king).at(next_kingloc) = NO;
				cnt--;
			}
		}
	}



	if (my_king->get_attack_list().size() > 1 && cnt == 0) // there is more than 1 treat on my king
		return false; //its a chess (it is a loss) ---> GAMEOVER !!!

	// here we find out if one of oure pieces can attack the threat or block his way to the king 
	bool success_flag = false;

	square dest(0, 0);
	it_threat = my_king->get_attack_list().begin();
	list<Piece*>::const_iterator it_def = my_set->get_listOFpicese().begin();
	clc_unit_vec((*it_threat)->get_loc(), my_king->get_loc(), unit_vector);
	for (dest = (*it_threat)->get_loc(); it_def != my_set->get_listOFpicese().end(); it_def++, dest = (*it_threat)->get_loc())
		if ((*it_def)->get_status())
			while (dest != my_king->get_loc()) { // run on all the asquares between the threat to the king
				if (deep_check((*it_def), dest)) {   //true if the threat can be remove !
					sp_vaild_map[(*it_def)][dest] = YES;
					success_flag = true;// the king can be saved
				}
				dest += unit_vector;
			}
	return success_flag || cnt > 0;
}
void Player:: castling(const square& cur_sqr, const square& dest_sqr) {
	move_pis_on_board(my_king, cur_sqr, dest_sqr);
	Rook* partner_rook = dynamic_cast<Rook*> (my_king->get_partner_rook());
	move_pis_on_board(partner_rook, partner_rook->get_loc(), partner_rook->get_castling_loc());
	my_king->set_castling(NO);
	my_king->set_piece_moved(YES);
	partner_rook->set_piece_moved(YES);
}
// calculate the unit vector from start square to end square
void Player:: clc_unit_vec(const square& start, const square& end, square& unit_vector) {// Piece* threat_p ,,Piece* which_king
	unit_vector.set_square(0, 0);
	int vec_row = end.get_row() - start.get_row();
	int vec_col = end.get_col() - start.get_col();

	if (vec_row && !vec_col || !vec_row && vec_col || abs(vec_row) == abs(vec_col)) { // vetor can be only row or col or diagonal
		if (!vec_row)
			unit_vector.set_square(0, vec_col / abs(vec_col));
		else if (!vec_col)
			unit_vector.set_square(vec_row / abs(vec_row), 0);
		else
			unit_vector.set_square(vec_row / abs(vec_row), vec_col / abs(vec_col)); // vector = (cur_sqr-riv_kingloc) without absolute value
	}
}
bool Player:: deep_check(Piece* sp, const square& dest) {

	if (sp->valid_move(dest, map_board)) {  //in point of view of the specfic piece
		if (my_king->get_castling())
			if (castling_simulation(dest))
				return true;
			else my_king->set_castling(NO);


		else if (check_free_way(sp->get_loc(), dest) || sp->who_am_i() == KNIGHT)
			if (king_safe_simulation(sp, sp->get_loc(), dest))  // to check if my king will be safe after my move
				return true;
	}

	return false;
}

bool Player::castling_simulation(const square& dest) {
	Piece* ROOKS[2] = { nullptr };

	list<Piece*>::const_iterator it = my_set->get_listOFpicese().begin();
	for (int i = 0; i < 2 && it != my_set->get_listOFpicese().end(); it++)
		if ((*it)->who_am_i() == ROOK)
			if ((*it)->get_status() && !(*it)->get_piece_moved()) {
				ROOKS[i] = (*it);
				i++;
			}

	if (ROOKS[0] == nullptr)
		return false;

	// in this point there is at least 1 ROOK that alive and doesn't move

	square unit_vector(0, 0);
	clc_unit_vec(my_king->get_loc(), dest, unit_vector);

	square tmp_king_loc(my_king->get_loc());

	for (int i = 0; ROOKS[i] && i < 2; i++) {
		if (ROOKS[i]->get_loc() == (dest + unit_vector)) // rook_loc = dest + 1 col in the same direction of unit vector
			if (check_free_way(tmp_king_loc, dest)) {
				while (tmp_king_loc != dest) {// by chess rules - the king need to be safe all the way to dest squre
					if (!king_safe_simulation(my_king, my_king->get_loc(), tmp_king_loc + unit_vector))
						return false;
					tmp_king_loc += unit_vector;
				}
				//at this point tmp_king_loc == dest
				my_king->set_partner_rook(ROOKS[i]);
				dynamic_cast<Rook*>(ROOKS[i])->set_castling_loc(tmp_king_loc -= unit_vector);
				return true;
			}
	}
	return false;
}
// do virtual moving on the board and make shur that my_king is safe
bool Player:: king_safe_simulation(Piece* sp, square cur_sqr, const square& dest_sqr) {
	//movment of our piece can lead to passive chess(movment of the king or a regular peice)
	//Assuming the simulation happens fast enough that the player does not notice, we will do the simulation  original board
	Piece* dest_tmp = map_board->at(dest_sqr);

	move_pis_on_board(sp, cur_sqr, dest_sqr); // do virtual moving on the board
	Piece_set* rivel_set = gameboard->get_set(!m_color);
	list<Piece*>::const_iterator it = rivel_set->get_listOFpicese().begin();
	for (; it != rivel_set->get_listOFpicese().end(); it++) {
		if ((*it)->get_status())
			if ((*it)->valid_move(my_king->get_loc(), map_board))
				if (check_free_way((*it)->get_loc(), my_king->get_loc()) || (*it)->who_am_i() == KNIGHT) {
					// come back to riginal parameters before moving on the board
					revers_move(dest_sqr, cur_sqr, sp, dest_tmp);
					return false; //the king cant move to this destination
				}
	}
	// come back to riginal parameters before moving on the board
	revers_move(dest_sqr, cur_sqr, sp, dest_tmp);
	return true;
}
//put the rivel king in check
void Player:: check_rival_king(Piece* sp, const square cur_sqr, const square dest_sqr) {
	//Active Check
	if (sp->valid_move(rival_king->get_loc(), map_board))
		if (check_free_way(sp->get_loc(), rival_king->get_loc()) || sp->who_am_i() == KNIGHT) {
			rival_king->set_check(YES);// put the rival king in check
			rival_king->get_attack_list().emplace_back(sp); // enter the threats to a list
		}


	//Pasive Check
	square unit_vector(0, 0);
	clc_unit_vec(rival_king->get_loc(), cur_sqr, unit_vector);
	if (unit_vector == square(0, 0)) { //unit_vector can be only row or col or diagonal 
		return;
	}
	square temp_cur_sqr = cur_sqr;
	Piece* temp_piece = nullptr;

	while (true) { //shifting by unit_vector
		try { temp_piece = map_board->at(temp_cur_sqr += unit_vector); }
		catch (out_of_range) { return; }// cur_sqr is out of map boarders
		if (temp_piece == nullptr)
			continue;
		if (temp_piece->get_color() != m_color) { //ememy blocking the way
			return;
		}
		if (temp_piece->valid_move(rival_king->get_loc(), map_board))
			if (check_free_way(temp_piece->get_loc(), rival_king->get_loc())) {
				rival_king->set_check(YES);// put the rivel king in check
				rival_king->get_attack_list().emplace_back(sp); // enter the threats to a list
				return;
			}
	}
}
bool  Player:: move_pis_on_board(Piece* sp, const square cur_sqr, const square dest_sqr) {
	bool attack_log=false;
	if (map_board->at(dest_sqr) != nullptr)
		if (map_board->at(dest_sqr)->get_color() != m_color) { // the enemy is there
			map_board->at(dest_sqr)->set_status(NO); //kill the enemy
			map_board->at(dest_sqr)->set_loc(square());
			attack_log = true;
		}
	map_board->at(sp->get_loc()) = nullptr; // leave cur loc
	sp->set_loc(dest_sqr); 
	map_board->at(sp->get_loc()) = sp; 
	return attack_log;
}
// here the cur_sqr was the dest_sqr befor, and perv_sqr was the cur_sqr befor
void  Player:: revers_move(const square cur_sqr, const square perv_sqr, Piece* moving_sp, Piece* dead_sp) {
	if (dead_sp != nullptr) {
		dead_sp->set_status(YES);//cur_sqr
		dead_sp->set_loc(cur_sqr);
	}
	moving_sp->set_loc(perv_sqr);
	map_board->at(perv_sqr) = moving_sp;
	map_board->at(cur_sqr) = dead_sp;
}
bool Player:: check_free_way(const square cur_sqr, const square dest_sqr) {
	square unit_vector(0, 0);
	square tmp = cur_sqr;
	clc_unit_vec(cur_sqr, dest_sqr, unit_vector);// unit_vector of the vector = (dest_sqr-cur_sqr) without absolute value
	for (tmp += unit_vector; tmp != dest_sqr; tmp += unit_vector) {
		try {
			if (map_board->at(tmp)) // //check if this square is free. if true there is piece there, you are block!
				return false;
		}
		catch (out_of_range) { return false; }
	}
	//all the way is free to the dest in this point
	return true;
}
bool Player:: select_valid_destination(square& dest_sqr, string& go_to_menu) {
	go_to_menu = input(dest_sqr);
	if (go_to_menu != VALID_SQUARE_INPUT)
		return false;
	//input_square(dest_sqr, "destination location");
	try {
		if (map_board->at(dest_sqr) == nullptr) // empty 
			return true;
		else if (map_board->at(dest_sqr)->get_color() != m_color)
			return true; // rival is there
		else
			cout << "you already have piece there" << endl;
		return false;
	}
	catch (out_of_range) { return false; }
}
Piece* Player::select_piece(square& cur_sqr, string& go_to_menu) {
	do {
		go_to_menu = input(cur_sqr);
		if (go_to_menu != VALID_SQUARE_INPUT)
			return nullptr;
		//input_square(cur_sqr, "piece location");
		if (map_board->at(cur_sqr) != nullptr)
			if (map_board->at(cur_sqr)->get_color() == m_color)
				return map_board->at(cur_sqr);
			else
				cout << "This is not your piece" << endl;
		else
			cout << "In this point there are no pieces" << endl;
	} while (1);
}
// get coor for player's attack
string Player::input(square& sqr) {
	string user_chocie;
	int size = 0;
	do {
		cin >> user_chocie;
		size = user_chocie.size();
		if (size > 2) {
			if (user_chocie == MENU || user_chocie == TIE || user_chocie == SURRNDOR)// check for action word input
				return user_chocie;// menu - word pressed
		}
		else if (size == 2) {// assume that the input is square.
			if (castNvalid_square_input(user_chocie[0], user_chocie[1], sqr))
				return VALID_SQUARE_INPUT;
		}
		cout << "invalid input, you must enter at least 2 characters" << endl;
	} while (true);
}

bool  Player:: castNvalid_square_input(char row, char col, square& tmp_sqr) {
	if (row >= MIN_ROW && row <= MAX_ROW) {
		// col can be  1,2,...,9  or 10
		if (col >= MIN_COL1 && col <= MAX_COL1 || col >= MIN_COL2 && col <= MAX_COL2) {
			if (col >= MIN_COL1 && col <= MAX_COL1)
				col -= SMALL_A_TO_INT; //(0-7)
			else if (col >= MIN_COL2 && col <= MAX_COL2)
				col -= BIG_A_TO_INT; //(0-7)
			row -= INT2CHAR + NUM2INDEX; //(0-7)
			tmp_sqr.set_square(row, col);
			return true;
		}
	}
	return false;
}

bool Player::get_color() const { return m_color; }
