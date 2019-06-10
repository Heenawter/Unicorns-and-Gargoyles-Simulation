
#include "Decks.h"
#include "Player.h"
#include "Game.h"

#include <iostream>
#include <chrono>

const int NUM_PLAYERS = 2;
const int NUM_ROUNDS = 5000;
const int NUM_CARDS = 7;

int main()
{
    // Player test;
    // int tryMe = test.stringDistance("[ ][*][ ]", "[ ][*][ ][ ]");
    // std::cout << tryMe << std::endl;

    int wins = 0;
    bool win = false;

    auto t1 = std::chrono::high_resolution_clock::now();

    int playerNum;
    Player* player;
    // for (int k = 0; k < NUM_ROUNDS; k++)
    // {  

        Game *simulation = new Game(NUM_PLAYERS);
        StringDeck *deck = simulation->getDeck();
        while(!win)
        {
            for (playerNum = 0; playerNum < NUM_PLAYERS; playerNum++)
            {
                player = simulation->getPlayer(playerNum);
                win = player->takeTurn(*deck, "[ ][ ][*]");
                if (win)
                {
                    wins++;
                    std::cout << "Player " << playerNum + 1 << " wins!" << std::endl;
                    break;
                }
            }
        }
    
        delete simulation;
    // }

    auto t2 = std::chrono::high_resolution_clock::now();

    std::cout << "We won " << wins << " times and lost " << NUM_ROUNDS - wins << " times!" << std::endl;
    auto difference = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();
    // std::cout << "Each round took approximately " << difference / NUM_ROUNDS << " milliseconds to complete." << std::endl;
}