
#include "Player_Aggressive.h"


/**************************************************/
/*               Private Functions                */
/**************************************************/

/******************************* ACTION CARDS ******************************* /

/*  Function: discardCard()
    Goal:     Based on the goal, try to remove the card that gets you CLOSER;
              if there is no card that makes your hand bettter when removed,
              then simply remove a card that does the LEAST DAMAGE. 
                   - Best case: Removal causes an IMPROVEMENT
                   - Worst case: Removal either causes no damage OR 
                                 removal does the least damage possible
              For finding best case card, if there is a tie,
                    - remove the first one encountered
              For finding the worst case card, if there is a tie,
                    - remove the last one encountered */
void AggressivePlayer::action_discardCard()
{
    Hand testHand = Hand(*this->hand); // use copy constructor to make a copy

    int testDistance = 0;
    int bestDistance = this->hand->getDistance();
    int worstDistance = MAX_INT;
    int cardToRemove_bestCase = -1;
    int cardToRemove_worstCase = -1;

    // first, see if any card will IMPROVE the current hand....
    for (int i = 0; i < this->hand->getNumCards(); i++)
    {
        // try removing every single card once
        testHand.removeCard(i);
        testDistance = testHand.getDistance();
        if (testDistance < bestDistance) 
        {
            // removing the card either made the hand BETTER or kept it the same
            bestDistance = testDistance;
            cardToRemove_bestCase = i;
        } 

        if(testDistance <= worstDistance)
        {
            // removing the card either made the hand WORSE or kept it the same
            worstDistance = testDistance;
            cardToRemove_worstCase = i;
        }
    
        testHand = Hand(*this->hand); // start over
    }

    std::cout << "Previous hand: " << this->hand->toString() << std::endl;
    std::cout << "Current distance: " << this->hand->getDistance() << std::endl;

    if (cardToRemove_bestCase != -1) {
        // there is a card that makes the hand BETTER, so remove that one
        this->discardCard(cardToRemove_bestCase);
        std::cout << "Removal of card " << cardToRemove_bestCase << " made hand better..." << std::endl;
    } else {
        // no removal improves the hand, so remove the card that has the least impact
        this->discardCard(cardToRemove_worstCase);
        std::cout << "Removal of card " << cardToRemove_worstCase << " was the best option..." << std::endl;
    }

    std::cout << "New hand: " << this->hand->toString() << std::endl;
    std::cout << "New distance: " << this->hand->getDistance() << std::endl;
}

/*  Function: springCleaning()
    Goal:     Remove cards ONLY if one makes the solution better; otherwise, 
              just keep everything (remember, spring cleaning makes removal
              optional, whereas discardCard action is required)  */
void AggressivePlayer::action_springCleaning()
{
    Hand combinations = Hand(*this->hand);
    Hand bestHand = Hand(*this->hand);

    std::cout << "Starting Distance: " << this->hand->getDistance() << std::endl;

    int numCards = this->hand->getNumCards();
    for (int i = 1; i <= numCards; i++)
    {
        // loop through subsets of ALL sizes, including the subset that would be
        // created by removing ZERO cards (i -less than OR EQUAL TO- numCards)
        combinationUtil(*this->hand, combinations, bestHand, 0, numCards - 1, 0, i);
    }
    std::cout << "Best Distance: " << bestHand.getDistance() << std::endl;

    // now that we know which hand we are aiming for (bestHand), we must
    // actually CREATE that hand by discarding the appropriate cards
    std::vector<int> setDifference = this->hand->setDifference(bestHand);
    std::cout << this->hand->toString() << " ---> " << bestHand.toString() << std::endl;
    // delete this->hand;
    // this->hand = new Hand(bestHand);

    // std::cout << this->deck->toString() << std::endl;
    for(int i = 0; i < setDifference.size(); i++)
    {
        // std::cout << this->cardInfo->getCardName(setDifference[i]);
        std::cout << "-- discard "<< this->cardInfo->getCardName(this->hand->getCard(setDifference[i]));
        std::cout << ", " << std::to_string(setDifference[i]) << std::endl;
        this->discardCard(setDifference[i]);

        // adjust next indeces, since hand is now 1 smaller
        for(int j = i + 1; j < setDifference.size(); j++)
        {
            setDifference[j]--;
        }
    }

    std::cout << "After: " << this->hand->toString() << std::endl;

}

// bool sortDamageInfo(const std::tuple<Player *, int, int> &a, const std::tuple<Player *, int, int> &b)
// {
//     // sort by DAMAGE primarily (decreasing)
//     if(std::get<2>(a) == std::get<2>(b)) 
//     {
//         // sort by DISTANCE secondarily (increasing)
//         int distanceA = std::get<0>(a)->getDistance();
//         int distanceB = std::get<0>(b)->getDistance();
//         if(distanceA == distanceB)
//         {
//             // sort by NUMBER OF UNICORNS thirdly (decreasing)
//             int numUnicornsA = std::get<0>(a)->getUnicornCount();
//             int numUnicornsB = std::get<0>(b)->getUnicornCount();
//             return numUnicornsA > numUnicornsB;
//         }
//         return distanceA < distanceB;
//     }
//     else 
//     {
//         return (std::get<2>(a) > std::get<2>(b));
//     }
// }

/*  Function: poisonUnicorn()
    Goal:     Try to maximize the damage by removing a unicorn; that is,
              target the player + unicorn such that removal of that unicorn
              causes the MOST damage out of all other unicorns on the table. 
              If there is a tie for the amount of damage, 
                    - target the player that is CLOSEST to winning;
                      that is, target the player with the smallest 
                      distance from the goal string
              If there is a tie for winners as well,
                    - target the player with the most unicorns
              If the number of unicorns is also a tie,
                    - target the player with the most cards 
              If this is ALSO a tie,
                    - target the first player */
std::tuple<Player *, int> AggressivePlayer::action_poisonUnicorn()
{
    Player *targetPlayer = NULL;
    int targetUnicorn = -1;
    
    // we only care about players with unicorns, so get them first
    std::vector<Player*> playersWithUnicorns = this->getPlayersWithUnicorns();
    int numPlayersWithUnicorns = playersWithUnicorns.size();
    if(numPlayersWithUnicorns == 0)
        return std::tuple<Player *, int>(targetPlayer, targetUnicorn);

    // now, try to find the way to maximize the damage; that is,
    // try removing each unicorn from eac player and target the
    // player+unicorn pair that does the most damage

    // start by generating all of the worst damages possible
    Player *currentPlayer;
    std::tuple <Player*, int, int> currentPlayerDamage;
    std::vector<std::tuple<Player *, int, int>> damageInfo;
    for (int i = 0; i < numPlayersWithUnicorns; i++)
    {
        currentPlayer = playersWithUnicorns[i];
        // std::cout << "Player " << currentPlayer->getPlayerNum() << ": ";
        // std::cout << "before: " << currentPlayer->getDistance() << " ... ";

        currentPlayerDamage = findTargetCard(currentPlayer, true);
        // std::cout << "target unicorn: " << std::get<1>(currentPlayerDamage) << " ... ";
        // std::cout << "damage: " << std::get<2>(currentPlayerDamage) << std::endl;

        damageInfo.push_back(currentPlayerDamage);
    }

    std::sort(damageInfo.begin(), damageInfo.end(), sortDamageInfo_Unicorns());
    for(auto val : damageInfo)
    {
        std::cout << "Player " << std::get<0>(val)->getPlayerNum() << ": ";
        std::cout << "damage: " << std::get<2>(val) << " ... ";
        std::cout << "distance: " << std::get<0>(val)->getDistance() << " ... ";
        std::cout << "numUnicorns: " << std::get<0>(val)->getUnicornCount() << std::endl;
    }

    targetPlayer  = std::get<0>(damageInfo[0]);
    targetUnicorn = std::get<1>(damageInfo[0]);
    std::cout << "Targetting Player " << targetPlayer->getPlayerNum();
    std::cout << " and unicorn " << targetUnicorn << std::endl;
    
    return std::tuple<Player *, int>(targetPlayer, targetUnicorn);
}

/*  Function: stealCard()
    Goal:     Steal the card that will cause the most damage; that is,
              go through every card try to remove it; target the 
              player+card pair that does the MOST damage. If there is a 
              tie for the amount of damage, then target the winning
              player; otherwise, target the player with the most cards;
              If there is no winner and no player with more cards
              than the rest of the potential targets, then simply
              target the first player and the first card. */
std::tuple<Player *, int> AggressivePlayer::action_stealCard()
{
    Player *targetPlayer = NULL;
    int targetCard = -1;

    // we only care about players with unicorns, so get them first
    std::vector<Player *> playersWithCards = getPlayersWithCards();
    int numPlayersWithCards = playersWithCards.size();
    if (numPlayersWithCards == 0)
        return std::tuple<Player *, int>(targetPlayer, targetCard);

    Player *currentPlayer;
    std::tuple<Player *, int, int> currentPlayerDamage;
    std::vector<std::tuple<Player *, int, int>> damageInfo;
    for (int i = 0; i < numPlayersWithCards; i++)
    {
        currentPlayer = playersWithCards[i];
        // std::cout << "Player " << currentPlayer->getPlayerNum() << ": ";
        // std::cout << "before: " << currentPlayer->getDistance() << " ... ";

        currentPlayerDamage = findTargetCard(currentPlayer, false);
        // std::cout << "target unicorn: " << std::get<1>(currentPlayerDamage) << " ... ";
        // std::cout << "damage: " << std::get<2>(currentPlayerDamage) << std::endl;

        damageInfo.push_back(currentPlayerDamage);
    }

    std::sort(damageInfo.begin(), damageInfo.end(), sortDamageInfo());
    for (auto val : damageInfo)
    {
        std::cout << "Player " << std::get<0>(val)->getPlayerNum() << ": ";
        std::cout << "damage: " << std::get<2>(val) << " ... ";
        std::cout << "distance: " << std::get<0>(val)->getDistance() << " ... ";
        std::cout << "numCards: " << std::get<0>(val)->getHandSize() << std::endl;
    }

    targetPlayer = std::get<0>(damageInfo[0]);
    targetCard = std::get<1>(damageInfo[0]);
    std::cout << "Targetting Player " << targetPlayer->getPlayerNum();
    std::cout << " and card " << targetCard << std::endl;

    return std::tuple<Player *, int>(targetPlayer, targetCard);
}

/*  Function: findWinningPlayers()
    Goal:     Return the player(s) with the smallest distance from
              the goal; that is, find the player(s) that are closest
              to winning the game and return them */
std::vector<std::tuple<Player *, int, int>> AggressivePlayer::
    findWinningPlayers(std::vector<std::tuple<Player *, int, int>> playerInfo)
{
    int numPlayers = playerInfo.size();
    int winningDistance = MAX_INT;
    int testDistance;
    Player* currentPlayer;
    for (int i = 0; i < numPlayers; i++)
    {
        currentPlayer = std::get<0>(playerInfo[i]);
        testDistance = currentPlayer->getDistance();
        if (testDistance < winningDistance)
            winningDistance = testDistance;
    }
    std::cout << "winning distance: " << winningDistance << std::endl;

    std::vector<std::tuple<Player *, int, int>> winningInfo;
    std::tuple<Player *, int, int> currentInfo;
    for (int i = 0; i < numPlayers; i++)
    {
        currentPlayer = std::get<0>(playerInfo[i]);
        if (currentPlayer->getDistance() == winningDistance)
            winningInfo.push_back(playerInfo[i]);
    }

    return winningInfo;
}

/*  Function: findTargetUnicorn()
    Goal:     For the given player, find the unicorn that would do the
              most damage when removed (i.e. removing the unicorn makes 
              that player's hand as bad as possible); if no unicorn 
              makes a difference, then simply target the first unicorn
              that makes no difference.
              This function returns a tuple as follows: 
                    tuple<Player*, targetUnicornIndex, damageDone> */
std::tuple<Player *, int, int> AggressivePlayer::findTargetCard(Player *targetPlayer, bool unicornsOnly)
{
    std::tuple<Player *, int, int> damageInfo;

    Hand testHand = Hand(*targetPlayer->getHand());
    int cardToRemove = -1;
    
    std::cout << "Target player " << targetPlayer->getPlayerNum() << ": ";
    std::cout << "before: " << targetPlayer->getDistance() << " ... ";

    int unicornCount = 0;
    int currentDistance = targetPlayer->getDistance(), testDistance;
    int maxDamage = -MAX_INT, testDamage;
    bool found = false;

    int numCards = targetPlayer->getHandSize();
    if(unicornsOnly)
        numCards = targetPlayer->getUnicornCount();

    for (int i = 0; i < numCards; i++)
    {
        if(unicornsOnly)
            testHand.removeUnicorn(i + 1);
        else
            testHand.removeCard(i);
        testDistance = testHand.getDistance();
        testDamage = testDistance - currentDistance;
        std::cout << testDistance << " ... ";
        if (testDamage > maxDamage)
        {
            // because > and not >=, always target FIRST unicorn encountered
            // unless you find a better one - ties result in FIRST not LAST
            cardToRemove = i;
            if (unicornsOnly)
                cardToRemove++;
            maxDamage = testDamage;
            found = true;
        }

        testHand = Hand(*targetPlayer->getHand());
    }

    std::cout << "max: " << maxDamage << " ... ";
    std::cout << "after: " << currentDistance + maxDamage << " ... ";
    int damage = maxDamage;
    if (!found)
    {
        // no unicorn causes outright damage, so just target the first
        std::cout << "here!" << " ... ";
        testHand = Hand(*targetPlayer->getHand());
        testHand.removeUnicorn(1);
        testDistance = testHand.getDistance();
        damage = testDistance - targetPlayer->getDistance();
        cardToRemove = 0;
        if (unicornsOnly)
            cardToRemove++;
    }

    damageInfo = std::make_tuple(targetPlayer, cardToRemove, damage);
    std::cout << "damage: " << damage << " ... " << std::endl;

    return damageInfo;
}

/*  Function: combinationUtil()
    Goal:     This function is used to build all of the possible subsets 
              of length r (WITHOUT REORDERING CARDS) of "hand" parameter
              and finds the hand that is closest to the goal string.
              This is a helper function for springCleaning */
// https://www.geeksforgeeks.org/print-all-possible-combinations-of-r-elements-in-a-given-array-of-size-n/
void AggressivePlayer::combinationUtil(Hand hand, Hand tempHand, Hand &bestHand, 
                             int start, int end, int index, int r)
{
    // base case
    if (index == r)
    {
        Hand newHand = Hand(tempHand, r);
        std::cout << "... " << newHand.getDistance() << std::endl;

        if (newHand <= bestHand)
        {
            // because <= and not <, this implies we are being CONSERVATIVE i.e. the player is
            // choosing to remove AS FEW CARDS AS POSSIBLE while still maintaining the best distance
            bestHand = newHand;
        }

        return;
    }

    // replace index with all possible elements. The condition "end-i+1 >= r-index"
    // makes sure that including one element at index will make a combination with
    // remaining elements at remaining positions
    for (int i = start; i <= end && end - i + 1 >= r - index; i++)
    {
        tempHand.changeCard(index, hand.getCard(i));
        combinationUtil(hand, tempHand, bestHand, i + 1, end, index + 1, r);
    }
}

/**************************************************/
/*                Public Functions                */
/**************************************************/

AggressivePlayer::AggressivePlayer(Deck *deck, std::string goalString, Cards *cardInfo, int playerNum)
    : Player(deck, goalString, cardInfo, playerNum)
{
    
}

char AggressivePlayer::takeTurn()
{
    // action_discardCard();
    // action_springCleaning();
    action_poisonUnicorn();
    return 'n';
}