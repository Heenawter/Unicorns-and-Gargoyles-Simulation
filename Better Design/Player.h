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
protected:
    // std::unordered_set<std::vector<char>, vectorHash> allHands;
    Hand *hand;
    Deck *deck;
    Cards *cardInfo;
    std::vector<Player*> otherPlayers;

    int playerNum;

    char drawCard();
    char handleActionCard(char actionCard);
    void moveCard();

public:
    Player(Deck *deck, std::string goalString, Cards* cardInfo, int playerNum);
    ~Player();

    void initOtherPlayers(std::vector<Player *> otherPlayers);
    char takeTurn();

    /**** ACTION CARDS ****/
    void drawNonActionCard();
    virtual void discardCard() = 0;
    virtual void springCleaning() = 0;
    virtual void poisonUnicorn() = 0;
    virtual void stealCard() = 0;

    int getPlayerNum() { return playerNum; }
    Hand* getHand() { return this->hand; }
};

#endif