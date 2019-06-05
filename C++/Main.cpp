
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
    test.drawCard(deck);
    test.drawCard(deck);
    test.drawCard(deck);
    test.drawCard(deck);

    // test.drawCard(deck);
    // test.drawCard(deck);
    // test.drawCard(deck);
    // test.drawCard(deck);

    // test.drawCard(deck);
    // test.drawCard(deck);
    // test.drawCard(deck);
    // test.drawCard(deck);

    // std::cout << "Hello!" << std::endl;

    // StringDeck deck;

    // std::string testString = "[ ][*][ ][G][ ]";
    // std::cout << testString << std::endl;
    // deck.playCard("reverse order", testString);
    // std::cout << testString << std::endl;

    // std::cout << "this is a test" << std::endl;
}