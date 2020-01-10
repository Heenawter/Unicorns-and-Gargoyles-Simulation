
#pragma once

#ifndef PLAYER_AGGRESSIVE_H

#include <string>
#include "Player.h"

class AggressivePlayer : public Player
{
public:
    AggressivePlayer(Deck *deck, std::string goalString, Cards *cardInfo, int playerNum);
    ~AggressivePlayer();

    /**** ACTION CARDS ****/
    void action_discardCard();
    void action_springCleaning();
    void action_poisonUnicorn();
    void action_stealCard();
};

#endif