#pragma once

#ifndef DECK_H // include guard

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>

class Deck {
    protected:
        std::map<std::string, short> map;
        std::vector<short> cards;

        short getCardType(std::string card);
        std::string getCardName(short cardType); // WARNING: inefficient - avoid use
                                                 // mostly for debugging purposes
        virtual void initializeMap() = 0;

    public:
        void addToDeck(std::string line);
        short drawCard();
        void shuffleDeck();
        std::vector<short> getCards(){ return cards; }
};

class StringDeck : public Deck
{
        public:
            void playCard(std::string card, std::string &current);
            void initializeMap();
};

class GoalsDeck : public Deck {
    public:
        void initializeMap();
};

#endif

// int main() {
//     Deck deck;
//     // deck.readGameStats();
//     std::string testString = "[ ][*][ ][G][ ]";
//     std::cout << testString << std::endl;
//     deck.playCard("reverse order", testString);
//     std::cout << testString << std::endl;

//     deck.drawCard();
// }
