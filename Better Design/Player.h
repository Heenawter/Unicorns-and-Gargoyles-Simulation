#pragma once

#ifndef PLAYER_H

#include <vector>
#include <string>
#include <unordered_set>

#include "Debugging.h"
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
    void handleActionCard(char type);

    void action_drawNonActionCard_helper();
    void action_discardCard_helper();
    void action_springCleaning_helper();
    // void action_poisonUnicorn_helper();
    // void action_stealCard_helper();

    /**** ACTION CARDS ****/
    // void action_drawNonActionCard();
    virtual void action_discardCard() = 0;
    virtual void action_springCleaning() = 0;
    virtual std::tuple<Player *, int> action_poisonUnicorn() = 0;
    virtual std::tuple<Player *, int> action_stealCard() = 0;

public:
    Player(Deck *deck, std::string goalString, Cards* cardInfo, int playerNum);
    ~Player();

    void initOtherPlayers(std::vector<Player *> otherPlayers);
    virtual char takeTurn() = 0;
    bool matchesGoal();

    std::string toString();

    void action_drawNonActionCard();    // MAKE ME PRIVATE WHEN NOT TESTING
    void action_poisonUnicorn_helper(); // MAKE ME PRIVATE WHEN NOT TESTING
    void action_stealCard_helper();     // MAKE ME PRIVATE WHEN NOT TESTING


    int getUnicornCount() { return this->hand->getNumUnicorns(); }
    int getHandSize() { return this->hand->getNumCards(); }
    char getCard(int i) { return this->hand->getCard(i); }
    int getPlayerNum() { return playerNum; }
    int getDistance() { return this->hand->getDistance(); }
    Hand* getHand() { return this->hand; }

    std::vector<Player *> getPlayersWithUnicorns();
    std::vector<Player *> getPlayersWithCards();
    void discardCard(int card);
    void moveCard(int oldIndex, int newIndex);
};

#endif