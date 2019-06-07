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
    std::string currentString;

    std::vector<char> allCards;

public:
    void drawCard(StringDeck &deck);
    std::string generateString(StringDeck &deck, std::vector<char> &hand, std::string current);
    void takeTurn(StringDeck &deck);

    void printSize(int numCards);
    void printAll(StringDeck &deck);
    void printHand(StringDeck &deck, std::vector<char> hand);

    void generate_permutations(StringDeck &deck);
};

#endif