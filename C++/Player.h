#pragma once

#ifndef PLAYER_H

#include <vector>
#include <list>
#include <string>

class Player {    
    private:
        std::list<std::vector<char> > allHands;
        std::vector<std::string> allStrings;

        int currentHandIndex;
        int currentStringIndex;

    public:
        void drawCard(StringDeck &deck);
        void generateString(StringDeck &deck, std::vector<char> &hand, std::string &current);
        void takeTurn(StringDeck &deck);
        void printAll();
        void printHand(StringDeck &deck, std::vector<char> &hand);
};

#endif