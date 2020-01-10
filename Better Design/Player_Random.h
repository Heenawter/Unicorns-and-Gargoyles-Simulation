
#pragma once

#ifndef PLAYER_RANDOM_H

#include <string>
#include <chrono>
#include <random>
#include "Player.h"

class TrollPlayer : public Player
{
private:
    std::mt19937 randomGenerator;

public:
    TrollPlayer(Deck *deck, std::string goalString, Cards *cardInfo, int playerNum);
    ~TrollPlayer();

    /**** ACTION CARDS ****/
    void action_discardCard();
    void action_springCleaning();
    void action_poisonUnicorn();
    void action_stealCard();
};

#endif