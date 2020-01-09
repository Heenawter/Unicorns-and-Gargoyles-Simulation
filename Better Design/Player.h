#pragma once

#ifndef PLAYER_H

#include <vector>
#include <string>
#include <unordered_set>

#include "Cards.h"
#include "Hand.h"
#include "Deck.h"

class Player
{
private:
    // std::unordered_set<std::vector<char>, vectorHash> allHands;
    Hand *hand;
    Deck *deck;
    Cards *cardInfo;

    int playerNum;

    char drawCard();
    char handleActionCard(char actionCard);
    void moveCard();

public:
    Player(Deck *deck, std::string goalString, Cards* cardInfo, int playerNum);
    ~Player();

    char takeTurn();

    /**** ACTION CARDS ****/
    void drawNonActionCard();
    virtual void discardCard() = 0;
    virtual void springCleaning(std::string goalString) = 0;
    virtual void poisonUnicorn() = 0;
    virtual void stealCard() = 0;

    // pair<int, int> == pair<targetCard, damageDone>
    std::pair<int, int> unicornToPoison(std::string goalString);
    std::pair<int, int> cardToSteal(std::string goalString);

    void getPoisoned(std::string goalString, int unicornToPoison);
    void getRobbed(std::string goalString, int cardToSteal);    

    int getPlayerNum() { return playerNum; }
    Hand* getHand() { return this->hand; }
};

#endif