
#pragma once

#ifndef PLAYER_AGGRESSIVE_H

#include <string>
#include <tuple>
#include "Player.h"

class AggressivePlayer : public Player
{
private:
    /**** ACTION CARDS ****/
    void action_discardCard();
    void action_springCleaning();
    std::tuple<Player *, int> action_poisonUnicorn();
    std::tuple<Player *, int> action_stealCard();

public:
    AggressivePlayer(Deck *deck, std::string goalString, Cards *cardInfo, int playerNum);
    char takeTurn();
};

#endif