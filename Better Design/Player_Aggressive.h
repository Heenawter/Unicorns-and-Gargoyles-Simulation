
#pragma once

#ifndef PLAYER_Aggressive_H

#include <string>
#include <tuple>
#include "Player.h"
#include "GameStates.h"

class AggressivePlayer : public Player
{
private:
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

    struct sortCardInfo
    {
        // const std::tuple<index_of_card_to_remove_from_target_player,
        //                  target_player's_distance_after_removal,
        //                  distance_after_card_appended_to_own_hand>
        bool operator()(const std::tuple<int, int, int> &a,
                        const std::tuple<int, int, int> &b) const
        {
            const int TARGET_DISTANCE = 1;
            const int OWN_DISTANCE = 2;
            // sort to MINIMIZE OWN DISTANCE primarily (increasing)
            if (std::get<OWN_DISTANCE>(a) == std::get<OWN_DISTANCE>(b))
            {
                // sort to MAXIMIZE distance of target player (decreasing)
                return (std::get<TARGET_DISTANCE>(a) > std::get<TARGET_DISTANCE>(b));
            }
            else
                return (std::get<OWN_DISTANCE>(a) < std::get<OWN_DISTANCE>(b));
        }
    };

    struct sortCardsToSteal
    {
        // const std::tuple<target_player,
        //                  index_of_card_to_remove_from_target_player,
        //                  damage_done_to_target_player_after_removal,
        //                  distance_after_card_appended_to_own_hand>
        bool operator()(const std::tuple<Player *, int, int, int> &a,
                        const std::tuple<Player *, int, int, int> &b) const
        {
            const int TARGET_DAMAGE = 2;
            const int OWN_DISTANCE = 3;
            // primary: sort to MINIMIZE OWN DISTANCE (increasing)
            if (std::get<OWN_DISTANCE>(a) == std::get<OWN_DISTANCE>(b))
            {
                // secondary: sort by MAXIMIZE DAMAGE done to target player (decreasing)
                if (std::get<TARGET_DAMAGE>(a) == std::get<TARGET_DAMAGE>(b))
                {
                    // tertiary: sort by DISTANCE to target players closest to winning (increasing)
                    int distanceA = std::get<0>(a)->getDistance();
                    int distanceB = std::get<0>(b)->getDistance();
                    return distanceA < distanceB;
                }
                else
                    return std::get<TARGET_DAMAGE>(a) > std::get<TARGET_DAMAGE>(b);
            }
            else
                return (std::get<OWN_DISTANCE>(a) < std::get<OWN_DISTANCE>(b));
        }
    };


    /**** HELPER FUNCTIONS ****/
    std::tuple<Player*, int, int> findTargetUnicorn(Player *targetPlayer);
    std::tuple<Player *, int, int, int> findCardToSteal(Player *targetPlayer);
    void combinationUtil(Hand hand, Hand tempHand, Hand &bestHand,
                         int start, int end, int index, int r);
public:
    AggressivePlayer(Deck *deck, std::string goalString, Cards *cardInfo, int playerNum, int seed);
    char takeTurn();
    Hand trySwapping();

    /**** ACTION CARDS ****/
    void action_discardCard();
    void action_springCleaning();
    std::tuple<Player *, int> action_poisonUnicorn();
    std::tuple<Player *, int> action_stealCard();
};

#endif