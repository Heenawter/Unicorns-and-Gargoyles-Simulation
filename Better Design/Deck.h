#pragma once

#ifndef DECK_H // include guard

#include "Cards.h"

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <random>
#include <chrono> // need std::chrono::system_clock for randomness

class Deck
{
private:
    std::vector<char> cards;
    std::vector<char> discard;

    Cards* cardInfo;

    bool hasNonActionCard(std::vector<char> deck);

public:
    Deck(std::map<char, int> cardCounts, Cards *cardInfo);

    void shuffleDeck();
    char drawNextCard();
    char drawNonActionCard();
    void discardCard(char card);

    bool hasNonActionCard();
    std::string getCardName(char card);
};

#endif
