#pragma once

#ifndef GAME_H

#include <fstream>

class Game {
    private:
        Table* table;
        std::vector<Player*> players;

        Player* currentWinner;
        int gameDirection;
        int startingPlayer;

        char playerTurn(std::string goalString, int playerNum);

    public:   
        Game(int numPlayers, std::string goal);
        ~Game();
        
        char gameRound(std::string goalString);
};

#endif