/*
Written by Ivo Mihov
ID 9918179
The University of Manchester
30 April 2020

This code was written and tested using Visual Studio 2019 on Windows 10
*/

// This header contains the game class and its methods. It is the abstract base class
// for both backgammon and tapa board games. Includes functions for rolling dice, game input etc. 
// that are universal for all tables games.

#pragma once
#include "board.h"

class game
{
protected:
	std::vector<std::vector<int>> _moves;
	unsigned* _hit_pieces{ new unsigned[2] };
	unsigned* _extracted_pieces{ new unsigned[2] };
	board* _backgammon_board{ nullptr };
	player* _player1{ nullptr };
	player* _player2{ nullptr };
public:
	game();
	game(const game& game_to_copy);
	game(game&& game_to_move) noexcept;
	game& operator=(const game& game_to_copy);
	game& operator=(game&& game_to_move) noexcept;
	bool is_int(const std::string& str) const;
	int* throw_dice() const;
	void save_move(const int start, const int destination, const bool hit_checker);
	virtual void print_rules() = 0;
	virtual void begin() = 0;
	virtual void play_turn() = 0;
	virtual int* enter_move() const;
	virtual ~game();
};
game::game()
{
	_hit_pieces[0] = 0;
	_hit_pieces[1] = 0;
	_extracted_pieces[0] = 0;
	_extracted_pieces[1] = 0;
}

game::game(const game& game_to_copy)
{	
	if (this != &game_to_copy) {
		if (game_to_copy._moves.size() > 0) {
			for (unsigned iter_vector_row{ 0 }; iter_vector_row < game_to_copy._moves.size(); iter_vector_row++) {
				for (unsigned iter_vector_col{ 0 }; iter_vector_col < game_to_copy._moves[iter_vector_row].size(); iter_vector_col++) {
					this->_moves[iter_vector_row][iter_vector_col] = game_to_copy._moves[iter_vector_row][iter_vector_col];
				}
			}
		}
		this->_hit_pieces[0] = game_to_copy._hit_pieces[0];
		this->_hit_pieces[1] = game_to_copy._hit_pieces[1];
		this->_extracted_pieces[0] = game_to_copy._extracted_pieces[0];
		this->_extracted_pieces[1] = game_to_copy._extracted_pieces[1];

		this->_backgammon_board = new board(game_to_copy._backgammon_board);
	}
}
game::game(game&& game_to_move) noexcept
{
	if (this != &game_to_move) {
		this->_moves = game_to_move._moves;
		this->_hit_pieces = game_to_move._hit_pieces;
		this->_extracted_pieces = game_to_move._extracted_pieces;
		this->_backgammon_board = game_to_move._backgammon_board;
		// free space (after moving)
		std::vector<std::vector<int>>().swap(game_to_move._moves);
		game_to_move._hit_pieces = 0;
		game_to_move._extracted_pieces = 0;
		game_to_move._backgammon_board = nullptr;
	}
}
game& game::operator=(const game& game_to_copy)
{
	if (this != &game_to_copy) {
		if (game_to_copy._moves.size() > 0) {
			for (unsigned iter_vector_row{ 0 }; iter_vector_row < game_to_copy._moves.size(); iter_vector_row++) {
				for (unsigned iter_vector_col{ 0 }; iter_vector_col < game_to_copy._moves[iter_vector_row].size(); iter_vector_col++) {
					this->_moves[iter_vector_row][iter_vector_col] = game_to_copy._moves[iter_vector_row][iter_vector_col];
				}
			}
		}
		this->_hit_pieces[0] = game_to_copy._hit_pieces[0];
		this->_hit_pieces[1] = game_to_copy._hit_pieces[1];
		this->_extracted_pieces[0] = game_to_copy._extracted_pieces[0];
		this->_extracted_pieces[1] = game_to_copy._extracted_pieces[1];
		this->_backgammon_board = new board(game_to_copy._backgammon_board);
	}
	return *this;
}
game& game::operator=(game&& game_to_move) noexcept
{
	if (this != &game_to_move) {
		this->_moves = game_to_move._moves;
		this->_hit_pieces = game_to_move._hit_pieces;
		this->_extracted_pieces = game_to_move._extracted_pieces;
		this->_backgammon_board = game_to_move._backgammon_board;
		// free space (after moving)
		std::vector<std::vector<int>>().swap(game_to_move._moves);
		game_to_move._hit_pieces = 0;
		game_to_move._extracted_pieces = 0;
		game_to_move._backgammon_board = nullptr;
	}
	return *this;
}
bool game::is_int(const std::string& str) const
{
	char* end{ nullptr };
	// set end to point to the first character after integer
	unsigned long val = strtoul(str.c_str(), &end, 10);
	// end should be \0 or space if str contains only an int
	return end != str.c_str() && ((end[0] == '\0') || (std::isspace(end[0]) && end[1] == '\0')) && val != HUGE_VAL;
}

int* game::throw_dice() const
{
	int* dice_roll{new int[2]};
	// set random seed to time (in sec) from 1 Jan 1970
	srand((unsigned)time(NULL));
	// simulate 2 dice rolling
	dice_roll[0] = rand() % 6 + 1;
	dice_roll[1] = rand() % 6 + 1;
	return dice_roll;
}

int* game::enter_move() const
{
	std::string whole_input{ "" };
	std::string location{ "" };
	std::string destination{ "" };
	std::stringstream whole_istream{ whole_input };
	int* move_details{ new int[2] };
	int loc{ 0 };
	int dest{ 0 };
	int loc_extraction{ 0 };
	getline(std::cin >> std::ws, whole_input);
	if (whole_input == "help") {
		return new int{ 0 };
	}
	// check if one integer is entered
	if (is_int(whole_input)) {
		loc_extraction = std::stoi(whole_input);
		if (loc_extraction > 0 && loc_extraction < 25) {
			move_details[0] = loc_extraction;
			// set second element to 100 to signal one-number input
			move_details[1] = 100;
			return move_details;
		}
	} else {
		// otherwise extract the two integers separated by a space
		whole_istream << whole_input;
		getline(whole_istream, location, ' ');
		if (is_int(location)) {
			loc = std::stoi(location);
			if (loc > 0 && loc < 25) {
				move_details[0] = loc;
			} else {
				std::cerr << "	Expected a location between 1 and 24!\n";
				return new int{ 100 };
			}
		} else {
			std::cerr << "	Expected an integer for the location of the checker that you would like to move!\n";
			// return 100 in case of invalid input
			return new int{ 100 };
		}
		getline(whole_istream >> std::ws, destination);
		if (is_int(destination)) {
			dest = std::stoi(destination);
			if (dest > 0 && dest < 25) {
				move_details[1] = dest;
			} else {
				std::cerr << "	Expected a destination between 1 and 24!\n";
				return new int{ 100 };
			}
		} else {
			std::cout << destination;
			std::cerr << "	Expected an integer for the destination to which you would like to move the checker!\n";
			return new int{ 100 };
		}
	}
	return move_details;
}

void game::save_move(const int start, const int destination, const bool hit_checker)
{
	// push back move details in a vector
	std::vector<int> temp_move;
	temp_move.push_back(start);
	temp_move.push_back(destination);
	temp_move.push_back((int)hit_checker);
	_moves.push_back(temp_move);
}

game::~game() 
{ 
	delete[] _hit_pieces;
	delete[] _extracted_pieces;
	delete _backgammon_board; 
	delete _player1; 
	delete _player2; 
	//std::cout << "Destroyed game!\n"; 
}