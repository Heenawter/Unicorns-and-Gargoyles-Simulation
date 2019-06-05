#include "Decks.h"
#include "Game.h"

#include <iostream>
#include <fstream>

void Game::readGameStats() {
    std::ifstream file;
    file.open("Draft1_Stats.txt");

    if(file.is_open()) {
        std::string line;
        int numCards;
        int i;

        /* read the string deck */
        std::getline(file, line); // garbage read of first line
        file >> numCards; // the number of cards to read
        std::getline(file, line); // garbage read of new line
        for(i = 0; i < numCards - 2; i++)
        { // -2 because I want to ignore comment/uncomment for now
            std::getline(file, line);
            deck.addToDeck(line);
        }

        /* skip comment/uncomment and action cards */
        for(i = 0; i < NUM_LINES_TO_SKIP; i++) 
            std::getline(file, line); // garbage read
        
        /* read the goal deck */
        file >> numCards;         // the number of cards to read
        std::getline(file, line); // garbage read of new line
        for (int i = 0; i < numCards; i++)
        { 
            std::getline(file, line);
            goals.addToDeck(line);
        }

        file.close();
    }
}


int main() {
    Game simulation;
    simulation.readGameStats();

    StringDeck deck;

    std::string testString = "[ ][*][ ][G][ ]";
    std::cout << testString << std::endl;
    deck.playCard("reverse order", testString);
    std::cout << testString << std::endl;

    std::cout << "this is a test" << std::endl;
}