#pragma once

#ifndef DECK_H // include guard

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

public:
    Deck(std::map<char, int> cardCounts);
    ~Deck();

    void shuffleDeck();
    char drawNextCard();
    char drawNonActionCard();
    void discardCard(char card);
};

#endif
