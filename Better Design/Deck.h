#pragma once

#ifndef DECK_H // include guard

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <random>

class Deck
{
private:
    std::vector<Card> cards;
    std::vector<Card> discard;

public:
    void addToDeck(std::string line, char cardType);

    void shuffleDeck();
    char drawCard();
    void discardCard(char card);
};

#endif
