
#include "Decks.h"
#include "Player.h"
#include "Game.h"

#include <iostream>

int main()
{
    Game simulation;
    simulation.initializeGame();

    StringDeck deck = simulation.getDeck();

    Player test;

    for(int i = 0; i < 11; i++) {
        test.drawCard(deck);
    }

    test.printAll();
}