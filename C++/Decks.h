#pragma once

#ifndef DECK_H // include guard

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>

class Deck {
    private:
        std::map<std::string, char> map;
        std::vector<char> cards;

        void removeHelper(std::string &current, int numToRemove);

    public:
        void addToDeck(std::string line, char cardType);
        char drawCard();
        void shuffleDeck();
        bool hasCards();
        std::vector<char> getCards() { return cards; }

        std::string getCardName(char cardType); // WARNING: inefficient - avoid use
                                                 // mostly for debugging purposes
  
        void playCard(char type, std::string &current);
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
