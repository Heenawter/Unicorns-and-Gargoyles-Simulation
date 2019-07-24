#pragma once

#ifndef CARD_H // include guard

#include <string>
#include <map>
#include <vector>
#include <algorithm>

class Cards
{
private:
    std::map<std::string, char> map;
    
    
public:
    void addToDeck(std::string line, char cardType);

    void shuffleDeck();
    char drawCard();
    void discardCard(char card);

    bool hasCards();
    std::vector<char> getCards() { return cards; }
    int numberOfCards() { return cards.size(); }
    std::string getCardName(char cardType); // WARNING: inefficient - avoid use
                                            // mostly for debugging purposes
    bool hasNonActionCard();

    void playCard(char type, std::string &current);
};

#endif
