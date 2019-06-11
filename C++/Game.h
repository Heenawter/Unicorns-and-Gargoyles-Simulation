#pragma once

#ifndef GAME_H

#include <fstream>

class Game {
    private:
        Deck deck;
        std::vector<Player*> players;

        void readGameStats();

    public:   
        Game(int numPlayers);
        ~Game();

        void generatePlayers(int numPlayers);
        Deck* getDeck();
        Player* getPlayer(int index);
};

#endif