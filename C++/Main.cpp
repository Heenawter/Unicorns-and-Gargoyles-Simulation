
#include "Constants.h"
#include "Decks.h"
#include "Player.h"
#include "Game.h"

#include <iostream>
#include <thread>
#include <iomanip>
#include <chrono>

std::vector<std::string> readGoals();
void simulateGame(std::ofstream& outputFile);

int main()
{
    std::string goal = "[*][ ][ ][*]";

    Game *simulation = new Game(1, goal);
    Player *player = new Player();
    Deck *deck = simulation->getDeck();

    player->takeTurn(*deck, goal);
    player->takeTurn(*deck, goal);
    player->takeTurn(*deck, goal);
    player->takeTurn(*deck, goal);
    player->takeTurn(*deck, goal);
    player->takeTurn(*deck, goal);
    player->takeTurn(*deck, goal);
    player->springCleaning(*deck, goal);
    // player->takeTurn(*deck, goal);
    // player->takeTurn(*deck, goal);
    // player->takeTurn(*deck, goal);
    // player->takeTurn(*deck, goal);
    // player->takeTurn(*deck, goal);
    // player->takeTurn(*deck, goal);

    // std::ofstream file;
    // file.open(OUTPT_FILE);

    // if(file.is_open()) 
    //     simulateGame(file);
    
    // file.close();
}

void simulateGame(std::ofstream& outputFile)
{
    // keep track of the number of wins per player
    int wins[NUM_PLAYERS];
    // keep track of the size of each player's hand at the end of each round
    int handCounts[NUM_PLAYERS]; 

    // counters
    int playerNum = 0;
    int turnNum = 0;
    int totalCards = 0;
    int ranOutOfCards = 0;
    int ranOutOfCards_sum = 0;

    // game stuff
    Player *player;
    Game *simulation;
    Deck *deck;
    std::vector<std::string>::iterator it; // used to loop through goal list

    // initialize the wins with 0 for each player
    for (playerNum = 0; playerNum < NUM_PLAYERS; playerNum++) 
        wins[playerNum] = 0;

    std::vector<std::string> goals = readGoals(); // list of all goals

    // for each goal....
    bool keepLooping;
    for (it = goals.begin(); it < goals.end(); it++)
    {
        for (playerNum = 0; playerNum < NUM_PLAYERS; playerNum++)
            handCounts[playerNum] = 0;

        std::cout << "\n----" << " GOAL " << std::left << std::setw(15) << *it << " ----" << std::endl;
        outputFile << "----" << " GOAL " << std::left << std::setw(15) << *it << " ----" << std::endl;
        totalCards = 0;
        ranOutOfCards = 0;

        auto t1 = std::chrono::high_resolution_clock::now();
        char gameStatus;
        // run k simulations of that goal...
        for (int k = 0; k < NUM_ROUNDS; k++)
        {
            // std::cout << "Round " << k + 1;
            simulation = new Game(NUM_PLAYERS, *it);

            turnNum = 1;
            keepLooping = true;
            gameStatus = 'X';
            while (keepLooping)
            {
                // std::cout << ".";
                // std::cout << "turn " << turnNum << " --- ";
                // std::cout << "-- new turn -- " << std::endl;
                gameStatus = simulation->gameRound(*it);
                turnNum++;

                if(gameStatus == WIN) {
                    keepLooping = false;
                    wins[simulation->getWinningPlayer()]++;
                } else if (gameStatus == RAN_OUT_OF_CARDS) {
                    keepLooping = false;
                    ranOutOfCards++;
                }
            }

            for(playerNum = 0; playerNum < NUM_PLAYERS; playerNum++) {
                player = simulation->getPlayer(playerNum);
                handCounts[playerNum] += player->getHandSize();
            }
            // std::cout << std::endl;
            totalCards += turnNum;
            delete simulation;
        }

        auto t2 = std::chrono::high_resolution_clock::now();

        auto difference = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
        outputFile << "\t~" << difference / NUM_ROUNDS << " milliseconds to complete." << std::endl;
        outputFile << "\t~" << totalCards / NUM_ROUNDS << " turns/rounds to win." << std::endl;

        for(playerNum = 0; playerNum < NUM_PLAYERS; playerNum++) {
            
            // also output wins for each player FOR EACH GOAL?
            // PERCENTAGE of times ran out of cards rather than number
            outputFile << "\t   - Player " << playerNum + 1 << " had ~" << handCounts[playerNum] / NUM_ROUNDS << " cards." << std::endl;
        }
        outputFile << "\tRan out of cards " << ranOutOfCards << " times (i.e. "
                   << (ranOutOfCards / double(NUM_ROUNDS)) * 100 << "%)." << std::endl << std::endl;

        ranOutOfCards_sum += ranOutOfCards;
    }

    outputFile << "\n-------------------------------------\n" << std::endl;

    for (playerNum = 0; playerNum < NUM_PLAYERS; playerNum++)
    {
        outputFile << "Player " << playerNum + 1 << " won " << wins[playerNum] << " times ";
        outputFile << "(i.e. " << (wins[playerNum] / double(NUM_ROUNDS * 8)) * 100 << "%)." << std::endl;
    }

    outputFile << "Ran out of cards " << ranOutOfCards_sum << " times (i.e. "
               << (ranOutOfCards_sum / double(NUM_ROUNDS * 8)) * 100 << "%)." << std::endl;
}

std::vector<std::string> readGoals()
{
    std::ifstream file;
    file.open(INPUT_FILE);

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