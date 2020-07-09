/*
Written by Ivo Mihov
ID 9918179
The University of Manchester
30 April 2020

This code was written and tested using Visual Studio 2019 on Windows 10
*/

// The player class enables the users to keep the history of 
// their games and play several games in a row with their own names.

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <sstream>
#include <vector>
#include <cmath>
#include <stdio.h>

#pragma once
class player
{
private:
	std::string _players_name{ "Unnamed" };
	// _players_turn can be 1 or 2 (if 1 player starts with whites and 2 follows with blacks.)
	unsigned _players_turn{ 1 }; 
	unsigned _games_won{ 0 };
	unsigned _games_lost{ 0 };
public:
	player() {}
	player(const std::string players_name, const unsigned players_turn);
	player(const player& player_to_copy);
	player(player&& player_to_move) noexcept;
	player& operator=(const player& player_to_copy);
	player& operator=(player&& player_to_move) noexcept;
	void won_game(const unsigned num_games = 0);
	void lost_game(const unsigned num_games = 0);
	void switch_turn();
	std::string get_name() const { return _players_name; }
	unsigned get_turn() const { return _players_turn; }
	~player() {}
};

player::player(const std::string players_name, const unsigned players_turn)
{
	_games_won = 0;
	_games_lost = 0;
	_players_name = players_name;
	if (players_turn == 1 || players_turn == 2) {
		_players_turn = players_turn;
	} else {
		std::cerr << "Only two players play backgammon. Please state whether this is player 1 or 2. \n";
		exit(2);
	}
}
player::player(const player& player_to_copy)
{
	if (this != &player_to_copy) {
		this->_players_name = player_to_copy._players_name;
		this->_players_turn = player_to_copy._players_turn;
		this->_games_won = player_to_copy._games_won;
		this->_games_lost = player_to_copy._games_lost;
	}
}
player::player(player&& player_to_move) noexcept
{
	if (this != &player_to_move) {
		this->_players_name = player_to_move._players_name;
		this->_players_turn = player_to_move._players_turn;
		this->_games_won = player_to_move._games_won;
		this->_games_lost = player_to_move._games_lost;
		// empty moved player
		player_to_move._players_name = "";
		player_to_move._players_turn = 0;
		player_to_move._games_won = 0;
		player_to_move._games_lost = 0;
	}
}
player& player::operator=(const player& player_to_copy)
{
	if (this != &player_to_copy) {
		this->_players_name = player_to_copy._players_name;
		this->_players_turn = player_to_copy._players_turn;
		this->_games_won = player_to_copy._games_won;
		this->_games_lost = player_to_copy._games_lost;
	}
	return *this;
}
player& player::operator=(player&& player_to_move) noexcept
{
	if (this != &player_to_move) {
		this->_players_name = player_to_move._players_name;
		this->_players_turn = player_to_move._players_turn;
		this->_games_won = player_to_move._games_won;
		this->_games_lost = player_to_move._games_lost;
		// empty moved player
		player_to_move._players_name = "";
		player_to_move._players_turn = 0;
		player_to_move._games_won = 0;
		player_to_move._games_lost = 0;
	}
	return *this;
}
void player::won_game(const unsigned num_games)
{
	_games_won += num_games;
	if (num_games == 2) {
		std::cout << "\n" << _players_name << " won two games since the opponent collected 0 pieces!!!!!!!!\n";
	} else {
		std::cout << "\n" << _players_name << " won this game :-)\n";
	}
}
void player::lost_game(const unsigned num_games)
{
	_games_lost += num_games;
	if (num_games == 2) {
		std::cout << "\n" << _players_name << " lost two games since no pieces were collected :-(\n";
	} else {
		std::cout << "\n" << _players_name << " lost this game :-(\n";
	}
}
void player::switch_turn()
{
	// if the player started first, they will start seconnd now and vice versa
	if (_players_turn == 1) {
		_players_turn = 2;
	} else if (_players_turn == 2) {
		_players_turn = 1;
	}
}