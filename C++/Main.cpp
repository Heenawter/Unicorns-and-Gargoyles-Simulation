
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

// int thread_count = 0;
// bool time_to_quit = false;
// std::mutex m;
// std::condition_variable cv;

// typedef std::chrono::steady_clock Clock;

int main()
{
    Player* player = new Player();
    std::string test1 = "[*][G]";
    std::string test2 = "[*]";
    int result = player->stringDistance(test1, test2);
    std::cout << "The string distance is " << result << std::endl;

    // Game *simulation = new Game(1);
    // Player* player = new Player();
    // Deck *deck = simulation->getDeck();

    // std::string goal = "[*][ ][ ][*]";

    // std::ofstream file;
    // file.open(OUTPT_FILE);

    // if(file.is_open()) 
    //     simulateGame(file);
    
    // file.close();
}

void simulateGame(std::ofstream& outputFile)
{
    int wins[NUM_PLAYERS]; // keep track of the number of wins per player

    // counters
    int playerNum = 0;
    int cardNum = 0;
    int totalCards = 0;
    int ranOutOfCards = 0;

    // game stuff
    Player *player;
    Game *simulation;
    Deck *deck;
    char playerStatus;                     // used to determine win, lose, ran out of cards
    std::vector<std::string>::iterator it; // used to loop through goal list

    // initialize the wins with 0 for each player
    for (playerNum = 0; playerNum < NUM_PLAYERS; playerNum++)
        wins[playerNum] = 0;

    std::vector<std::string> goals = readGoals(); // list of all goals

    // for each goal....
    bool keepLooping;
    for (it = goals.begin(); it < goals.end(); it++)
    {
        std::cout << "\n----" << " GOAL " << std::left << std::setw(15) << *it << " ----" << std::endl;
        outputFile << "----" << " GOAL " << std::left << std::setw(15) << *it << " ----" << std::endl;
        totalCards = 0;

        auto t1 = std::chrono::high_resolution_clock::now();
        int distance;
        // run k simulations of that goal...
        for (int k = 0; k < NUM_ROUNDS; k++)
        {
            // std::cout << "Round " << k + 1;
            simulation = new Game(NUM_PLAYERS);
            deck = simulation->getDeck();

            playerStatus = ' ';
            cardNum = 1;
            keepLooping = true;
            while (keepLooping)
            {
                // std::cout << ".";
                for (playerNum = 0; playerNum < NUM_PLAYERS & keepLooping; playerNum++)
                {
                    player = simulation->getPlayer(playerNum);
                    distance = player->takeTurn(*deck, *it);
                    if(distance == 0) {
                        keepLooping = false;
                        wins[playerNum]++;
                    }
                    
                    // check if still cards to draw
                    if(!deck->hasCards()) {
                        keepLooping = false;
                        ranOutOfCards++;
                    }
                    
                }
                cardNum++;
            }
            // std::cout << std::endl;
            totalCards += cardNum;
            delete simulation;
        }

        auto t2 = std::chrono::high_resolution_clock::now();

        auto difference = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
        outputFile << "\t~" << difference / NUM_ROUNDS << " milliseconds to complete." << std::endl;
        outputFile << "\t~" << totalCards / NUM_ROUNDS << " cards to win." << std::endl;
    }

    outputFile << "\n-------------------------------------\n" << std::endl;


    for (playerNum = 0; playerNum < NUM_PLAYERS; playerNum++)
    {
        outputFile << "Player " << playerNum + 1 << " won " << wins[playerNum] << " times ";
        outputFile << "(i.e. " << (wins[playerNum] / double(NUM_ROUNDS * 8)) * 100 << "%)." << std::endl;
    }
  
    outputFile << "Ran out of cards " << ranOutOfCards << " times." << std::endl;
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