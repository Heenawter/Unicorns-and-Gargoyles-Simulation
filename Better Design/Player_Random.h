
#pragma once

#ifndef PLAYER_RANDOM_H

#include <string>
#include <chrono>
#include <random>
#include <tuple>
#include "Player.h"

class TrollPlayer : public Player
{
private:
    std::mt19937 randomGenerator;

    /**** ACTION CARDS ****/
    void action_discardCard();
    void action_springCleaning();
    std::tuple<Player *, int> action_poisonUnicorn();
    std::tuple<Player *, int> action_stealCard();

public:
    TrollPlayer(Deck *deck, std::string goalString, Cards *cardInfo, int playerNum);
    char takeTurn();
};

#endif