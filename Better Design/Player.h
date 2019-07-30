#pragma once

#ifndef PLAYER_H

#include <vector>
#include <string>
#include <unordered_set>

#include "Cards.h"
#include "Hand.h"
#include "Table.h"

class Player
{
private:
    // std::unordered_set<std::vector<char>, vectorHash> allHands;
    Hand *hand;
    Table *table;

public:
    Player(Table *table, Cards *cardInfo);
    ~Player();

    /**** GAME FUNCTIONS ****/
    void appendToHand();

    std::pair<int, std::vector<char>> moveCard(std::string goalString);
    bool winningCondition();

    /**** ACTION CARDS ****/
    void discardCard(std::string goalString);
    void springCleaning(std::string goalString);
    void drawNonAction();

    // pair<int, int> == pair<targetCard, damageDone>
    std::pair<int, int> unicornToPoison(std::string goalString);
    std::pair<int, int> cardToSteal(std::string goalString);

    void getPoisoned(std::string goalString, int unicornToPoison);
    void getRobbed(std::string goalString, int cardToSteal);    
};

#endif