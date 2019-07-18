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

    Deck* deck;
    Deck* discardDeck;

    void removeCard(std::string goalString, int cardToRemove);

    /* HELPER FUNCTIONS */
    void combinationUtil(std::vector<char> hand, std::vector<char> tempHand,
                         std::vector<char> &bestHand, int &bestDistance,
                         std::string goalString,
                         int start, int end,
                         int index, int r);
public:

    ~Player();
    Player(Deck *deck, Deck *discardDeck);

    /* GAME FUNCTIONS */
    char takeTurn(std::string goalString);
    char drawCard(std::string goalString);
    std::pair<int, std::vector<char>> moveCard(std::string goalString);
    bool winningCondition();

    /* ACTION CARDS */
    void discardCard(std::string goalString);
    void springCleaning(std::string goalString);
    void poisonCard(std::string goalString, std::vector<Player*> &otherPlayers);
    void stealCard(std::string goalString, std::vector<Player*> &otherPlayers);

    void getPoisoned(std::string goalString, int unicornToPoison);
    void getRobbed(std::string goalString, int cardToSteal);


    /* HELPER FUNCTIONS */
    int getHandSize() { return numCards; }
    int getDistance() { return currentDistance; }
    std::vector<char> getHand() { return currentHand; }
    void printCurrentHand();
    void printHand(std::vector<char> hand);


    ////// MOVE THIS SHIT TO DECK DUDE
    std::string generateString(std::vector<char> hand);
    int stringDistance(const std::string &string1, const std::string &string2);
};

#endif