#ifndef GENERAL_H
#define GENERAL_H

/*~~~~~~~~~~~~~~~~~~~ general define & include  ~~~~~~~~~~~~~~~~~~~*/

#include <iostream>
using namespace std;
#include <string>
#include <time.h>
#include <fstream>
#include <map>
#include <math.h>
#include <list>
#include <dos.h>
#include <Windows.h>
#include "utils.h"

#define YES true
#define NO false
#define NOT_MATCH -1
#define PLAYCHESS "Play-Chess"	// title
#define MENU "MENU" // go to the menu
#define VALID_SQUARE_INPUT "valid square input" 
#define NUM_OF_MOVES_TO_PRINT 6
#define IS_EMPTY "is empty"
#define PAUSE "pause"
#define INVALID_INPUT "invalid input"
// ~~~~~~~~~~ pieces names ~~~~~~~~~ //
#define KING "K"
#define QUEEN "Q"
#define KNIGHT "N"
#define BISHOP "B"
#define ROOK "R"
#define PAWN "P"

// ~~~~~~~~~~ players ~~~~~~~~~ //
#define WHITE "White"
#define BLACK "Black"

// ~~~~~~~~~~ board borders ~~~~~~~~~ //
#define NUM_OF_ROWS 8
#define NUM_OF_COLS 8
#define FIRST_ROW 1
#define LAST_ROW 8
#define  MIN_ROW '1'
#define  MAX_ROW '8'
#define  MIN_COL1 'a'
#define  MIN_COL2 'A'
#define  MAX_COL1 'h'
#define  MAX_COL2 'H'

// ~~~~~~~~~~ color code ~~~~~~~~~ //
#define BLACK_DEC 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHTGRAY 7
#define DARKGRAY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTMAGENTA 13
#define YELLOW 14
#define WHITE_DEC 15

// ~~~~~~~~~~ casting ~~~~~~~~~ //
#define  BIG_A_TO_INT 'A'
#define  SMALL_A_TO_INT 'a'
#define INT2CHAR '0'
#define NUM2INDEX 1

// ~~~~~~~~~~ statistics ~~~~~~~~~ //
#define SCORE_PER_RANK 150
#define TOP_LEVEL_INDEX 5
#define MIN_LEVEL_INDEX 0
#define ROOKIE_RANK "rookie"
#define BEGINER_RANK "beginer"
#define AVERGE_RANK "averge" 
#define ADVANCE_RANK "advance" 
#define EXPERT_RANK "expert"
#define GOD_RANK "god"
// ~~~~~~~~~~  players data base points ~~~~~~~~~ //
#define WIN_SCORE 30
#define LOSE_SCORE -10
#define TIE_SCORE 10
#define SURRNDOR_SCORE -5
#define CHECH_SCORE 2
#define ATTACK_SCORE 2
// ~~~~~~~~~~  players data base keys ~~~~~~~~~ //
#define KEYS 8
#define POINTS "POINTS"
#define GAME_PLAYED "G_PLAY" 
#define WIN "WIN"
#define LOSE "LOSE"
#define TIE "TIE" // players agreed to end the game 
#define SURRNDOR "SURRND"// player give up (surrender)
#define CHECK "CHECK"
#define ATTACKS "ATTACK"
#define ENDGAME "end game"

// ~~~~~~~~~~ MENU ~~~~~~~~~ //
#define START_NEW_GAME 1;
#define	LOAD_GMAE 2;
#define PLAYER_STATISTICS 3;
#define HELL_OF_FAME 4;
#define BACK_TO_GAME 5 ;
#define EXIT_TO_MENU 6;

// ~~~~~~~~~~ valid_charcters check ~~~~~~~~~ //
#define valid_charcters_for_name "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_"
#define VALID_NUMBERS "0123456789"

// ~~~~~~~~~~ game_data ~~~~~~~~~ //
#define NONE "None";
#define PIECE_RESTOR_DATA  3;
#define MOVED "MOVED";
#define NOT_MOVED "NOT_MOVED";

/* ~~~~~~~~~~ FILES ~~~~~~~~~ */
#define TXT ".txt"
#define CHESS_GAME_USERS_TXT   "Chess_game_users.txt"
#define CHESS_GAME_COUNTER_TXT "Chess_game_counter.txt"
#define CHESS_RULES_TXT "chess_rules.txt"
#define GAME_LOG_FILE_FORM "game_log_" // start for the complte file name is. for exemple: "game_log_324.txt"
#define CHESS_SAVEGAME_FILE_FORM  "Chess_savedgame_"


#endif