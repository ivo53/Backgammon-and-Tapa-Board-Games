/*
Written by Ivo Mihov
ID 9918179
The University of Manchester
30 April 2020

This code was written and tested using Visual Studio 2019 on Windows 10
*/

// The board class contains all the information about the checkers on the board.
// It provides numerous functions that describe the status of the board.
// An object of type board is included in the game class and is needed for the board games.

#pragma once

#include "player.h"

class board
{
private:
	// keep number of checkers in each point (positive for whites, negative for blacks)
	int* _checkers{ new int[24] }; 
	// save positions where piece is blocked in tapa
	bool* _stuck{ nullptr };
	bool _playing_tapa{ false };
public:
	board(const bool playing_tapa = false);
	board(const int* points, const bool* stuck, const bool playing_tapa);
	board(const board& board_to_copy);
	board(board&& board_to_move) noexcept;
	board& operator=(const board& board_to_copy);
	board& operator=(board&& board_to_copy) noexcept;
	// order for different board games
	void order_standard();
	void order_tapa();
	// checks for status of board
	bool is_move_possible(const unsigned location, const unsigned destination) const;
	bool is_placement_possible(const int players_turn, const unsigned dice1, const unsigned dice2) const;
	bool is_extraction_possible(const int players_turn, const unsigned location) const;
	bool is_empty_or_one(const int players_turn, const unsigned point) const;
	bool is_tie() const;
	bool is_all_in_own(const unsigned players_turn) const;
	int furthest_piece_location(const int players_turn) const;
	bool list_possible_moves(const unsigned player, const unsigned dice1, const unsigned dice2 = 0) const;
	void print_board();
	// actions on board
	bool move_checker(const unsigned location, const unsigned destination) const;
	void move_checker_back(const unsigned location, const unsigned destination);
	void extract_checker(const unsigned location, const unsigned players_turn);
	bool place_checker(const unsigned location, const unsigned players_turn) const;
	void add_checker(const unsigned location, const unsigned players_turn);
	~board() { delete[] _checkers; delete[] _stuck; };
};
board::board(const bool playing_tapa)
{
	if (playing_tapa) {
		_stuck = new bool[24];
	}
	_playing_tapa = playing_tapa;
	if (playing_tapa) {
		order_tapa();
	} else {
		order_standard();
	}

}
board::board(const int* points, const bool* stuck, const bool playing_tapa)
{
	// this parametrised constructor can make a new game that uses the same board
	_checkers = new int[24];
	_stuck = new bool[24];
	for (unsigned iter{ 0 }; iter < 24; iter++) {
		_checkers[iter] = points[iter];
		_stuck[iter] = stuck[iter];
	}
	_playing_tapa = playing_tapa;
}
board::board(const board& board_to_copy)
{
	if (this != &board_to_copy) {
		if (board_to_copy._stuck != nullptr) {
			_stuck = new bool[24];
		}
		for (unsigned iter{ 0 }; iter < 24; iter++) {
			_checkers[iter] = board_to_copy._checkers[iter];
			if (board_to_copy._stuck != nullptr) {
				_stuck[iter] = board_to_copy._stuck[iter];
			}
		}
		_playing_tapa = board_to_copy._playing_tapa;
	}
}
board::board(board&& board_to_move) noexcept
{
	if (this != &board_to_move) {
		this->_checkers = board_to_move._checkers;
		this->_stuck = board_to_move._stuck;
		this->_playing_tapa = board_to_move._playing_tapa;
		// empty old object
		board_to_move._checkers = nullptr;
		board_to_move._stuck = nullptr;
		board_to_move._playing_tapa = false;
	}
}
board& board::operator=(const board& board_to_copy)
{
	if (this != &board_to_copy) {
		if (board_to_copy._stuck != nullptr) {
			_stuck = new bool[24];
		}
		for (unsigned iter{ 0 }; iter < 24; iter++) {
			_checkers[iter] = board_to_copy._checkers[iter];
			if (board_to_copy._stuck != nullptr) {
				_stuck[iter] = board_to_copy._stuck[iter];
			}
		}
		_playing_tapa = board_to_copy._playing_tapa;
	}
	return *this;
}

board& board::operator=(board&& board_to_move) noexcept
{
	if (this != &board_to_move) {
		this->_checkers = board_to_move._checkers;
		this->_stuck = board_to_move._stuck;
		this->_playing_tapa = board_to_move._playing_tapa;
		// empty old object
		board_to_move._checkers = nullptr;
		board_to_move._stuck = nullptr;
		board_to_move._playing_tapa = false;
	}
	return *this;
}

void board::order_standard()
{
	for (unsigned iter{ 0 }; iter < 24; iter++) {
		_checkers[iter] = 0;
	}
	_checkers[5] = 5;
	_checkers[7] = 3;
	_checkers[12] = 5;
	_checkers[23] = 2;
	_checkers[0] = -2;
	_checkers[11] = -5;
	_checkers[16] = -3;
	_checkers[18] = -5;
}

void board::order_tapa()
{
	for (unsigned iter{ 0 }; iter < 24; iter++) {
		_checkers[iter] = 0;
		_stuck[iter] = false;
	}
	_checkers[0] = -15;
	_checkers[23] = 15;
}

bool board::is_move_possible(const unsigned location, const unsigned destination) const
{
	// check which colour of checkers is being move and whether direction is right
	if (_checkers[location] > 0 && location > destination) {
		// check whether destination is empty or has one piece
		if (_checkers[destination] < -1) {
			return false;
		} else if (_stuck != nullptr) {
			// check whether a piece is blocked in destination (tapa)
			if (_checkers[destination] < 0 && _stuck[destination]) {
				return false;
			} else {
				return true;
			}
		} else {
			return true;
		}
	} else if (_checkers[location] < 0 && location < destination) {
		// check whether destination is empty or has one piece
		if (_checkers[destination] > 1) {
			return false;
		} else if (_stuck != nullptr) {
			// check whether a piece is blocked in destination (tapa)
			if (_checkers[destination] > 0 && _stuck[destination]) {
				return false;
			} else { 
				return true; 
			}
		} else {
			return true;
		}
	}
	return false;
}
bool board::is_placement_possible(const int players_turn, const unsigned dice1, const unsigned dice2) const
{
	if (players_turn == 1) {
		if (_checkers[24 - dice1] < -1 && _checkers[24 - dice2] < -1) {
			return false;
		} else {
			return true;
		}
	} else if (players_turn == 2) {
		if (_checkers[dice1 - 1] > 1 && _checkers[dice2 - 1] > 1) {
			return false;
		} else {
			return true;
		}
	} else {
		exit(8);
	}
}
bool board::is_extraction_possible(const int players_turn, const unsigned location) const
{
	const int multiplier{ -(players_turn * 2 - 3) }; // 1 or -1, gives the sign for checkers of different colours
	const unsigned limit1[2]{ 0, 18 };
	const unsigned limit2[2]{ 6, 24 };
	if (_checkers[location] * multiplier > 0 && location >= limit1[players_turn - 1] && location < limit2[players_turn - 1]) {
		return true;
	} else {
		return false;
	}
}
bool board::is_empty_or_one(const int players_turn, const unsigned point) const
{
	// multiplier = -1 for player 1 and 1 for player 2
	const int multiplier{ -(players_turn * 2 - 3) }; 
	if (_checkers[point] * multiplier < -1) {
		return false;
	} else if (_stuck != nullptr) {
		if (_stuck[point] && _checkers[point] * multiplier < 0) {
			return false;
		}
	} 
	return true;
}
bool board::is_tie() const 
{
	// check for tie (for tapa)
	if ((_stuck[23] && _checkers[23] < 0) && (_stuck[0] && _checkers[0] > 0)) {
		std::cout << "\nThis is a tie!!\n";
		return true;
	}
	return false;
}
bool board::list_possible_moves(const unsigned player, const unsigned dice1, const unsigned dice2) const 
{
	unsigned move_counter{ 0 };
	if (player == 1) {
		for (int pos{ 0 }; pos < 24; pos++) {
			// iterate over all possible one-step moves with given dice roll
			if (_checkers[pos] > 0) {
				if (pos - dice1 > 0) {
					if (is_move_possible(pos, pos - dice1)) {
						std::cout << "	You can move checker from " << pos + 1 << " to " << pos + 1 - dice1 << std::endl;
						move_counter++;
					}
				}
				if (dice2 != 0) {
					if (pos - dice2 > 0) {
						if (is_move_possible(pos, pos - dice2)) {
							std::cout << "	You can move checker from " << pos + 1 << " to " << pos + 1 - dice2 << std::endl;
							move_counter++;
						}
					}
				}
			}
		}
	}
	if (player == 2) {
		for (int pos{ 0 }; pos < 24; pos++) {
			if (_checkers[pos] < 0) {
				if (pos + dice1 < 23) {
					if (is_move_possible(pos, pos + dice1)) {
						std::cout << "	You can move checker from " << pos + 1 << " to " << pos + 1 + dice1 << std::endl;
						move_counter++;
					}
				}
				if (dice2 != 0) {
					if (pos + dice2 < 23) {
						if (is_move_possible(pos, pos + dice2)) {
							std::cout << "	You can move checker from " << pos + 1 << " to " << pos + 1 + dice2 << std::endl;
							move_counter++;
						}
					}
				}
			}
		}
	}
	if (player != 1 && player != 2) {
		std::cerr << "Only two players play backgammon. Please state whether this is player 1 or 2. \n";
		exit(2);
	}
	return move_counter > 0;
}
bool board::move_checker(const unsigned location, const unsigned destination) const
{
	bool hit_checker{ false };
	if (is_move_possible(location, destination)) {
		if (_checkers[location] > 0) {
			_checkers[location]--;
			// check if landing on an opponent's checker
			if (_checkers[destination] < 0) {
				_checkers[destination] += 2;
				// set stuck to true for tapa (blocked checker)
				if (_stuck != nullptr) {
					_stuck[destination] = true;
				}
				hit_checker = true;
			} else {
				_checkers[destination] ++;
				hit_checker = false;
			}
			if (_stuck != nullptr){
				if (_stuck[location] && _checkers[location] == 0) {
					_stuck[location] = false;
					_checkers[location] --;
				}
			}
		} else if (_checkers[location] < 0) {
			_checkers[location]++;
			// check if landing on an opponent's checker
			if (_checkers[destination] > 0) {
				_checkers[destination] -= 2;
				if (_stuck != nullptr) {
					// set stuck to true for tapa (blocked checker)
					_stuck[destination] = true;
				}
				hit_checker = true;
			} else {
				_checkers[destination] --;
				hit_checker = false;
			}
			// check checker is freed by move
			if (_stuck != nullptr) {
				if (_stuck[location] && _checkers[location] == 0) {
					_stuck[location] = false;
					_checkers[location] ++;
				}
			}
		}
	} else {
		std::cerr << "Move is not possible!\n";
		exit(7);
	}
	return hit_checker;
}
void board::move_checker_back(const unsigned location, const unsigned destination)
{
	if (_checkers[location] > 0) {
		_checkers[location]--;
		// check if landing on an opponent's checker
		if (_checkers[destination] < 0) {
			_checkers[destination] += 2;
			// set stuck to true for tapa (blocked checker)
			if (_stuck != nullptr) {
				_stuck[destination] = true;
			}
		} else {
			_checkers[destination] ++;
		}
		if (_stuck != nullptr) {
			if (_stuck[location] && _checkers[location] == 0) {
				_stuck[location] = false;
				_checkers[location] --;
			}
		}
	} else if (_checkers[location] < 0) {
		_checkers[location]++;
		// check if landing on an opponent's checker
		if (_checkers[destination] > 0) {
			_checkers[destination] -= 2;
			if (_stuck != nullptr) {
				// set stuck to true for tapa (blocked checker)
				_stuck[destination] = true;
			}
		} else {
			_checkers[destination] --;
		}
		// check checker is freed by move
		if (_stuck != nullptr) {
			if (_stuck[location] && _checkers[location] == 0) {
				_stuck[location] = false;
				_checkers[location] ++;
			}
		}
	}
}

void board::extract_checker(const unsigned location, const unsigned players_turn) 
{
	if (is_all_in_own(players_turn)) {
		if (players_turn == 1 && location < 6 && location >= 0 && _checkers[location] > 0) {
			_checkers[location] --;
			if (_stuck != nullptr) {
				if (_stuck[location] && _checkers[location] == 0) {
					_stuck[location] = false;
					_checkers[location] --;
				}
			}
		}
		if (players_turn == 2 && location < 24 && location >= 18 && _checkers[location] < 0) {
			_checkers[location] ++;
			if (_stuck != nullptr) {
				if (_stuck[location] && _checkers[location] == 0) {
					_stuck[location] = false;
					_checkers[location] ++;
				}
			}
		}
	} else {
		std::cerr << "Bug in system, extraction of this piece is impossible.\n";
		exit(6);
	}
}
bool board::place_checker(const unsigned location, const unsigned players_turn) const
{
	// multiplier = -1 for player 1 and 1 for player 2
	const int multiplier = (players_turn * 2 - 3); 
	// check if sommething is hit by placed checker
	if (_checkers[location] * multiplier == 1) {
		_checkers[location] = _checkers[location] - 2 * multiplier;
		if (_stuck != nullptr) {
			_stuck[location] = true;
		}
		// return true if hit, false if not
		return true;
	} else if (_checkers[location] * multiplier == 0) {
		_checkers[location] = _checkers[location] - multiplier;
		return false;
	} else if (-_checkers[location] * multiplier > 0) {
		_checkers[location] = _checkers[location] - multiplier;
		return false;
	} else {
		exit(5);
	}
}

void board::add_checker(const unsigned location, const unsigned players_turn)
{
	if (players_turn == 1) {
		_checkers[location] ++;
	} else if (players_turn == 2) {
		_checkers[location] --;
	}
}

int board::furthest_piece_location(const int players_turn) const
{
	int furthest_location{ 23 * (2 - players_turn) };
	// multiplier = 1 for player 1 and -1 for player 2
	const int multiplier{ -(players_turn * 2 - 3) }; 
	for (int iter{ 0 }; iter < 24; iter++) {
		// iterate over all points for both players simultaneously (using multiplier)
		if (_checkers[23 * (players_turn - 1) + multiplier * iter] * multiplier > 0) {
			furthest_location = 23 * (players_turn - 1) + multiplier * iter;
		}
	}
	return furthest_location + 1;
}
bool board::is_all_in_own(const unsigned players_turn) const
{
	unsigned count_empty_white{ 0 };
	unsigned count_empty_black{ 0 };
	// loop over all points to check how many checkers are outside of own field of players
	for (unsigned iter{ 0 }; iter < 24; iter++) {
		if (_checkers[iter] <= 0 && iter > 5) {
			count_empty_white++;
		}
		if (_checkers[iter] >= 0 && iter < 18) {
				count_empty_black++;
		}
	}
	if (players_turn == 1 && count_empty_white == 18) {
		return true;
	}
	if (players_turn == 2 && count_empty_black == 18) {
		return true;
	}
	return false;
}

void board::print_board()
{
	const char white_checker{ 'w' };
	const char black_checker{ 'b' };
	std::cout << "\n           ";
	for (int iter_col{ 0 }; iter_col < 12; iter_col++) {
		std::cout <<std::setw(4) << iter_col + 13;
		if (iter_col == 5) {
			std::cout << std::setw(6) << "   ";
		}
	}
	std::cout << "\n          ----------------------------------------------------------\n";
	// draw upper part
	for (int iter_row{ 0 }; iter_row < 8; iter_row++) {
		std::cout << "          |";
		for (int iter_col{ 0 }; iter_col < 12; iter_col++) {
			if (!_playing_tapa) {
				if (_checkers[iter_col + 12] > iter_row) {
					std::cout << std::setw(4) << white_checker;
				} else if (_checkers[iter_col + 12] < -1 * iter_row) {
					std::cout << std::setw(4) << black_checker;
				} else {
					std::cout << std::setw(4) << ".";
				}
			} else {
				// check for blocked checkers if playing tapa
				if (_stuck[iter_col + 12]) {
					if (iter_row == 0) {
						if (_checkers[iter_col + 12] > 0) {
							std::cout << std::setw(4) << black_checker;
						} else if (_checkers[iter_col + 12] < 0) {
							std::cout << std::setw(4) << white_checker;
						}
					} else {
						if (_checkers[iter_col + 12] > iter_row - 1) {
							std::cout << std::setw(4) << white_checker;
						} else if (_checkers[iter_col + 12] < -1 * iter_row + 1) {
							std::cout << std::setw(4) << black_checker;
						} else {
							std::cout << std::setw(4) << ".";
						}
					}
				} else {
					if (_checkers[iter_col + 12] > iter_row) {
						std::cout << std::setw(4) << white_checker;
					} else if (_checkers[iter_col + 12] < -1 * iter_row) {
						std::cout << std::setw(4) << black_checker;
					} else {
						std::cout << std::setw(4) << ".";
					}
				}
			}
			if (iter_col == 5) {
				std::cout << std::setw(6) << "|||";
			}
		}
		std::cout << std::setw(4) << "|\n";
	}
	// draw central part
	for (int iter_row{ 0 }; iter_row < 3; iter_row++) {
		std::cout << "          |";
		for (int iter_col{ 0 }; iter_col < 12; iter_col++) {
			// add +n where n is the number of checkers in triangle that could not be printed 
			if (_playing_tapa) {
				// again take blocked checker into account if playing tapa
				if (iter_row == 0 && _checkers[iter_col + 12] > 8 - (int)_stuck[iter_col + 12]) {
					std::cout << std::setw(3) << '+' << _checkers[iter_col + 12] - 8 + (int)_stuck[iter_col + 12];
				} else if (iter_row == 0 && _checkers[iter_col + 12] < -8) {
					std::cout << std::setw(3) << '+' << -8 + (int)_stuck[iter_col + 12] - _checkers[iter_col + 12];
				} else if (iter_row == 2 && _checkers[11 - iter_col] > 8 - (int)_stuck[11 - iter_col]) {
					std::cout << std::setw(3) << '+' << _checkers[11 - iter_col] - 8 + (int)_stuck[11 - iter_col];
				} else if (iter_row == 2 && _checkers[11 - iter_col] < -8) {
					std::cout << std::setw(3) << '+' << -8 + (int)_stuck[11 - iter_col] - _checkers[11 - iter_col];
				} else {
					std::cout << std::setw(4) << " ";
				}
			} else {
				if (iter_row == 0 && _checkers[iter_col + 12] > 8) {
					std::cout << std::setw(3) << '+' << _checkers[iter_col + 12] - 8;
				} else if (iter_row == 0 && _checkers[iter_col + 12] < -8) {
					std::cout << std::setw(3) << '+' << -8 - _checkers[iter_col + 12];
				} else if (iter_row == 2 && _checkers[11 - iter_col] > 8) {
					std::cout << std::setw(3) << '+' << _checkers[11 - iter_col] - 8;
				} else if (iter_row == 2 && _checkers[11 - iter_col] < -8) {
					std::cout << std::setw(3) << '+' << -8 - _checkers[11 - iter_col];
				} else {
					std::cout << std::setw(4) << " ";
				}
			}
			if (iter_col == 5) {
				std::cout << std::setw(6) << "|||";
			}
		}
		std::cout << std::setw(4) << "|\n";
	}
	// draw lower part
	for (int iter_row{ 7 }; iter_row >= 0; iter_row--) {
		std::cout << "          |";
		for (int iter_col{ 11 }; iter_col >= 0; iter_col--) {
			if (!_playing_tapa) {
				if (_checkers[iter_col] > iter_row) {
					std::cout << std::setw(4) << white_checker;
				} else if (_checkers[iter_col] < -1 * iter_row) {
					std::cout << std::setw(4) << black_checker;
				} else {
					std::cout << std::setw(4) << ".";
				}
				if (iter_col == 6) {
					std::cout << std::setw(6) << "|||";
				}
			} else {
				if (_stuck[iter_col]) {
					if (iter_row == 0) {
						if (_checkers[iter_col] > 0) {
							std::cout << std::setw(4) << black_checker;
						} else if (_checkers[iter_col] < 0) {
							std::cout << std::setw(4) << white_checker;
						}
					} else {
						if (_checkers[iter_col] > iter_row - 1) {
							std::cout << std::setw(4) << white_checker;
						} else if (_checkers[iter_col] < -1 * iter_row + 1) {
							std::cout << std::setw(4) << black_checker;
						} else {
							std::cout << std::setw(4) << ".";
						}
					}
					if (iter_col == 6) {
						std::cout << std::setw(6) << "|||";
					}
				} else {
					if (_checkers[iter_col] > iter_row) {
						std::cout << std::setw(4) << white_checker;
					} else if (_checkers[iter_col] < -1 * iter_row) {
						std::cout << std::setw(4) << black_checker;
					} else {
						std::cout << std::setw(4) << ".";
					}
					if (iter_col == 6) {
						std::cout << std::setw(6) << "|||";
					}
				}
			}
		}
		std::cout << std::setw(4) << "|\n";
	}
	std::cout << "          ----------------------------------------------------------\n";
	std::cout << "           ";
	for (int iter_col{ 12 }; iter_col > 0; iter_col--) {
		std::cout << std::setw(4) << iter_col ;
		if (iter_col == 7) {
			std::cout << std::setw(6) << "   ";
		}
	}
	std::cout << "\n\n";
}