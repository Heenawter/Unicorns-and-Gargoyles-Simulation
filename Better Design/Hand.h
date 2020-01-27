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
    Hand(const Hand &oldHand);  // copy constructor
    Hand(Hand oldHand, int r); // copy cards from 0 to r

    std::string getCardName(char type);

    bool operator < (const Hand &otherHand);
    bool operator <= (const Hand &otherHand);
    bool operator == (const Hand &otherHand);
    std::string toString();

    void addToHand(char card);
    void moveCard(int oldIndex, int newIndex);
    char removeCard(int i);
    int getNumUnicorns();
    void removeUnicorn(int unicornNumber);

    void changeCard(int i, char card) { this->cards[i] = card; }
    char getCard(int i) { return this->cards[i]; }
    std::vector<char> getCards() { return this->cards; }
    int getNumCards() { return this->numCards; }
    int getDistance() { return this->currentDistance; }
    std::string getCurrentString() { return this->currentString; }
    std::string getGoalString() { return this->goalString; }

    std::vector<int> setDifference(Hand otherHand);
};

#endif
