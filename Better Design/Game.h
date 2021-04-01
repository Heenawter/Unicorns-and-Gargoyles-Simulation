#pragma once

#ifndef GAME_H

#include <fstream>
#include <string>

#include "Debugging.h"
#include "Player.h"
#include "Player_Greedy.h"
#include "Player_Random.h"
#include "Player_Aggressive.h"
#include "Deck.h"
#include "Constants.h"
#include "Cards.h"


class Game
{
private:
    int gameSeed;
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
    int numTurns;

    void readCards();
    void addToMaps(std::string line, char cardType);
    Player* getNextPlayer(Player *current);

    void actionCard_reverse();

public:
    Game(int numPlayers, std::vector<std::string> playerTypes, std::string goal,
         int seed = std::chrono::system_clock::now().time_since_epoch().count());
    ~Game();

    bool gameRound();
    bool gameRound_limitedTime(int* remainingTime_s, int timePerTurn_s);

    Player *getPlayer(int index) { return players.at(index); }
    Player *getCurrentPlayer() { return this->currentPlayer; }
    std::vector<Player *> getPlayers() { return players; }
    int getWinningPlayer() { return winningPlayer; }
    int getDeckSize() { return deck->getDeckSize(); }
    Deck* getDeck() { return this->deck; }
    void printDeck() { std::cout << this->deck->toString() << "\n"; }
    int getSeed() { return this->gameSeed; }
    int getTurnCount() { return this->numTurns; }
};

#endif