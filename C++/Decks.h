#pragma once

#ifndef DECK_H // include guard

#include <string>
#include <map>
#include <vector>
#include <algorithm>

const short UNICORN = 0, GARGOYLE = 1, REVERSE = 2, ROTATE_R = 3, ROTATE_L = 4, DOUBLE = 5, 
            APPEND_1 = 6, APPEND_2 = 7, APPEND_3 = 8, REMOVE_1 = 9, REMOVE_2 = 10, REMOVE_3 = 11;

class Deck {
    protected:
        std::map<std::string, short> map;
        std::vector<short> cards;

        short getCardType(std::string card);
        std::string getCardName(short cardType); // WARNING: inefficient - avoid use
                                                 // mostly for debugging purposes
        void shuffleDeck();
        virtual void initializeMap() = 0;

    public:
        void addToDeck(std::string line);
        short drawCard();
};

class StringDeck : public Deck {
    private:
        void initializeMap();

    public:
        void playCard(std::string card, std::string &current);
};

class GoalsDeck : public Deck {
    private:
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
