#include "Constants.h"
#include "Decks.h"
#include "Player.h"
#include "Game.h"

// #include <iostream>

Game::Game(int numPlayers)
{
    readGameStats();
    deck.shuffleDeck();

    for (int i = 0; i < numPlayers; i++)
        players.push_back(new Player());
}

Game::~Game() {
    std::vector<Player*>::iterator it;
    for(it = players.begin(); it != players.end(); it++) {
        delete *it;
    }
}

void Game::readGameStats()
{
    std::ifstream file;
    file.open(INPUT_FILE);

    if(file.is_open()) {
        std::string line;
        int numCards;
        int i;
        char cardCounter = 0;

        /* read the string deck */
        std::getline(file, line); // garbage read of first line
        file >> numCards; // the number of cards to read
        std::getline(file, line); // garbage read of new line
        for(i = 0; i < numCards - 2; i++)
        { // -2 because I want to ignore comment/uncomment for now
            std::getline(file, line);
            deck.addToDeck(line, cardCounter);
            cardCounter++;
        }

        file.close();
    }
}

Deck* Game::getDeck() {
    return &deck;
}

Player* Game::getPlayer(int index) {
    return players.at(index);
}