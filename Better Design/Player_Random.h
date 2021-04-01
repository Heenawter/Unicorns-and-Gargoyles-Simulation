
#pragma once

#ifndef PLAYER_RANDOM_H

#include <string>
#include <chrono>
#include <random>
#include <tuple>
#include <unistd.h>
#include "Player.h"
#include "GameStates.h"

class TrollPlayer : public Player
{
private:
    int findRandomCard();

public:
    TrollPlayer(Deck *deck, std::string goalString, Cards *cardInfo, int playerNum, int seed);
    ~TrollPlayer();
    char takeTurn();
    Hand trySwapping();

    /**** ACTION CARDS ****/
    void action_discardCard();
    void action_springCleaning();
    std::tuple<Player *, int> action_poisonUnicorn();
    std::tuple<Player *, int> action_stealCard();
};

#endif