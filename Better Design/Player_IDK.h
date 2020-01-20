
#pragma once

#ifndef PLAYER_Greedy_H

#include <string>
#include <tuple>
#include "Player.h"

class GreedyPlayer : public Player
{
private:
    /**** ACTION CARDS ****/
    void action_discardCard();
    void action_springCleaning();
    std::tuple<Player *, int> action_poisonUnicorn();
    std::tuple<Player *, int> action_stealCard();

    void combinationUtil(Hand hand, Hand tempHand, Hand &bestHand,
                         int start, int end, int index, int r);

public:
    GreedyPlayer(Deck *deck, std::string goalString, Cards *cardInfo, int playerNum);
    char takeTurn();
};

#endif