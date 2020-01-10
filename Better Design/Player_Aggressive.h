
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
    void discardCard();
    void springCleaning();
    void poisonUnicorn();
    void stealCard();
};

#endif