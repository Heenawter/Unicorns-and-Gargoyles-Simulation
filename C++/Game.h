#pragma once

#ifndef GAME_H

const char NUM_LINES_TO_SKIP = 10;

class Game {
    private:
        StringDeck deck;
        GoalsDeck goals;
        std::vector<Player*> players;

        void readGameStats();

    public:   
        Game(int numPlayers);
        ~Game();

        void generatePlayers(int numPlayers);
        StringDeck* getDeck();
        Player* getPlayer(int index);
};

#endif