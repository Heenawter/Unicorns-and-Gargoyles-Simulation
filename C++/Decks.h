#pragma once

#include <iostream> 
#include <fstream> 
#include <string>  
#include <algorithm>
#include <map>
#include <vector> 
#include <chrono> // need std::chrono::system_clock for randomness

#ifndef DECK_H // include guard

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
        short drawCard();
};

class StringDeck : public Deck {
    private:
        void initializeMap();

    public:
        void playCard(std::string card, std::string &current);
};

// class GoalsDeck : public Deck {
    

// };

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
