#pragma once

#ifndef GAME_H

#include <fstream>
#include <string>

#include "Player.h"
#include "Table.h"
#include "Constants.h"
#include "Cards.h"

class Game
{
private:
    int numPlayers;
    std::vector<Player *> players;
    Table* table;
    Player* currentPlayer;
    Cards* cardInfo;

    std::map<char, int> cardCounts;      // <card type, count>
    std::map<char, std::string> cardMap; // <card type, long card name>

    int winningPlayer;
    int gameDirection;
    int startingPlayer;

    char gameStatus;

    void readCards();
    void addToMaps(std::string line, char cardType);

    void takeTurn();

public:
    Game(int numPlayers, std::string goal);
    ~Game();

    void gameRound();

    char getGameStatus() { return gameStatus; }
    Player *getPlayer(int index) { return players.at(index); }
    std::vector<Player *> getPlayers() { return players; }
    int getWinningPlayer() { return winningPlayer; }
};

#endif