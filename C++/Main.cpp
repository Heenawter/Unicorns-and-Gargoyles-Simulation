
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
    bool win;

    auto t1 = std::chrono::high_resolution_clock::now();

    for (int k = 0; k < NUM_ROUNDS; k++)
    {  
        Game* simulation = new Game(NUM_PLAYERS);
        StringDeck* deck = simulation->getDeck();
   
        Player* player = simulation->getPlayer(0);
        for(int i = 0; i < NUM_CARDS; i++) {
            win = player->takeTurn(*deck, "[ ][ ][*]");
            if(win) {
                wins++;
                break;
            }
        }
        delete simulation;
    }

    auto t2 = std::chrono::high_resolution_clock::now();

    std::cout << "We won " << wins << " times and lost " << NUM_ROUNDS - wins << " times!" << std::endl;
    auto difference = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();
    std::cout << "Each round took approximately " << difference / NUM_ROUNDS << " milliseconds to complete." << std::endl;
}