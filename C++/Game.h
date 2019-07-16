#pragma once

#ifndef GAME_H

#include <fstream>

class Game {
    private:
        Deck deck;
        std::vector<Player*> players;
        std::string goalString;
        int winningPlayer;
        int gameDirection;
        int startingPlayer;

        void readGameStats();
        char playerTurn(std::string goalString, int playerNum);

    public:   
        Game(int numPlayers, std::string goal);
        ~Game();
        
        char gameRound(std::string goalString);

        Deck* getDeck() { return &deck; }
        Player* getPlayer(int index) { return players.at(index); }
        std::vector<Player*> getPlayers() { return players; }
        int getWinningPlayer() { return winningPlayer; }
};

#endif