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

    void drawNonActionCard_helper();

public:
    Player(Deck *deck, std::string goalString, Cards* cardInfo, int playerNum);
    ~Player();

    void initOtherPlayers(std::vector<Player *> otherPlayers);
    char takeTurn();

    /**** ACTION CARDS ****/
    void action_drawNonActionCard();
    virtual void action_discardCard() = 0;
    virtual void action_springCleaning() = 0;
    virtual void action_poisonUnicorn() = 0;
    virtual void action_stealCard() = 0;

    void stealCard_helper(int cardToSteal, Player* targetPlayer);
    void discardCard(int card);
    void discardUnicorn(int unicornNumber);

    void printHand() { this->hand->printHand(); }
    int getUnicornCount() { return this->hand->getNumUnicorns(); }
    int getHandSize() { return this->hand->getNumCards(); }
    char getCard(int i) { return this->hand->getCard(i); }
    int getPlayerNum() { return playerNum; }
};

#endif