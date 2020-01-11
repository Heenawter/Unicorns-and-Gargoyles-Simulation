
#pragma once

#ifndef PLAYER_AGGRESSIVE_H

#include <string>
#include <tuple>
#include "Player.h"

class AggressivePlayer : public Player
{
public:
    AggressivePlayer(Deck *deck, std::string goalString, Cards *cardInfo, int playerNum);
    ~AggressivePlayer();
    char takeTurn();

    /**** ACTION CARDS ****/
    void action_discardCard();
    void action_springCleaning();
    std::tuple<Player *, int> action_poisonUnicorn();
    std::tuple<Player *, int> action_stealCard();
};

#endif