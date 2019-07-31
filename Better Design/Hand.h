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

public:
    Hand(std::string goalString, Cards *cardInfo);
    std::string getCardName(char type);
    std::string generateString();

    bool operator < (Hand* h2);
    void addToHand(char card, std::string goalString);
};

#endif
