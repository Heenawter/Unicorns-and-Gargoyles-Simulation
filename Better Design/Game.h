#pragma once

#ifndef GAME_H

#include <fstream>

class Game
{
private:
    Deck *deck;
    std::vector<Player *> players;
    Player* currentPlayer;

    std::string goalString;
    int winningPlayer;
    int gameDirection;
    int startingPlayer;

    void readGameStats();
    void takeTurn();

    char gameStatus;

public:
    Game(int numPlayers, std::string goal);
    ~Game();

    void gameRound();

    char getGameStatus() { return gameStatus; }
    Deck *getDeck() { return deck; }
    Player *getPlayer(int index) { return players.at(index); }
    std::vector<Player *> getPlayers() { return players; }
    int getWinningPlayer() { return winningPlayer; }
};

#endif