
#include "Constants.h"
#include "Decks.h"
#include "Player.h"
#include "Game.h"


#include <iostream>
#include <iomanip>
#include <chrono>

std::vector<std::string> readGoals();

int main()
{
    int wins[NUM_PLAYERS]; // keep track of the number of wins per player
    
    // counters
    int playerNum = 0;
    int cardNum = 0;
    int numFails = 0;
    int totalCards = 0;

    // game stuff
    Player *player;
    Game *simulation;
    Deck *deck;
    char playerStatus; // used to determine win, lose, ran out of cards
    std::vector<std::string>::iterator it; // used to loop through goal list

    // initialize the wins with 0 for each player
    for (playerNum = 0; playerNum < NUM_PLAYERS; playerNum++)
        wins[playerNum] = 0;

    std::vector<std::string> goals = readGoals(); // list of all goals
    
    // for each goal....
    for(it = goals.begin(); it < goals.end(); it++) {
        std::cout << "----" << " GOAL " << std::left << std::setw(15) << *it << " ----" << std::endl;    
        totalCards = 0;

        auto t1 = std::chrono::high_resolution_clock::now();
        // run k simulations of that goal...
        for (int k = 0; k < NUM_ROUNDS; k++)
        {
            std::cout << "Round " << k + 1;
            simulation = new Game(NUM_PLAYERS);
            deck = simulation->getDeck();
            
            playerStatus = ' ';
            cardNum = 1;
            while (playerStatus != WIN and playerStatus != RAN_OUT_OF_CARDS)
            {
                std::cout << ".";
                for (playerNum = 0; playerNum < NUM_PLAYERS; playerNum++)
                {
                    player = simulation->getPlayer(playerNum);
                    playerStatus = player->takeTurn(*deck, *it);
                    if (playerStatus == WIN)
                    {
                        std::cout << " Player " << playerNum << " wins!";
                        wins[playerNum]++;
                        break;
                    } else if (playerStatus == RAN_OUT_OF_CARDS) {
                        std::cout << "Ran out of cards!";
                        numFails++;
                        break;
                    }
                }
                cardNum++;
            }
            std::cout << std::endl;
            totalCards += cardNum;

            delete simulation;
        }

        auto t2 = std::chrono::high_resolution_clock::now();

        auto difference = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
        std::cout << "Each round took approximately " << difference / NUM_ROUNDS << " milliseconds to complete." << std::endl;
        std::cout << "It took, on average, " << totalCards / NUM_ROUNDS << " cards to win." << std::endl;
    }

    for(playerNum = 0; playerNum < NUM_PLAYERS; playerNum++) 
        std::cout << "Player " << playerNum + 1 << " won " << wins[playerNum] << " times." << std::endl;

    std::cout << "The game failed " << numFails << " times." << std::endl;
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