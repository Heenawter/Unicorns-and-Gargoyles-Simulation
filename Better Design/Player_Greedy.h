
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

    bool removeFirst(std::function<bool(int, int)> func);
    bool removeFirstUnicorn(Player *player, int &unicornNumber, std::function<bool(int, int)> func);

public:
    GreedyPlayer(Deck *deck, std::string goalString, Cards *cardInfo, int playerNum);
    char takeTurn();
};

#endif