
#include "Decks.h"
#include "Player.h"
#include "Game.h"

#include <iostream>
#include <chrono>

const int NUM_PLAYERS = 4;
const int NUM_ROUNDS = 20;
const int NUM_CARDS = 7;

int main()
{
    int wins[NUM_PLAYERS];
    int playerNum;
    for (playerNum = 0; playerNum < NUM_PLAYERS; playerNum++)
        wins[playerNum] = 0;
    bool win;

    auto t1 = std::chrono::high_resolution_clock::now();

    int cardNum;
    Player* player;
    for (int k = 0; k < NUM_ROUNDS; k++)
    {
        std::cout << "--- Round " << k + 1 << " ---" << std::endl;
        Game *simulation = new Game(NUM_PLAYERS);
        StringDeck *deck = simulation->getDeck();
        win = false;
        cardNum = 1;
        while(!win)
        {
            std::cout << cardNum;
            for (playerNum = 0; playerNum < NUM_PLAYERS; playerNum++)
            {
                player = simulation->getPlayer(playerNum);
                win = player->takeTurn(*deck, "[ ][ ][*]");
                if (win)
                {
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

    for(playerNum = 0; playerNum < NUM_PLAYERS; playerNum++) {
        std::cout << "Player " << playerNum + 1 << " won " << wins[playerNum] << " times." << std::endl;
    }

    auto difference = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();
    std::cout << "Each round took approximately " << difference / NUM_ROUNDS << " milliseconds to complete." << std::endl;
}