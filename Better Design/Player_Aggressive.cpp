
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
              I.E. Minimize the string distance by removing a card */
void AggressivePlayer::action_discardCard()
{
    int numCards = getHandSize();
    if (numCards > 0)
    {
        Hand testHand = Hand(*this->hand); // use copy constructor to make a copy

        int testDistance = 0;
        int bestDistance = MAX_INT;
        int cardToRemove = -1;

        // try to minimize the string distance by removing the card
        DEBUG("Trying to remove... \n");
        for (int i = 0; i < numCards; i++)
        {
            // try removing every single card once
            testHand.removeCard(i);
            testDistance = testHand.getDistance();
            
            DEBUG("- remove card " + std::to_string(i) + ", ")
            DEBUG("distance " + std::to_string(bestDistance) + " -> " + std::to_string(testDistance) + "... ");
            if (testDistance < bestDistance)
            {
                DEBUG("marked for removal... ");

                // removing the card made improved the hand MORE than any other
                bestDistance = testDistance;
                cardToRemove = i;
            }

            DEBUG("\n");
            testHand = Hand(*this->hand); // start over
        }

        DEBUG("\nFinal decision: remove card " + std::to_string(cardToRemove) + "\n");
        
        DEBUG("- Hand Before:     " + this->hand->toString() + "\n");
        DEBUG("- Distance Before: " + std::to_string(this->hand->getDistance()) + "\n");
        this->discardCard(cardToRemove);
        DEBUG("- Hand After:      " + this->hand->toString() + "\n");
        DEBUG("- Distance After:  " + std::to_string(this->hand->getDistance()));


        LOG("Player " + std::to_string(this->playerNum) + " removed card " + std::to_string(cardToRemove) + " ... ");
        TEST("remove " + std::to_string(cardToRemove) + "... ");
    }
}

/*  Function: springCleaning()
    Goal:     Remove cards ONLY if one makes the solution better; otherwise, 
              just keep everything (remember, spring cleaning makes removal
              optional, whereas discardCard action is required)  */
void AggressivePlayer::action_springCleaning()
{
    Hand combinations = Hand(*this->hand);
    Hand bestHand = Hand(*this->hand);
    int numCards = this->hand->getNumCards();

    int cardsRemoved = 0;
    int cardToRemove;
    #ifdef _DEBUGSTATEMENTS
        std::string handBefore = this->hand->toString();
        std::string distanceBefore = std::to_string(this->hand->getDistance());
    #endif

    DEBUG("Trying to remove...\n");
    if(numCards > 0)
    {
        for (int i = numCards; i >= 0; i--)
        {
            // loop through subsets of ALL sizes, including the subset that would be
            // created by removing ZERO cards (i -less than OR EQUAL TO- numCards)
            combinationUtil(*this->hand, combinations, bestHand, 0, numCards - 1, 0, i);
        }
        DEBUG("---> best hand:      " + bestHand.toString() + "\n");
        DEBUG("---> best distance:  " + std::to_string(bestHand.getDistance()) + "\n");

        // now that we know which hand we are aiming for (bestHand), we must
        // actually CREATE that hand by discarding the appropriate cards
        std::vector<int> setDifference = this->hand->setDifference(bestHand);

        for (int i = 0; i < setDifference.size(); i++)
        {
            cardToRemove = setDifference[i] - cardsRemoved;
            LOG("removed card " + std::to_string(cardToRemove) + " ... ");
            TEST("remove " + std::to_string(cardToRemove) + "... ");

            DEBUG("- removed card " + std::to_string(setDifference[i]) + ", ");
            DEBUG("distance " + std::to_string(this->hand->getDistance()) + " -> ");
            this->discardCard(cardToRemove);
            DEBUG(std::to_string(this->hand->getDistance()) + "... \n");

            cardsRemoved++;
        }

        DEBUG("\nFinal decision: removed " + std::to_string(cardsRemoved) + " card(s)\n");
        DEBUG("- Hand Before:     " + handBefore + "\n");
        DEBUG("- Distance Before: " + distanceBefore + "\n");
        DEBUG("- Hand After:      " + this->hand->toString() + "\n");
        DEBUG("- Distance After:  " + std::to_string(this->hand->getDistance()));
    }
}

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
    {
        TEST("no unicorns to remove ... ");
        DEBUG("No unicorns to remove...");
        return std::tuple<Player *, int>(targetPlayer, targetUnicorn);
    }

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
        currentPlayerDamage = findTargetUnicorn(currentPlayer);
        damageInfo.push_back(currentPlayerDamage);
    }

    std::sort(damageInfo.begin(), damageInfo.end(), sortDamageInfo_Unicorns());

    targetPlayer  = std::get<0>(damageInfo[0]);
    targetUnicorn = std::get<1>(damageInfo[0]);

    LOG("targetting player " + std::to_string(targetPlayer->getPlayerNum()) + " and poisoning unicorn " + std::to_string(targetUnicorn) + " ... ");
    TEST("targetting player " + std::to_string(targetPlayer->getPlayerNum()) + " and poisoning unicorn " + std::to_string(targetUnicorn) + " ... ");

    #ifdef _DEBUGSTATEMENTS
        Hand targetHandAfter = Hand(*targetPlayer->getHand());
        targetHandAfter.removeUnicorn(targetUnicorn);
        int targetDistanceAfter = targetHandAfter.getDistance();

        std::cout << "\nFinal damage info:\n";
        for(auto info : damageInfo)
        {
            std::cout << "- Target player " << std::get<0>(info)->getPlayerNum() << " and poison unicorn " << std::get<1>(info);
            std::cout << ", target damage " << std::get<2>(info) << "\n";
        }
    #endif

    DEBUG("\nFinal decision:  target " + std::to_string(targetPlayer->getPlayerNum()) + " and poison unicorn " + std::to_string(targetUnicorn) + " \n");
    DEBUG("- Target Hand Before:     " + targetPlayer->getHand()->toString() + "\n");
    DEBUG("- Target Distance Before: " + std::to_string(targetPlayer->getHand()->getDistance()) + "\n");
    DEBUG("- Target Hand After:      " + targetHandAfter.toString() + "\n");
    DEBUG("- Target Distance After:  " + std::to_string(targetDistanceAfter));

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
    {
        LOG("no cards to steal ... ");
        TEST("no cards to steal... ");
        DEBUG("No cards to steal... ");
        return std::tuple<Player *, int>(targetPlayer, targetCard);
    }

    Player *currentPlayer;

    std::tuple<Player *, int, int, int> currentPlayerInfo;
    std::vector<std::tuple<Player *, int, int, int>> allPlayerInfo;
    
    for (int i = 0; i < numPlayersWithCards; i++)
    {
        currentPlayer = playersWithCards[i];
        currentPlayerInfo = findCardToSteal(currentPlayer);
        allPlayerInfo.push_back(currentPlayerInfo);
    }

    std::sort(allPlayerInfo.begin(), allPlayerInfo.end(), sortCardsToSteal());

    targetPlayer = std::get<0>(allPlayerInfo[0]);
    targetCard = std::get<1>(allPlayerInfo[0]);

    TEST("targetting player " + std::to_string(targetPlayer->getPlayerNum()) + " and stealing card " + std::to_string(targetCard) + " ... ");

    #ifdef _DEBUGSTATEMENTS
        Hand targetHandAfter = Hand(*targetPlayer->getHand());
        char cardRemoved = targetHandAfter.removeCard(targetCard);
        int targetDistanceAfter = targetHandAfter.getDistance();

        Hand ownHandAfter = Hand(*this->hand);
        ownHandAfter.addToHand(cardRemoved);
        int ownDistanceAfter =  ownHandAfter.getDistance();

        std::cout << "\nFinal damage info:\n";
        for(auto info : allPlayerInfo)
        {
            // player, cardToRemove, damage, bestDistance
            std::cout << "- Target player " << std::get<0>(info)->getPlayerNum() << " and steal card " << std::get<1>(info);
            std::cout << ", target damage " << std::get<2>(info) << ", best distance " << std::get<3>(info) << "\n";
        }
    #endif

    DEBUG("\nFinal decision:  target " + std::to_string(targetPlayer->getPlayerNum()) + " and steal card " + std::to_string(targetCard) + " \n");
    DEBUG("- Target Hand Before:     " + targetPlayer->getHand()->toString() + "\n");
    DEBUG("- Target Distance Before: " + std::to_string(targetPlayer->getHand()->getDistance()) + "\n");
    DEBUG("- Target Hand After:      " + targetHandAfter.toString() + "\n");
    DEBUG("- Target Distance After:  " + std::to_string(targetDistanceAfter) + "\n");
    DEBUG("- Own Hand Before:        " + this->hand->toString() + "\n");
    DEBUG("- Own Distance Before:    " + std::to_string(this->hand->getDistance()) + "\n");
    DEBUG("- Own Hand After:         " + ownHandAfter.toString() + "\n");
    DEBUG("- Own Distance After:     " + std::to_string(ownDistanceAfter));

    return std::tuple<Player *, int>(targetPlayer, targetCard);
}

/*  Function: findTargetUnicorn()
    Goal:     For the given player, find the unicorn that would do the
              most damage when removed (i.e. removing the unicorn makes 
              that player's hand as bad as possible); if no unicorn 
              makes a difference, then simply target the first unicorn
              that makes no difference.
              This function returns a tuple as follows: 
                    tuple<Player*, targetUnicornIndex, damageDone> */
std::tuple<Player *, int, int> AggressivePlayer::findTargetUnicorn(Player *targetPlayer)
{
    std::tuple<Player *, int, int> damageInfo;

    Hand testHand = Hand(*targetPlayer->getHand());
    int cardToRemove = -1;
    
    // std::cout << "Target player " << targetPlayer->getPlayerNum() << ": ";
    // std::cout << "before: " << targetPlayer->getDistance() << " ... ";

    int unicornCount = 0;
    int currentDistance = targetPlayer->getDistance(), testDistance;
    int maxDamage = -MAX_INT, testDamage;
    bool found = false;

    int numCards = targetPlayer->getUnicornCount();

    for (int i = 1; i < numCards + 1; i++)
    {
        testHand.removeUnicorn(i);
        testDistance = testHand.getDistance();
        testDamage = testDistance - currentDistance;
        // std::cout << testDistance << " ... ";

        DEBUG("- targetting player " + std::to_string(targetPlayer->getPlayerNum()) + " and poisoning unicorn " + std::to_string(i));
        DEBUG(", distance " + std::to_string(currentDistance) + " -> " + std::to_string(testDistance));
        DEBUG(", damage " + std::to_string(testDamage) + "... ");
        if (testDamage > maxDamage)
        {
            // because > and not >=, always target FIRST unicorn encountered
            // unless you find a better one - ties result in FIRST not LAST
            cardToRemove = i;
            maxDamage = testDamage;
            found = true;
            DEBUG("found potential target");
        }

        DEBUG("\n");

        testHand = Hand(*targetPlayer->getHand());
    }

    // std::cout << "max: " << maxDamage << " ... ";
    // std::cout << "after: " << currentDistance + maxDamage << " ... ";
    int damage = maxDamage;
    if (!found)
    {
        // no unicorn causes outright damage, so just target the first
        // std::cout << "here!" << " ... ";

        testHand = Hand(*targetPlayer->getHand());
        testHand.removeUnicorn(1);   
        cardToRemove = 1;
        testDistance = testHand.getDistance();
        damage = testDistance - targetPlayer->getDistance();
    }

    damageInfo = std::make_tuple(targetPlayer, cardToRemove, damage);

    return damageInfo;
}


std::tuple<Player *, int, int, int> AggressivePlayer::findCardToSteal(Player *targetPlayer)
{
    std::vector<std::tuple<int, int, int>> allCardInfo;
    std::tuple<int, int, int> currentCardInfo;

    Hand targetHandCopy = Hand(*targetPlayer->getHand());
    Hand currentHandCopy = Hand(*this->getHand());
    int numCards = targetPlayer->getHandSize();
    char cardRemoved;
    DEBUG("- targetting player " + std::to_string(targetPlayer->getPlayerNum()) + "... \n");
    for (int i = 0; i < numCards; i++)
    {
        cardRemoved = targetHandCopy.removeCard(i);
        currentHandCopy.addToHand(cardRemoved);

        currentCardInfo = std::make_tuple(i, targetHandCopy.getDistance(), currentHandCopy.getDistance());
        allCardInfo.push_back(currentCardInfo);

        targetHandCopy = Hand(*targetPlayer->getHand());
        currentHandCopy = Hand(*this->getHand());
    }

    std::sort(allCardInfo.begin(), allCardInfo.end(), sortCardInfo());
    #ifdef _DEBUGSTATEMENTS
        for(auto info : allCardInfo)
        {
            std::cout << "-- stealing card " << std::get<0>(info);
            std::cout << ", own distance " << this->hand->getDistance() << " -> " << std::get<2>(info);
            std::cout << ", target distance " << targetPlayer->getHand()->getDistance() << " -> " << std::get<1>(info);
            
            int test = std::get<1>(info) - targetPlayer->getDistance();
            std::cout << ", damage " << test << std::endl;
        }
    #endif

    currentCardInfo = allCardInfo[0]; // get the front card after sorting
    int cardToRemove = std::get<0>(currentCardInfo);
    int damage = std::get<1>(currentCardInfo) - targetPlayer->getDistance();
    int bestDistance = std::get<2>(currentCardInfo);

    std::tuple<Player *, int, int, int> finalDecision;
    finalDecision = std::make_tuple(targetPlayer, cardToRemove, damage, bestDistance);

    return finalDecision;
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
        // std::cout << newHand.toString() << ", distance: " << newHand.getDistance() << std::endl;

        if (newHand.getDistance() <= bestHand.getDistance())
        {
            // because <= and not <, this implies we are being AGRESSIVE i.e. the player is
            // choosing to remove AS MANY CARDS AS POSSIBLE while still maintaining the best distance
            // (remember that we are going from LARGEST subsets to SMALLEST when calling)
            bestHand = newHand;

        }
        // DEBUG(".");
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

AggressivePlayer::AggressivePlayer(Deck *deck, std::string goalString, Cards *cardInfo, int playerNum, int seed)
    : Player(deck, goalString, cardInfo, playerNum, seed)
{
    this->type = "aggressive";
}

/*  Function: takeTurn()
    Goal:     If you have less than 2 cards, simply draw a card; otherwise,
              if swapping will improve your current hand, then do that;
              for swapping, find the BEST POSSIBLE swap that will improve
              your hand by the largest possible margin;
              if swapping does not help, draw a new card */
char AggressivePlayer::takeTurn()
{
    if(this->getHandSize() >= 20)
    {
        throw(TooManyCardsException());
    }

    LOG(" Take turn ... ");
    char card = ' ';

    if (this->getHandSize() > 1)
    {
        Hand bestSwap = trySwapping();
        if (bestSwap.getDistance() < this->hand->getDistance())
        {
            // if swapping made the hand better, then make it the new hand
            LOG("Re-arrange cards ... ");
            delete this->hand;
            this->hand = new Hand(bestSwap);
        }
        else
        {
            LOG("draw ... ")
            card = drawCard();
        }
    }
    else
    {
        LOG("can only draw ... ")
        card = drawCard();
    }

    return card;
}

Hand AggressivePlayer::trySwapping()
{
    Hand bestHand = Hand(*this->hand);
    Hand testHand = Hand(*this->hand);
    int bestDistance = this->hand->getDistance(), testDistance;
    int cardToMove, whereToMove;

    #ifdef _DEBUGSTATEMENTS
        int actualCard, actualMove;
        bool foundCardToSwap;
    #endif

    #ifdef _TESTSTATEMENTS
        bool foundCardToSwap;
    #endif

    int numCards = this->getHandSize();

    DEBUG("Trying to move...\n");
    for (cardToMove = 0; cardToMove < numCards; cardToMove++)
    {
        // remove the card so we can insert it somewhere else
        for (whereToMove = 0; whereToMove < numCards; whereToMove++)
        {
            // obviously, inserting a card in the exact position it already is
            // is pointless; so just skip the entire thing when this happens
            if (whereToMove != cardToMove)
            {
                testHand.moveCard(cardToMove, whereToMove);
                testDistance = testHand.getDistance();
                DEBUG("- move " + std::to_string(cardToMove) + " to " + std::to_string(whereToMove) + ", ");
                DEBUG("distance " + std::to_string(bestDistance) + " -> " + std::to_string(testDistance) + "... ");
                if (testDistance < bestDistance)
                {
                    bestHand = Hand(testHand);
                    bestDistance = testDistance;
                    LOG("move " + std::to_string(cardToMove) + " to " + std::to_string(whereToMove) + " ... ");
                    TEST("move " + std::to_string(cardToMove) + " to " + std::to_string(whereToMove) + "... ");
                    DEBUG("found a possible move... ");

                    #ifdef _TESTSTATEMENTS
                        foundCardToSwap = true;
                    #endif

                    #ifdef _DEBUGSTATEMENTS
                        foundCardToSwap = true;
                        actualCard = cardToMove;
                        actualMove = whereToMove;
                    #endif
                }
                else
                {
                    testHand = Hand(*this->hand);
                }
                DEBUG("\n");
            }
        }
    }

    #ifdef _TESTSTATEMENTS
        if(!foundCardToSwap)
            std::cout << "swapping does not improve hand";
    #endif

    #ifdef _DEBUGSTATEMENTS
        if(!foundCardToSwap)
            std::cout << "Final decision: swapping does not improve hand";
        else {
            DEBUG("\nFinal decision: move " + std::to_string(actualCard) + " to " + std::to_string(actualMove) + " \n");

            DEBUG("- Hand Before:     " + this->hand->toString() + "\n");
            DEBUG("- Distance Before: " + std::to_string(this->hand->getDistance()) + "\n");
            DEBUG("- Hand After:      " + bestHand.toString() + "\n");
            DEBUG("- Distance After:  " + std::to_string(bestHand.getDistance()));
        }
    #endif

    // std::cout << "after distance: " << bestHand.getDistance() << std::endl;

    // return std::make_pair(bestDistance, bestHand);
    return bestHand;
}