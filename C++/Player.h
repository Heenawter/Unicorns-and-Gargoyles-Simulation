#pragma once

#ifndef PLAYER_H

#include <vector>
#include <string>
#include <unordered_set>


struct vectorHash
{
    size_t operator()(const std::vector<char> &v) const
    {
        std::hash<int> hasher;
        size_t seed = 0;
        for (int i : v)
        {
            seed ^= hasher(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

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
    char drawCard(Deck &deck, std::string goalString);
public:
    ~Player();
    Player();
    char takeTurn(Deck &deck, std::string goalString);
    void commentCard(std::string goalString);
    void discardCard(Deck &deck, std::string goalString); 

    int getHandSize() { return numCards; }

    bool winningCondition();
    std::pair<int, std::vector<char> > moveCard(Deck &deck, std::string goalString);
};

#endif