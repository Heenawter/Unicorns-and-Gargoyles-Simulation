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

    /* HELPER FUNCTIONS */
    void combinationUtil(std::vector<char> hand, std::vector<char> tempHand,
                         std::vector<char> &bestHand, int &bestDistance,
                         Deck &deck, std::string goalString,
                         int start, int end,
                         int index, int r);
    std::string generateString(Deck &deck, std::vector<char> hand);
    void printHand(Deck &deck, std::vector<char> hand);
    int stringDistance(const std::string &string1, const std::string &string2);

public:

    ~Player();
    Player();

    /* GAME FUNCTIONS */
    char takeTurn(Deck &deck, std::string goalString);
    char drawCard(Deck &deck, std::string goalString);
    std::pair<int, std::vector<char>> moveCard(Deck &deck, std::string goalString);
    bool winningCondition();

    /* ACTION CARDS */
    void discardCard(Deck &deck, std::string goalString);
    void springCleaning(Deck &deck, std::string goalString);
    void poisonCard(Deck &deck, std::string goalString, std::vector<Player*> &otherPlayers);
    void stealCard(Deck &deck, std::string goalString, std::vector<Player*> &otherPlayers);

    /* HELPER FUNCTIONS */ 
    int getHandSize() { return numCards; }
    int getDistance() { return currentDistance; }
    std::vector<char> getHand() { return currentHand; }
    void printCurrentHand(Deck &deck);
    
};

#endif