
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
    // std::vector<char> testHand;
    // testHand.push_back(1);
    // testHand.push_back(6);
    // testHand.push_back(2);
    // test.printHand(deck, testHand);
    // std::string testString = "[ ][*][ ][G][ ][ ]";
    // test.generateString(deck, testHand, testString);

    // std::cout << testString << std::endl;

    for(int i = 0; i < 10; i++) {
        test.drawCard(deck);
    }

    test.printAll(deck);
}