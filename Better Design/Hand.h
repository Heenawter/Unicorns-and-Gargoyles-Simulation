#pragma once

#ifndef HAND_H // include guard

#include "Cards.h"
#include "Constants.h"

#include <string>
#include <vector>
#include <iostream>

class Hand
{
private:
    std::vector<char> cards;
    int numCards;
    std::string currentString;
    std::string goalString;

    int currentDistance;

    Cards* cardInfo;
    
    void combinationUtil(std::vector<char> hand, std::vector<char> tempHand,
                         std::vector<char> &bestHand, int &bestDistance,
                         std::string goalString,
                         int start, int end,
                         int index, int r);
    int stringDistance(const std::string &string1, const std::string &string2);
    std::string generateString();
    std::string generateString(std::vector<char> cards); // used in combinationUtil

public:
    Hand(std::string goalString, Cards *cardInfo);
    Hand(const Hand &oldHand); // copy constructor
    std::string getCardName(char type);

    bool operator < (const Hand &oldHand);
    std::string toString();

    void addToHand(char card);
    void moveCard(int oldIndex, int newIndex);
    char removeCard(int i);
    char getCard(int i) { return this->cards[i]; }

    int getNumCards() { return this->numCards; }
    int getNumUnicorns();
    std::string getCurrentString() { return this->currentString; }
    void removeUnicorn(int unicornNumber);
};

#endif
