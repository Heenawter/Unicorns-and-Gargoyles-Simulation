
#include "Decks.h"
#include "Player.h"
#include "Game.h"

#include <iostream>

const int NUM_PLAYERS = 2;
const int NUM_ROUNDS = 1000;
const int NUM_CARDS = 8;

int main()
{
    Player test;
    int tryMe = test.stringDistance("[ ][*][ ]", "[ ][*][ ][ ]");
    std::cout << tryMe << std::endl;

    // int wins = 0;
    // bool win;

    // for (int k = 0; k < NUM_ROUNDS; k++)
    // {  
    //     Game* simulation = new Game(NUM_PLAYERS);
    //     StringDeck* deck = simulation->getDeck();
   
    //     Player* player = simulation->getPlayer(0);
    //     for(int i = 0; i < NUM_CARDS; i++) {
    //         win = player->takeTurn(*deck, "[ ][ ][*]");
    //         if(win) {
    //             wins++;
    //             break;
    //         }
    //     }
    //     delete simulation;
    // }

    // std::cout << "We won " << wins << " times and lost " << NUM_ROUNDS - wins << " times!" << std::endl;
}