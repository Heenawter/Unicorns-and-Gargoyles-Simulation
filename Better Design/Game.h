#pragma once

#ifndef GAME_H

#include <fstream>
#include <string>

#include "Player.h"
#include "Deck.h"
#include "Constants.h"
#include "Cards.h"

class Game
{
private:
    int numPlayers;

    Cards *cardInfo;
    Deck *deck;
    Player* currentPlayer;
    std::vector<Player *> players;

    std::map<char, int> cardCounts;      // <card type, count>
    std::map<char, std::string> cardMap; // <card type, long card name>

    int winningPlayer;
    int gameDirection;
    int startingPlayer;

    void readCards();
    void addToMaps(std::string line, char cardType);

public:
    Game(int numPlayers, std::string goal);
    ~Game();

    void gameRound();

    Player *getPlayer(int index) { return players.at(index); }
    std::vector<Player *> getPlayers() { return players; }
    int getWinningPlayer() { return winningPlayer; }
};

#endif