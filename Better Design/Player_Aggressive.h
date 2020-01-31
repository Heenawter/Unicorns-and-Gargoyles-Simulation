
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

    struct sortDamageInfo_Unicorns
    {
        bool operator()(const std::tuple<Player *, int, int> &a,
                        const std::tuple<Player *, int, int> &b) const
        {
            // sort by DAMAGE primarily (decreasing)
            if (std::get<2>(a) == std::get<2>(b))
            {
                // sort by DISTANCE secondarily (increasing)
                int distanceA = std::get<0>(a)->getDistance();
                int distanceB = std::get<0>(b)->getDistance();
                if (distanceA == distanceB)
                {
                    // sort by NUMBER OF UNICORNS thirdly (decreasing)
                    int numUnicornsA = std::get<0>(a)->getUnicornCount();
                    int numUnicornsB = std::get<0>(b)->getUnicornCount();
                    return numUnicornsA > numUnicornsB;
                }
                else
                    return distanceA < distanceB;
            }
            else
                return (std::get<2>(a) > std::get<2>(b));
        }
    };

    struct sortDamageInfo
    {
        bool operator()(const std::tuple<Player *, int, int> &a,
                        const std::tuple<Player *, int, int> &b) const
        {
            // sort by DAMAGE primarily (decreasing)
            if (std::get<2>(a) == std::get<2>(b))
            {
                // sort by DISTANCE secondarily (increasing)
                int distanceA = std::get<0>(a)->getDistance();
                int distanceB = std::get<0>(b)->getDistance();
                if (distanceA == distanceB)
                {
                    // sort by NUMBER OF CARDS thirdly (decreasing)
                    int numCardsA = std::get<0>(a)->getHandSize();
                    int numCardsB = std::get<0>(b)->getHandSize();
                    return numCardsA > numCardsB;
                }
                else
                    return distanceA < distanceB;
            }
            else
                return (std::get<2>(a) > std::get<2>(b));
        }
    };

    /**** HELPER FUNCTIONS ****/
    std::vector<std::tuple<Player *, int, int>> findWinningPlayers(
        std::vector<std::tuple<Player *, int, int>> playerInfo);
    std::tuple<Player*, int, int> findTargetCard(Player *targetPlayer, bool unicornsOnly);
    void combinationUtil(Hand hand, Hand tempHand, Hand &bestHand,
                         int start, int end, int index, int r);

public:
    AggressivePlayer(Deck *deck, std::string goalString, Cards *cardInfo, int playerNum);
    char takeTurn();
};

#endif