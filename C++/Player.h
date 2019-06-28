#pragma once

#ifndef PLAYER_H

#include <vector>
#include <string>
#include <unordered_set>

class Player
{
private:
    // std::unordered_set<std::vector<char>, vectorHash> allHands;
    std::string currentString;
    std::vector<char> currentHand;
    int numCards;
    int currentDistance;
    std::string generateString(Deck &deck, std::vector<char> hand);

    void printHand(Deck &deck, std::vector<char> hand);
    int stringDistance(const std::string &string1, const std::string &string2);
public:
    ~Player();
    Player();
    char takeTurn(Deck &deck, std::string goalString);
    void commentCard(std::string goalString);
    void discardCard(Deck &deck, std::string goalString);
    char drawCard(Deck &deck, std::string goalString);
    void springCleaning(Deck &deck, std::string goalString);

    int getHandSize() { return numCards; }
    void printCurrentHand(Deck &deck);

    bool winningCondition();
    
    std::pair<int, std::vector<char> > moveCard(Deck &deck, std::string goalString);
    void combinationUtil(std::vector<char> arr, std::vector<char> data,
                         std::vector<std::vector<char> > &allCombinations,
                         int start, int end,
                         int index, int r);
};

#endif