/*
Written by Ivo Mihov
ID 9918179
The University of Manchester
30 April 2020

This code was written and tested using Visual Studio 2019 on Windows 10
*/

#include "backgammon.h"
#include "tapa.h"

int main()
{
    std::string name_1;
    std::string name_2;
    std::string game_answer;
    std::string play_again;
    bool play_tapa{ false };
    // put time random seed for different pseudo random numbers
    srand((unsigned)time(NULL));
    std::cout << "Please enter the name of Player 1: ";
    getline(std::cin >> std::ws, name_1);
    std::cout << "Please enter the name of Player 2: ";
    getline(std::cin >> std::ws, name_2);
    player* Player1 = new player(name_1, 1);
    player* Player2 = new player(name_2, 2);
    // make array of board games
    game** games{ new game* [2] };
    games[0] = new backgammon(Player1, Player2);
    games[1] = new tapa(Player1, Player2);
game_choice_checkpoint:
    std::cout << "Would you like to play [b]ackgammon or [t]apa? (type 'exit' to quit)\n";
    getline(std::cin >> std::ws, game_answer);
    while (game_answer != "b" && game_answer != "backgammon" && game_answer != "t" && game_answer != "tapa" && game_answer != "exit") {
        std::cout << "\nPlease enter a valid game choice: [b]ackgammon or [t]apa!\n";
        std::cin.clear();
        goto game_choice_checkpoint;
    }
    // set play_tapa to true or false depending on game chosen by player
    if (game_answer == "b" || game_answer == "backgammon") {
        play_tapa = false;
    } else if (game_answer == "t" || game_answer == "tapa") {
        play_tapa = true;
    } else if (game_answer == "exit") {
        std::cout << "\nThank you for playing!\n";
        exit(EXIT_SUCCESS);
    } else {
        std::cerr << "game_answer has to be 'b', 't', 'backgammon' or 'tapa'! Invalid answer!";
        exit(EXIT_FAILURE);
    }
    // polymorphic call to start game
    games[(unsigned)play_tapa]->begin();
    std::cout << "\nWould you like to play again? [y/n]\n";
    getline(std::cin >> std::ws, play_again);
    while (play_again != "y" && play_again != "yes" && play_again != "Y" && play_again != "n" && play_again != "no" && play_again != "N") {
        std::cout << "\nPlease enter either [y]es or [n]o!\n";
        std::cin.clear();
        std::cout << "\nWould you like to play again? [y/n]\n";
        getline(std::cin >> std::ws, play_again);
    }
    if (play_again == "y" || play_again == "yes" || play_again == "Y") {
        goto game_choice_checkpoint;
    } else {
        std::cout << "\nThank you for playing!\n";
    }
    delete[] games;
    return 0;
}
