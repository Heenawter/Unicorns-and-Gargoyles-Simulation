#pragma once

#ifndef DECK_H // include guard

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>

class Deck {
    protected:
        std::map<std::string, char> map;
        std::vector<char> cards;

        char getCardType(std::string card);
        
        virtual void initializeMap() = 0;

    public:
        void addToDeck(std::string line);
        char drawCard();
        void shuffleDeck();
        std::vector<char> getCards(){ return cards; }

        std::string getCardName(char cardType); // WARNING: inefficient - avoid use
                                                 // mostly for debugging purposes
};

class StringDeck : public Deck
{
        public:
            void playCard(char type, std::string &current);
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
