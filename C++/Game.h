#pragma once

#ifndef GAME_H

#include <fstream>

class Game {
    private:
        Deck deck;
        std::vector<Player*> players;
        std::string goalString;
        int winningPlayer;

        void readGameStats();

    public:   
        Game(int numPlayers, std::string goal);
        ~Game();

        void generatePlayers(int numPlayers);
        Deck* getDeck();
        Player* getPlayer(int index);

        char gameRound(std::string goalString);
        
        int getWinningPlayer() { return winningPlayer; }
};

#endif