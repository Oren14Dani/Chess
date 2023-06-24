# Chess

# Introduction - a brief description of the program
The program allows you to play a chess game between 2 real players.
Interfacing is done through the Windows console screen.
The game has a user database stored in a .txt file, and players are identified by entering the player's name.
And in order to play it is necessary to have 2 players log in.
The game has a main screen from which you can switch between different screens.


# Description of the main actions that can be performed in the program
Player login.
Add a new player, and remove registered players - the user will be identified by name.
View user statistics by ranking all users by overall score.
Save a game in the middle.
Loading a discontinued game or old games that haven't finished yet.
Viewing moves made in a particular game.
Graphical display of a chessboard.


# Brief explanation of the departments
The Game engine class provides the general envelope for the game, manages the game, allows loading and saving, and viewing statistics.
The Player class is responsible for the rules of the game and allows players to perform actions on the game board.
The Piece class splits into possible tool types.
The board class contains a map that simulates the game board and creates connectivity between square and game tools.
The player department works closely with the board, piece, and square departments for the benefit of the game itself in managing moves.
The Game_engine department contains two other departments that help it manage the game. The Main_data class favors sittest memory in the form of files and available memory via a quick retrieval map.
In addition, the department also contains a save_load_game department that enables the ability to save and load, serialization
Inheritance and polymorphism - A PIECE class is an abstract class that bequeaths its attributes to all types of tools in a game. 
(The tool classes are: King, Queen, Rook, etc.)

