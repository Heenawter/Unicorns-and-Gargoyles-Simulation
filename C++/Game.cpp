#include "Decks.h"
#include "Player.h"
#include "Game.h"
#include <fstream>
// #include <iostream>

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

void Game::initializeGame() {
    deck.initializeMap();
    readGameStats();
    deck.shuffleDeck();
}

StringDeck Game::getDeck() { return deck; }