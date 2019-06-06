#pragma once

#ifndef PLAYER_H

#include <vector>
#include <list>
#include <string>
#include <set>


    class Player
{
private:
    std::list<std::vector<char> > allHands;
    // std::set<std::vector<char> > allHands;
    int currentHandIndex;
    std::string currentString;

    std::vector<char> allCards;

public:
    void drawCard(StringDeck &deck);
    std::string generateString(StringDeck &deck, std::vector<char> &hand, std::string current);
    void takeTurn(StringDeck &deck);

    void printSize(int numCards);
    void printAll(StringDeck &deck);
    void printHand(StringDeck &deck, std::vector<char> &hand);

    void generate_permutations(StringDeck &deck);
};

#endif