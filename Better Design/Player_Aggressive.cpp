
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

/*  Function: poisonUnicorn()
    Goal:     First, target a player:
                    - If another player is winning (i.e. their hand is 
                      closer to the goal than anyone elses'), then target
                      the winning player; if there is a tie for winning, 
                      target the player such that removing the unicorn 
                      does the most damage; if this is also a tie, then
                      remove from the player with the most unicorns
              Then, find which unicorn to target:
                    - Find the unicorn that does the most damage and
                      remove it; if there is a tie, then remove
                      from the FIRST unicorn that does the max
                      damage (since it is harder to get a unicorn
                      at the start of the string than the end) */
std::tuple<Player *, int> AggressivePlayer::action_poisonUnicorn()
{
    std::vector<std::tuple<int, int> > damageInfo;
    Player *targetPlayer = NULL;
    int targetUnicorn = -1;
    std::cout << "Here!" << std::endl;
    
    // we only care about players with unicorns, so get them first
    std::vector<Player*> playersWithUnicorns = this->getPlayersWithUnicorns();
    if(playersWithUnicorns.size() > 0)
    {
        // start by searching for the winning players
        std::vector<Player *> winners = findWinningPlayers(playersWithUnicorns);
        int numWinners = winners.size();
        std::cout << "number of winning players w/ unicorns: " << numWinners << std::endl;

        // std::vector< std::tuple<Player*, int unicorn, int damage> >

        // now, try to find the way to maximize the damage; that is,
        // try removing each unicorn from each winning player and target
        // the player+unicorn pair that does the most damage

        // start by generating all of the worst damages possible
        Player *currentPlayer;
        std::tuple <int, int> currentPlayerDamage;
        for (int i = 0; i < numWinners; i++)
        {
            currentPlayer = winners[i];
            std::cout << "Player " << currentPlayer->getPlayerNum() << ": ";
            std::cout << "before: " << currentPlayer->getDistance() << " ... ";

            currentPlayerDamage = findTargetUnicorn(currentPlayer);
            std::cout << "target unicorn: " << std::get<0>(currentPlayerDamage) << " ... ";
            std::cout << "damage: " << std::get<1>(currentPlayerDamage) << std::endl;

            damageInfo.push_back(currentPlayerDamage);
        }
        
        int testDamage;
        int maxDamage = 0;
        for(int i = 0; i < numWinners; i++)
        {
            testDamage = std::get<1>(damageInfo[i]);
            if(testDamage > maxDamage)
            {
                maxDamage = testDamage;
            }
        }
        std::cout << "max damage: " << maxDamage << std::endl;
    } 
    else
    {
        std::cout << "no player has a unicorn..." << std::endl;
    }
    
    if(targetPlayer != NULL)
    {
        std::cout << "Targetting Player " << targetPlayer->getPlayerNum();
        std::cout << " and unicorn " << targetUnicorn << std::endl;
    }
    return std::tuple<Player *, int>(targetPlayer, targetUnicorn);
}

std::tuple<int, int> AggressivePlayer::findTargetUnicorn(Player *targetPlayer)
{
    std::tuple<int, int> damageInfo;

    Hand testHand = Hand(*targetPlayer->getHand());
    int cardToRemove = -1;
    int numCards = targetPlayer->getHandSize();
    int testDistance, worstDistance = testHand.getDistance();

    int unicornCount = 0;
    for(int i = 0; i < numCards; i++)
    {
        if(testHand.getCard(i) == UNICORN)
        {
            unicornCount++;
            testHand.removeCard(i);
            testDistance = testHand.getDistance();
            if (testDistance > worstDistance)
            {
                worstDistance = testDistance;
                cardToRemove = unicornCount;
            }
            testHand = Hand(*targetPlayer->getHand());
        }
    }

    std::cout << "after: " << worstDistance << " ... ";

    int damage = worstDistance - targetPlayer->getDistance();
    if (cardToRemove == -1)
    {
        // no unicorn causes outright damage, so just remove the first
        cardToRemove = 1; 
    }

    damageInfo = std::make_tuple(cardToRemove, damage);

    return damageInfo;
}

/*  Function: stealCard()
    Goal:     */
std::tuple<Player *, int> AggressivePlayer::action_stealCard()
{
    return std::tuple<Player *, int>(NULL, 1);
}

/*  Function: findWinningPlayers()
    Goal:     Return the player(s) with the smallest distance from
              the goal; that is, find the player(s) that are closest
              to winning the game and return them */
std::vector<Player *> AggressivePlayer::findWinningPlayers(std::vector<Player*> players)
{
    int numPlayers = players.size();
    int winningDistance = MAX_INT;
    int testDistance;
    for (int i = 0; i < numPlayers; i++)
    {
        testDistance = players[i]->getDistance();
        if (testDistance < winningDistance)
            winningDistance = testDistance;
    }
    std::cout << "winning distance: " << winningDistance << std::endl;

    std::vector<Player *> winningPlayers;
    Player *currentPlayer;
    for (int i = 0; i < numPlayers; i++)
    {
        currentPlayer = players[i];
        if (currentPlayer->getDistance() == winningDistance)
            winningPlayers.push_back(currentPlayer);
    }

    return winningPlayers;
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