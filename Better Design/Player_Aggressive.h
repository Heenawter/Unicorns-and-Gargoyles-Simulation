
#pragma once

#ifndef PLAYER_Aggressive_H

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

    /**** HELPER FUNCTIONS ****/
    std::vector<Player *> findWinningPlayers(std::vector<Player *> players);
    std::tuple<Player*, int, int> findTargetUnicorn(Player *targetPlayer);
    void combinationUtil(Hand hand, Hand tempHand, Hand &bestHand,
                         int start, int end, int index, int r);

public:
    AggressivePlayer(Deck *deck, std::string goalString, Cards *cardInfo, int playerNum);
    char takeTurn();
};

#endif