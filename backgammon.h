/*
Written by Ivo Mihov
ID 9918179
The University of Manchester
30 April 2020

This code was written and tested using Visual Studio 2019 on Windows 10
*/

// The backgammon class provides the necessary functionality to play backgammon
// Inherits the game class and uses its members (with exception of _playing_tapa and 
// _stuck array that are used in tapa)

#pragma once
#include "game.h"

class backgammon : public game
{
public:
	// player with name1 starts first
	backgammon(std::string name1 = "Player 1", std::string name2 = "Player 2");
	backgammon(player* player1, player* player2);
	backgammon(backgammon& game_to_copy);
	backgammon(backgammon&& game_to_move) noexcept;
	backgammon& operator=(backgammon& game_to_copy);
	backgammon& operator=(backgammon&& game_to_move) noexcept;
	void print_rules();
	// functions that provide the functionality
	void begin();
	void play_turn();
	~backgammon() {}
};
backgammon::backgammon(std::string name1, std::string name2)
{
	_backgammon_board = new board(false);
	// order board for backgammon
	_backgammon_board->order_standard();
	// construct player objects with respective turns for the players 
	_player1 = new player(name1, 1);
	_player2 = new player(name2, 2);
}
backgammon::backgammon(player* player1, player* player2)
{
	_backgammon_board = new board(false);
	// order for backgammon
	_backgammon_board->order_standard();
	// check which player has the first turn
	if (player1->get_turn() == 1 && player2->get_turn() == 2) {
		_player1 = player1;
		_player2 = player2;
	} else if (player1->get_turn() == 2 && player2->get_turn() == 1) {
		_player2 = player1;
		_player1 = player2;
	} else {
		std::cerr << "Players have wrong turns: one has to have turn equal to 1, and the second to 2. \n";
		exit(1);
	}
}
backgammon::backgammon(backgammon& game_to_copy)
{
	// copy each element of vector
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
backgammon::backgammon(backgammon&& game_to_move) noexcept
{
	if (this != &game_to_move) {
		this->_moves = game_to_move._moves;
		this->_hit_pieces = game_to_move._hit_pieces;
		this->_extracted_pieces = game_to_move._extracted_pieces;
		this->_backgammon_board = game_to_move._backgammon_board;
		// free space of vector (after moving it)
		std::vector<std::vector<int>>().swap(game_to_move._moves);
		game_to_move._hit_pieces = 0;
		game_to_move._extracted_pieces = 0;
		game_to_move._backgammon_board = nullptr;
	}
}
backgammon& backgammon::operator=(backgammon& game_to_copy)
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
backgammon& backgammon::operator=(backgammon&& game_to_move) noexcept
{
	if (this != &game_to_move) {
		this->_moves = game_to_move._moves;
		this->_hit_pieces = game_to_move._hit_pieces;
		this->_extracted_pieces = game_to_move._extracted_pieces;
		this->_backgammon_board = game_to_move._backgammon_board;
		// free space of vector (after moving it)
		std::vector<std::vector<int>>().swap(game_to_move._moves);
		game_to_move._hit_pieces = 0;
		game_to_move._extracted_pieces = 0;
		game_to_move._backgammon_board = nullptr;
	}
	return *this;
}
void backgammon::print_rules()
{
	std::cout << "\n	RULES\n\n";
	std::cout << "	Each player starts with 15 checkers. The aim is to move all checkers to \n";
	std::cout << "	the own field of the player and bear them off the board. \n";
	std::cout << "	Checkers can only land on free spots or ones where the opponent has one checker only.\n";
	std::cout << "	If a checker lands on a lone checker, the lone checker is hit and taken out of the board.\n";
	std::cout << "	The player then has to place it back on the board, starting from the other player's field\n";
	std::cout << "	(19-24 point). Once all the checkers are in the player's own field, the bearing off process\n";
	std::cout << "	begins and the player can bear off a checker on a position that matches the rolled dice \n";
	std::cout << "	(e.g. if you roll 5 you can bear off checker at 5th position) or move. The player who first bears\n";
	std::cout << "	off all their checkers wins.\n\n";
	std::cout << "	Have fun playing!\n\n";
}
void backgammon::begin()
{
	// play next turn until one player collects all their pieces
	while (_extracted_pieces[1] < 15 && _extracted_pieces[0] < 15) {
		play_turn();
	}
	// if the other one collects nothing, award 2 points
	if (_extracted_pieces[1] == 15) {
		if (_extracted_pieces[0] == 0) {
			_player2->won_game(2);
			_player1->lost_game(2);
		} else {
			_player2->won_game();
			_player1->lost_game();
		}
	} else if (_extracted_pieces[0] == 15) {
		if (_extracted_pieces[0] == 0) {
			_player1->won_game(2);
			_player2->lost_game(2);
		} else {
			_player1->won_game();
			_player2->lost_game();
		}
	}
	std::cout << "\nThank you for playing!!!\n";
}

void backgammon::play_turn()
{
	int* dice_throw{ nullptr };
	int* move{ nullptr };
	unsigned num_moves{ 2 };
	// helper variables during iterations
	int dice_played{ 0 };
	int multiplier{ 0 };
	int largest_possible_extraction{ 12 };
	bool hit_checker{ false };
	for (int iter_player{ 1 }; iter_player < 3; iter_player++) {
		// roll dice
		dice_throw = throw_dice();
		dice_played = 0;
		// multiplier to find the direction of the move (positive or negative)
		multiplier = -(iter_player * 2 - 3); // 1 for player 1 and -1 for player 2
		// play 4 moves if player rolls a pair
		if (dice_throw[0] == dice_throw[1]) {
			num_moves = 4;
		} else {
			num_moves = 2;
		}
		for (unsigned iter_moves{ 0 }; iter_moves < num_moves; iter_moves++) {
			hit_checker = false;
			// check if a checker is hit
			if (_hit_pieces[iter_player - 1] == 0) {
				// check if the player is eligible to start the collection process
				if (_backgammon_board->is_all_in_own(iter_player)) {
					std::cout << "\n\n\n\n";
					_backgammon_board->print_board();
					if (this->_moves.size() == 0) {
						std::cout << "\n	Please type 'help' for the rules of the game.\n";
					}
					// output name of player, dice rolled, and number of their hit and extracted pieces
					if (iter_player == 1) {
						std::cout << "\n" << std::setw(21) << _player1->get_name() << std::setw(8) << "'s move:" << std::setw(34) << "HIT PIECES: " << _hit_pieces[iter_player - 1] << "\n";
					} else if (iter_player == 2) {
						std::cout << "\n" << std::setw(21) << _player2->get_name() << std::setw(8) << "'s move:" << std::setw(34) << "HIT PIECES: " << _hit_pieces[iter_player - 1] << "\n";
					}
					std::cout << std::setw(21) << "You threw " << std::setw(1) << dice_throw[0] << std::setw(5) << " and " << std::setw(1) << dice_throw[1] << ".";
					std::cout << std::setw(40) << "EXTRACTED PIECES: " << _extracted_pieces[iter_player - 1] << std::endl;
					std::cout << '\n';

					_backgammon_board->list_possible_moves(iter_player, dice_throw[0], dice_throw[1]);
					// find where is the furthest piece of the player 
					largest_possible_extraction = abs(25 * (iter_player - 1) - _backgammon_board->furthest_piece_location(iter_player));
					// check if the numbers on the dice are bigger (bear pieces off automatically)
					if ((largest_possible_extraction <= dice_throw[0] && largest_possible_extraction <= dice_throw[1]) || 
						(iter_moves == 1 && ((largest_possible_extraction <= dice_throw[0] && dice_played != dice_throw[0]) || 
						(largest_possible_extraction <= dice_throw[1] && dice_played != dice_throw[1])))) {
						_backgammon_board->extract_checker(_backgammon_board->furthest_piece_location(iter_player) - 1, iter_player);
						_extracted_pieces[iter_player - 1] ++;
						continue;
					}
					
					std::cout << "\n	Please enter your move in the form ";
					std::cout << "\n	'position of checker' 'desired destination' (e.g. '13 7')";
					std::cout << "\n	or simply type position of checker you want to bear off: ";
					move = enter_move();
					// set checkpoint for any failed check of input to return to	
				extraction_checkpoint:
					if (*move == 0) {
						print_rules();
						std::cout << "\n	Please enter your move in the form ";
						std::cout << "\n	'position of checker' 'desired destination' (e.g. '13 7')";
						std::cout << "\n	or simply type position of checker you want to bear off: ";
						move = enter_move();
						goto extraction_checkpoint;
					}
					// enter_move returns 100 for invalid input
					if (*move == 100) {
						std::cout << "	Invalid input! \n";
						std::cout << "\n	Please enter your move in the form ";
						std::cout << "\n	'position of checker' 'desired destination' (e.g. '13 7')";
						std::cout << "\n	or simply type position of checker you want to bear off: ";
						move = enter_move();
						goto extraction_checkpoint;
					}
					// check if dice is taken into account
					if (25 * (iter_player - 1) + multiplier * move[0] != dice_throw[0] &&
						25 * (iter_player - 1) + multiplier * move[0] != dice_throw[1] &&
						move[1] == 100) {
						std::cout << "	Invalid input! \n";
						std::cout << "	Keep in mind the dice you threw! \n";
						std::cout << "\n	Please enter your move in the form ";
						std::cout << "\n	'position of checker' 'desired destination' (e.g. '13 7')";
						std::cout << "\n	or simply type position of checker you want to bear off: ";
						move = enter_move();
						goto extraction_checkpoint;
					}
					// move[1] is set to 100 for single number input
					if (!_backgammon_board->is_extraction_possible(iter_player, move[0] - 1) && move[1] == 100) {
						std::cout << "	Invalid input! \n";
						std::cout << "	No checker present at this point! \n";
						std::cout << "\n	Please enter your move in the form ";
						std::cout << "\n	'position of checker' 'desired destination' (e.g. '13 7')";
						std::cout << "\n	or simply type position of checker you want to bear off: ";
						move = enter_move();
						goto extraction_checkpoint;
					}
					// check if dice is played in previous move
					if (multiplier * (move[0] - move[1]) == dice_played &&
						dice_throw[0] != dice_throw[1] && move[1] != 100) {
						std::cout << "	Invalid input! \n";
						std::cout << "	This die was played already! \n";
						std::cout << "\n	Please enter your move in the form ";
						std::cout << "\n	'position of checker' 'desired destination' (e.g. '13 7')";
						std::cout << "\n	or simply type position of checker you want to bear off: ";
						move = enter_move();
						goto extraction_checkpoint;
					}
					// check if dice is taken into account
					if (multiplier * (move[0] - move[1]) != dice_throw[0] &&
						multiplier * (move[0] - move[1]) != dice_throw[1] &&
						multiplier * (move[0] - move[1]) != dice_throw[0] + dice_throw[1] &&
						move[1] != 100) {
						std::cout << "	Invalid input! \n";
						std::cout << "	Keep in mind the dice you threw and the direction in which you should move! \n";
						std::cout << "\n	Please enter your move in the form ";
						std::cout << "\n	'position of checker' 'desired destination' (e.g. '13 7')";
						std::cout << "\n	or simply type position of checker you want to bear off: ";
						move = enter_move();
						goto extraction_checkpoint;
					}
					if (move[1] != 100 &&
						!(_backgammon_board->is_move_possible(move[0] - 1, move[1] - 1))) {
						std::cout << "	Invalid input! \n";
						std::cout << "	Move is not possible. Destination is not empty.\n";
						std::cout << "\n	Please enter your move in the form ";
						std::cout << "\n	'position of checker' 'desired destination' (e.g. '13 7')";
						std::cout << "\n	or simply type position of checker you want to bear off: ";
						move = enter_move();
						goto extraction_checkpoint;
					}
					// if move is over sum of dice, check if intermediate positions are free
					if (move[1] != 100 && multiplier * (move[0] - move[1]) == dice_throw[0] + dice_throw[1] &&
						!(_backgammon_board->is_move_possible(move[0] - 1, move[0] - 1 - multiplier * dice_throw[0])) &&
						!(_backgammon_board->is_move_possible(move[0] - 1, move[0] - 1 - multiplier * dice_throw[1]))) {
						std::cout << "	Invalid input! \n";
						std::cout << "	Cannot play both dice at once, since neither of the intermediate points is free.\n";
						std::cout << "\n	Please enter your move in the form ";
						std::cout << "\n	'position of checker' 'desired destination' (e.g. '13 7')";
						std::cout << "\n	or simply type position of checker you want to bear off: ";
						move = enter_move();
						goto extraction_checkpoint;
					}
					// move/extract checker
					if (multiplier * (move[0] - move[1]) == dice_throw[0] + dice_throw[1]) {
						if (_backgammon_board->is_move_possible(move[0] - 1, move[0] - 1 - multiplier * dice_throw[0])) {
							hit_checker = _backgammon_board->move_checker(move[0] - 1, move[0] - 1 - multiplier * dice_throw[0]);
							_hit_pieces[2 - iter_player] += (int)hit_checker;
							save_move(move[0], move[0] - multiplier * dice_throw[0], hit_checker);
							hit_checker = _backgammon_board->move_checker(move[0] - 1 - multiplier * dice_throw[0], move[1] - 1);
							_hit_pieces[2 - iter_player] += (int)hit_checker;
							save_move(move[0] - multiplier * dice_throw[0],  move[1], hit_checker);
						} else if (_backgammon_board->is_move_possible(move[0] - 1, move[0] - 1 - multiplier * dice_throw[1])) {
							hit_checker = _backgammon_board->move_checker(move[0] - 1, move[0] - 1 - multiplier * dice_throw[1]);
							_hit_pieces[2 - iter_player] += (int)hit_checker;
							save_move(move[0], move[0] - multiplier * dice_throw[1], hit_checker);
							hit_checker = _backgammon_board->move_checker(move[0] - 1 - multiplier * dice_throw[1], move[1] - 1);
							_hit_pieces[2 - iter_player] += (int)hit_checker;
							save_move(move[0] - multiplier * dice_throw[1], move[1], hit_checker);
						}
					} else if (move[1] == 100) {
						if (_backgammon_board->is_extraction_possible(iter_player, move[0] - 1)) {
							_backgammon_board->extract_checker(move[0] - 1, iter_player);
							_extracted_pieces[iter_player - 1] ++;
							save_move(move[0], move[1] + 1, hit_checker);
						} else {
							std::cerr << "Bug in system, impossible to extract this checker!\n";
							exit(10);
						}
						dice_played = 25 * (iter_player - 1) + multiplier * move[0];
					} else if (move[1] < 25 && move[1] > 0) {
						hit_checker = _backgammon_board->move_checker(move[0] - 1, move[1] - 1);
						_hit_pieces[2 - iter_player] += (int)hit_checker;
						save_move(move[0], move[1], hit_checker);
						dice_played = abs(move[0] - move[1]);
					}
					// stop/reduce iterations if two dice have been played at once
					if (dice_played == dice_throw[0] + dice_throw[1] && dice_throw[0] != dice_throw[1]) {
						break;
					} else if (dice_throw[0] == dice_throw[1] && dice_played == 2 * dice_throw[0]) {
						iter_moves++;
					}
				} else {
					std::cout << "\n\n\n\n";
					_backgammon_board->print_board();
					if (this->_moves.size() == 0) {
						std::cout << "\n	Please type 'help' for the rules of the game.\n";
					}
					// print turn details
					if (iter_player == 1) {
						std::cout << "\n" << std::setw(21) << _player1->get_name() << std::setw(8) << "'s move:" << std::setw(34) << "HIT PIECES: " << _hit_pieces[iter_player - 1] << "\n";
					} else if (iter_player == 2) {
						std::cout << "\n" << std::setw(21) << _player2->get_name() << std::setw(8) << "'s move:" << std::setw(34) << "HIT PIECES: " << _hit_pieces[iter_player - 1] << "\n";
					}
					std::cout << std::setw(21) << "You threw " << std::setw(1) << dice_throw[0] << std::setw(5) << " and " << std::setw(1) << dice_throw[1] << ".";
					std::cout << std::setw(40) << "EXTRACTED PIECES: " << _extracted_pieces[iter_player - 1] << std::endl;
					std::cout << '\n';
					// check if any moves are possible 
					if (dice_played == 0) {
						if (!_backgammon_board->list_possible_moves(iter_player, dice_throw[0], dice_throw[1])) {
							std::cout << "No possible moves!! Skipping turn!\n";
							continue;
						}
					} else if (dice_played == dice_throw[0]) {
						if (!_backgammon_board->list_possible_moves(iter_player, dice_throw[1])) {
							std::cout << "No possible moves!! Skipping turn!\n";
							continue;
						}
					} else if (dice_played == dice_throw[1]) {
						if (!_backgammon_board->list_possible_moves(iter_player, dice_throw[0])) {
							std::cout << "No possible moves!! Skipping turn!\n";
							continue;
						}
					} else {
						if (!_backgammon_board->list_possible_moves(iter_player, dice_throw[0], dice_throw[1])) {
							std::cout << "No possible moves!! Skipping turn!\n";
							continue;
						}
					}
					std::cout << "\n	Please enter your move in the form \n	'position of checker' 'desired destination' (e.g. '13 7'): ";
					move = enter_move();
					// set checkpoint for input checks to return to
				move_checkpoint:
					if (*move == 0) {
						print_rules();
						std::cout << "\n	Please enter your move in the form \n	'position of checker' 'desired destination' (e.g. '13 7'): ";
						move = enter_move();
						goto move_checkpoint;
					}
					// check if enter_move returns 100 for invalid input
					if (*move == 100) {
						std::cout << "\n	Please enter your move again in the form \n	'position of checker' 'desired destination' (e.g. '13 7'): ";
						move = enter_move();
						goto move_checkpoint;
					}
					// check if dice are taken into account
					if ((multiplier * (move[0] - move[1]) != dice_throw[0]) &&
						(multiplier * (move[0] - move[1]) != dice_throw[1]) &&
						(multiplier * (move[0] - move[1]) != dice_throw[0] + dice_throw[1])) {
						std::cout << "	Invalid input! \n";
						std::cout << "	Keep in mind the dice you threw and the direction in which you should move! \n";
						std::cout << "\n	Please enter your move again in the form \n	'position of checker' 'desired destination' (e.g. '13 7'): ";
						move = enter_move();
						goto move_checkpoint;
					}
					// check if this die has been played
					if (multiplier * (move[0] - move[1]) == dice_played && dice_throw[0] != dice_throw[1]) {
						std::cout << "	Invalid input! \n";
						std::cout << "	You already played this die. Please play the other one. \n";
						std::cout << "\n	Please enter your move again in the form \n	'position of checker' 'desired destination' (e.g. '13 7'): ";
						move = enter_move();
						goto move_checkpoint;
					}
					if (!(_backgammon_board->is_move_possible(move[0] - 1, move[1] - 1))) {
						std::cout << "	Invalid input! \n";
						std::cout << "	Move is not possible. Destination is not empty.\n";
						std::cout << "\n	Please enter your move again in the form \n	'position of checker' 'desired destination' (e.g. '13 7'): ";
						move = enter_move();
						goto move_checkpoint;
					}
					// check if intermediate points are free if sum of dice is played
					if (multiplier * (move[0] - move[1]) == dice_throw[0] + dice_throw[1] &&
						!(_backgammon_board->is_move_possible(move[0] - 1, move[0] - 1 - multiplier * dice_throw[0])) &&
						!(_backgammon_board->is_move_possible(move[0] - 1, move[0] - 1 - multiplier * dice_throw[1])) &&
						dice_played == 0) {
						std::cout << "	Invalid input! \n";
						std::cout << "	Cannot play both dice at once, since neither of the intermediate points is free.\n";
						std::cout << "\n	Please enter your move again in the form \n	'position of checker' 'desired destination' (e.g. '13 7'): ";
						move = enter_move();
						goto move_checkpoint;
					}
					// move checker
					if (multiplier * (move[0] - move[1]) == dice_throw[0] + dice_throw[1]) {
						if (_backgammon_board->is_move_possible(move[0] - 1, move[0] - 1 - multiplier * dice_throw[0])) {
							hit_checker = _backgammon_board->move_checker(move[0] - 1, move[0] - 1 - multiplier * dice_throw[0]);
							_hit_pieces[2 - iter_player] += (int)hit_checker;
							save_move(move[0], move[0] - multiplier * dice_throw[0], hit_checker);
							hit_checker = _backgammon_board->move_checker(move[0] - 1 - multiplier * dice_throw[0], move[1] - 1);
							_hit_pieces[2 - iter_player] += (int)hit_checker;
							save_move(move[0] - multiplier * dice_throw[0], move[1], hit_checker);
						} else if (_backgammon_board->is_move_possible(move[0] - 1, move[0] - 1 - multiplier * dice_throw[1])) {
							hit_checker = _backgammon_board->move_checker(move[0] - 1, move[0] - 1 - multiplier * dice_throw[1]);
							_hit_pieces[2 - iter_player] += (int)hit_checker;
							save_move(move[0], move[0] - multiplier * dice_throw[1], hit_checker);
							hit_checker = _backgammon_board->move_checker(move[0] - 1 - multiplier * dice_throw[1], move[1] - 1);
							_hit_pieces[2 - iter_player] += (int)hit_checker;
							save_move(move[0] - multiplier * dice_throw[1], move[1], hit_checker);
						}
					} else {
						hit_checker = _backgammon_board->move_checker(move[0] - 1, move[1] - 1);
						_hit_pieces[2 - iter_player] += (int)hit_checker;
						save_move(move[0], move[1], hit_checker);
					}
					dice_played = abs(move[0] - move[1]);
					// break/reduce iterations if two dice played at once 
					if (dice_played == dice_throw[0] + dice_throw[1] && dice_throw[0] != dice_throw[1]) {
						break;
					} else if (dice_throw[0] == dice_throw[1] && dice_played == 2 * dice_throw[0]) {
						iter_moves++;
					}
				}
			// if pieces are hit, return back to board
			} else if (_hit_pieces[iter_player - 1] > 0) {
				std::cout << "\n\n\n\n";
				if (_backgammon_board->is_placement_possible(iter_player, dice_throw[0], dice_throw[1])) {
					_backgammon_board->print_board();
					if (this->_moves.size() == 0) {
						std::cout << "\n	Please type 'help' for the rules of the game.\n";
					}
					// print turn details
					if (iter_player == 1) {
						std::cout << "\n" << std::setw(21) << _player1->get_name() << std::setw(8) << "'s move:" << std::setw(34) << "HIT PIECES: " << _hit_pieces[iter_player - 1] << "\n";
					} else if (iter_player == 2) {
						std::cout << "\n" << std::setw(21) << _player2->get_name() << std::setw(8) << "'s move:" << std::setw(34) << "HIT PIECES: " << _hit_pieces[iter_player - 1] << "\n";
					}
					std::cout << std::setw(21) << "You threw " << std::setw(1) << dice_throw[0] << std::setw(5) << " and " << std::setw(1) << dice_throw[1] << ".";
					std::cout << std::setw(40) << "EXTRACTED PIECES: " << _extracted_pieces[iter_player - 1] << std::endl;

					std::cout << "	You need to place your hit checkers on the table. \n	Please enter the number at which you would like to place one: ";
					move = enter_move();
				placement_checkpoint:
					if (*move == 0) {
						print_rules();
						std::cout << "\n	You need to place your hit checkers on the table. \nPlease enter the number at which you would like to place one: ";
						move = enter_move();
						goto placement_checkpoint;
					// enter_move returns 100 if there is a problem with the input
					} else if (*move == 100) {
						std::cout << "	You need to place your hit checkers on the table. \n	Please enter the number at which you would like to place one: ";
						move = enter_move();
						goto placement_checkpoint;
					}
					// check if only one number is entered (enter_move returns 100 if number not given)
					if (move[1] != 100) {
						std::cout << "	Invalid input! \n";
						std::cout << "	You need to only enter one number that corresponds to a point in your own field! \n";
						std::cout << "	Eg. 19 - 24 for whites and 1 - 6 for blacks. \n";
						std::cout << "	You need to place your hit checkers on the table. \n	Please enter the number at which you would like to place one: ";
						move = enter_move();
						goto placement_checkpoint;
					}	
					if (!_backgammon_board->is_empty_or_one(iter_player, move[0] - 1)) {
						std::cout << "	Invalid input! \n";
						std::cout << "	The point you entered (" << move[0] - 1 << ") is not empty.\n";
						std::cout << "	You need to place your hit checkers on the table. \n	Please enter the number at which you would like to place one: ";
						move = enter_move();
						goto placement_checkpoint;
					}
					// check if dice roll is kept in mind
					if (iter_player == 2 && move[0] != dice_throw[0] && move[0] != dice_throw[1]) {
						std::cout << "	Invalid input! \n";
						std::cout << "	Keep in mind the dice you threw and the direction in which you should move! \n";
						std::cout << "	You need to place your hit checkers on the table. \n	Please enter the number at which you would like to place one: ";
						move = enter_move();
						goto placement_checkpoint;
					}	
					if (iter_player == 1 && move[0] != 25 - dice_throw[0] && move[0] != 25 - dice_throw[1]) {
						std::cout << "	Invalid input! \n";
						std::cout << "	Keep in mind the dice you threw! \n";
						std::cout << "	You need to place your hit checkers on the table. \n	Please enter the number at which you would like to place one: ";
						move = enter_move();
						goto placement_checkpoint;
					}
					// check if this dice was played
					if (iter_player == 1 && move[0] == 25 - dice_played && dice_throw[0] != dice_throw[1]) {
						std::cout << "	Invalid input! \n";
						std::cout << "	This die was played already! Please play the other one!\n";
						std::cout << "	You need to place your hit checkers on the table. \n	Please enter the number at which you would like to place one: ";
						move = enter_move();
						goto placement_checkpoint;
					}
					if (iter_player == 2 && move[0] == dice_played && dice_throw[0] != dice_throw[1]) {
						std::cout << "	Invalid input! \n";
						std::cout << "	This die was played already! Please play the other one!\n";
						std::cout << "	You need to place your hit checkers on the table. \n	Please enter the number at which you would like to place one: ";
						move = enter_move();
						goto placement_checkpoint;
					}
					// place checker back and check if it hit another piece
					hit_checker = _backgammon_board->place_checker(move[0] - 1, iter_player);
					_hit_pieces[2 - iter_player] += (int)hit_checker;
					_hit_pieces[iter_player - 1] --;
					save_move(move[0], move[1], hit_checker);
					// update last played dice
					if (iter_player == 1) {
						dice_played = 25 - move[0];
					} else if (iter_player == 2) {
						dice_played = abs(move[0]);
					}
				} else {
					std::cout << "\n	No move is possible. You cannot place your hit checker.\n";
				}
			}
		}
	}
	delete[] dice_throw;
	delete[] move;
}