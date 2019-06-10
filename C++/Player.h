#pragma once

#ifndef PLAYER_H

#include <vector>
#include <list>
#include <string>
#include <unordered_set>

struct VectorHash
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
    std::unordered_set<std::vector<char>, VectorHash> allHands;
    int currentHandIndex;
    std::string currentString = "";

    std::string generateString(StringDeck &deck, std::vector<char> hand, std::string current);
    void drawCard(StringDeck &deck);

    void printHand(StringDeck &deck, std::vector<char> hand);
    int stringDistance(const std::string &string1, const std::string &string2);

public:
    ~Player();

    bool takeTurn(StringDeck &deck, std::string goalString);

    void printSize(int numCards);
    void printAll(StringDeck &deck);

};

#endif