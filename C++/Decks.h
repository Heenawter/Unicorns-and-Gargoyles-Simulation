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

    public:
        void addToDeck(std::string line, char cardType);
        char drawCard();
        void shuffleDeck();
        std::vector<char> getCards(){ return cards; }

        std::string getCardName(char cardType); // WARNING: inefficient - avoid use
                                                 // mostly for debugging purposes
};

class StringDeck : public Deck
{
    private:
        void removeHelper(std::string &current, int numToRemove);

    public:
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
