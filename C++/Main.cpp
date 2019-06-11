
#include "Decks.h"
#include "Player.h"
#include "Game.h"

#include <iostream>
#include<iomanip>
#include <chrono>

const int NUM_PLAYERS = 4;
const int NUM_ROUNDS = 3;
const int NUM_CARDS = 7;

const int NUM_LINES_TO_SKIP = 24;

std::vector<std::string> readGoals();

    int main()
{
    int wins[NUM_PLAYERS];
    int playerNum;
    for (playerNum = 0; playerNum < NUM_PLAYERS; playerNum++)
        wins[playerNum] = 0;
    bool win;

    int cardNum;
    Player *player;
    Game *simulation;
    Deck *deck;

    std::vector<std::string>
    goals = readGoals();
    std::vector<std::string>::iterator it;
    // for each goal....
    for(it = goals.begin(); it < goals.end(); it++) {
        std::cout << "----" << " GOAL " << std::left << std::setw(15) << *it << " ----" << std::endl;    

        auto t1 = std::chrono::high_resolution_clock::now();
        // run k simulations of that goal...
        for (int k = 0; k < NUM_ROUNDS; k++)
        {
            std::cout << "Round " << k + 1;
            simulation = new Game(NUM_PLAYERS);
            deck = simulation->getDeck();
            
            win = false;
            cardNum = 1;
            while(!win)
            {
                std::cout << ".";
                for (playerNum = 0; playerNum < NUM_PLAYERS; playerNum++)
                {
                    player = simulation->getPlayer(playerNum);
                    win = player->takeTurn(*deck, *it);
                    if (win)
                    {
                        std::cout << " Player " << playerNum << " wins!";
                        wins[playerNum]++;
                        break;
                    }
                }
                cardNum++;
            }
            std::cout << std::endl;

            delete simulation;
        }

        auto t2 = std::chrono::high_resolution_clock::now();
    }

    // for(playerNum = 0; playerNum < NUM_PLAYERS; playerNum++) {
    //     std::cout << "Player " << playerNum + 1 << " won " << wins[playerNum] << " times." << std::endl;
    // }

    // auto difference = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();
    // std::cout << "Each round took approximately " << difference / NUM_ROUNDS << " milliseconds to complete." << std::endl;
}

std::vector<std::string> readGoals()
{
    std::ifstream file;
    file.open("Draft1_Stats.txt");
    
    int numCards;
    std::vector<std::string> allGoals;
    std::string line;
    int i;

    if (file.is_open())
    {
        /* skip comment/uncomment and action cards */
        for (i = 0; i < NUM_LINES_TO_SKIP; i++)
            std::getline(file, line); // garbage read

        /* read the goal deck */
        file >> numCards;         // the number of cards to read
        std::getline(file, line); // garbage read of new line
        for (i = 0; i < numCards; i++)
        {
            std::getline(file, line);
            allGoals.push_back(line);
        }

        file.close();
    }

    return allGoals;
}