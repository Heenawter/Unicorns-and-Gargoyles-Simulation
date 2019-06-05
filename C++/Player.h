#pragma once

#ifndef PLAYER_H

#include <vector>
#include <list>
#include <string>
#include <set>

class Player {    
    private:
        std::list<std::vector<char> > allHands;
        std::set<std::string> allStrings;

        int currentHandIndex;
        std::string currentString;

    public:
        void drawCard(StringDeck &deck);
        std::string generateString(StringDeck &deck, std::vector<char> &hand, std::string current);
        void takeTurn(StringDeck &deck);
        void printAll(StringDeck &deck);
        void printHand(StringDeck &deck, std::vector<char> &hand);
};

#endif