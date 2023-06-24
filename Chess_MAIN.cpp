
/*~~~~~~~~~~~~~~~~~~~ check memory leaks  ~~~~~~~~~~~~~~~~~~~*/
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#include <cstdlib>
#include <crtdbg.h>
#else
#define DBG_NEW new
#endif


/*~~~~~~~~~~~~~~~~~~~ main  ~~~~~~~~~~~~~~~~~~~*/

#include "Game_engine.h"

void start_chess_game() {
	srand(time(NULL));
	Game_engine chess;
	chess.play_general();
}

int main() {
	start_chess_game();

	cout << "Leaks: " << _CrtDumpMemoryLeaks() << endl; //chek memory leaks
}