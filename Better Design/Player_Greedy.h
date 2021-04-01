
#pragma once

#ifndef PLAYER_Greedy_H

#include <string>
#include <tuple>
#include <functional>
#include "Player.h"
#include "GameStates.h"

class GreedyPlayer : public Player
{
private:
    /**** HELPER FUNCTIONS ****/
    int stealFirst(Player *player, std::function<bool(int, int)> func);
    int removeFirstUnicorn(Player *player, std::function<bool(int, int)> func);

public:
    GreedyPlayer(Deck *deck, std::string goalString, Cards *cardInfo, int playerNum, int seed);
    char takeTurn();
    Hand trySwapping();

    /**** ACTION CARDS ****/
    void action_discardCard();
    void action_springCleaning();
    std::tuple<Player *, int> action_poisonUnicorn();
    std::tuple<Player *, int> action_stealCard();
};

#endif