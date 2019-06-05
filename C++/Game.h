#pragma once

#ifndef GAME_H

const short NUM_LINES_TO_SKIP = 10;

class Game {
    private:
        StringDeck deck;
        GoalsDeck goals;
        Player player;

        void readGameStats();

    public:
        void initializeGame();     

        StringDeck getDeck();  
};

#endif