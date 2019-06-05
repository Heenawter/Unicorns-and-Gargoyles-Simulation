#pragma once

#ifndef PLAYER_H

#include <vector>
#include <list>
#include <string>

class Player {    
    private:
        std::list<std::vector<short> > allHands;
        std::vector<std::string> allStrings;

        int currentHandIndex;
        int currentStringIndex;

    public:
        void drawCard(StringDeck &deck);
        void generateString(StringDeck &deck);
        void takeTurn(StringDeck &deck);
        
};

#endif