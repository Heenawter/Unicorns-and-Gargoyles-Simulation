
#include "Player_Greedy.h"

/**************************************************/
/*               Private Functions                */
/**************************************************/

/******************************* ACTION CARDS ******************************* /

/*  Function: discardCard()
    Goal:     Remove the first card that makes your solution better; if no card
              IMPROVES your hand, then remove one that at least doesn't make it
              any worse; otherwise, if no such card exists, just remove the
              first card in your hand regardless of its impact */
void GreedyPlayer::action_discardCard()
{
    int numCards = getHandSize();
    if (numCards > 0)
    {
        int cardToRemove = MAX_INT;
        // first, try removing a card that makes the hand better
        // bool removed = removeFirst(this, cardToRemove, std::less<int>());

        int testDistance;
        Hand testHand = Hand(*this->hand); // use copy constructor to make a copy
        int currentDistance = this->hand->getDistance();
        bool removed = false;

        DEBUG("Trying to remove... \n");
        for (int i = 0; i < numCards && !removed; i++)
        {
            testHand.removeCard(i);
            testDistance = testHand.getDistance();

            DEBUG("- remove card " + std::to_string(i) + ", ");
            DEBUG("distance " + std::to_string(currentDistance) + " -> " + std::to_string(testDistance) + "... ");
            if (testDistance <= currentDistance) // 
            {
                DEBUG("found card to remove... ");
                removed = true;
                cardToRemove = i;
            }
            else
            {
                testHand = Hand(*this->hand);
            }
            DEBUG("\n");
        }

        if (!removed)
        {
            // then, just remove the last card regardless of the consequences
            DEBUG("Found no valid card to remove... Remove last card.\n");
            TEST("swapping only makes hand worse")
            cardToRemove = numCards - 1;
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
    Goal:     Remove a card; if the solution gets better, continue with 
              that hand and try removing another card from the start of
              the new tableau. NO BACKTRACKING */
void GreedyPlayer::action_springCleaning()
{
    Hand testHand = Hand(*this->hand); // use copy constructor to make a copy
    int testDistance;
    int currentDistance = this->hand->getDistance();

    #ifdef _DEBUGSTATEMENTS
        std::string handBefore = this->hand->toString();
        std::string distanceBefore = std::to_string(this->hand->getDistance());
        int cardsRemoved = 0;
    #endif

    #ifdef _TESTSTATEMENTS
        int cardsRemoved = 0;
    #endif

    DEBUG("Trying to remove...\n")
    for (int i = 0; i < this->hand->getNumCards(); i++)
    {
        DEBUG("- remove card " + std::to_string(i) + ", ");

        testHand.removeCard(i);
        testDistance = testHand.getDistance();

        DEBUG("distance " + std::to_string(currentDistance) + " -> " + std::to_string(testDistance) + "... ");

        if (testDistance < currentDistance)
        {
            LOG("removed card " + std::to_string(i) + " ... ");
            TEST("remove " + std::to_string(i) + "... ");
            DEBUG("remove this card... \n");
            DEBUG("---> old hand: " + this->hand->toString() + "\n");
            this->discardCard(i);
            DEBUG("---> new hand: " + this->hand->toString());

            currentDistance = this->hand->getDistance();
            i = -1;
            #ifdef _DEBUGSTATEMENTS
                cardsRemoved++;
            #endif
            #ifdef _TESTSTATEMENTS
                cardsRemoved++;
            #endif
        }
        testHand = Hand(*this->hand);
        DEBUG("\n");
    }

    #ifdef _TESTSTATEMENTS
        if(cardsRemoved <= 0) 
            std::cout << "no cards to remove";
    #endif

    DEBUG("\nFinal decision: removed " + std::to_string(cardsRemoved) + " card(s)\n");
    DEBUG("- Hand Before:     " + handBefore + "\n");
    DEBUG("- Distance Before: " + distanceBefore + "\n");
    DEBUG("- Hand After:      " + this->hand->toString() + "\n");
    DEBUG("- Distance After:  " + std::to_string(this->hand->getDistance()));
}

/*  Function: poisonUnicorn()
    Goal:     Target the player and unicorn such that, when the unicorn
              is removed, that player's solution gets worse; in other words,
                    for each player, 
                        for each unicorn in their hand,
                            if removal of that unicorn makes the player's
                            hand worse (i.e. makes their distance larger),
                            then target that player + that unicorn 
                            and exit both loops 
              If there are no unicorns to poison, simply do nothing
              If no removal makes the solutions worse, try >= rather than >
              If removal only makes the solutions BETTER, target the first
              player and the first unicorn */
std::tuple<Player *, int> GreedyPlayer::action_poisonUnicorn()
{
    bool removed = false;
    int numOtherPlayers = this->otherPlayers.size();
    Player *targetPlayer = NULL;
    int targetUnicorn = -1;
    std::vector<Player *> playersWithUnicorns = this->getPlayersWithUnicorns();
    numOtherPlayers = playersWithUnicorns.size();

    if (numOtherPlayers == 0)
    {
        TEST("no unicorns to poison... ");
        DEBUG("No unicorns to poison... ");
        return std::tuple<Player *, int>(targetPlayer, targetUnicorn);
    }

    std::function<bool(int, int)> cases[2] = {std::greater<int>(), std::greater_equal<int>()};

    DEBUG("Trying to poison...\n");
    for (int currentCase = 0; currentCase < 2 && !removed; currentCase++)
    {
        // case 0: check if removing a unicorn can make the hand WORSE
        // case 1: check if removing a unicorn won't make a difference
        // case 2: removing a unicorn ONLY makes every hand better
        for (int i = 0; i < numOtherPlayers && !removed; i++)
        {
            // for each player, go through their hand and try removing...
            targetPlayer = playersWithUnicorns[i];
            DEBUG("- targetting player " + std::to_string(targetPlayer->getPlayerNum()) + "... \n");
            targetUnicorn =  removeFirstUnicorn(targetPlayer, cases[currentCase]);

            if (targetUnicorn > 0)
                removed = true;
        }
    }

    if (removed) // case 0 or 1
    {
        LOG("targetting player " + std::to_string(targetPlayer->getPlayerNum()) + " and poisoning unicorn " + std::to_string(targetUnicorn) + " ... ");
        TEST("targetting player " + std::to_string(targetPlayer->getPlayerNum()) + " and poisoning unicorn " + std::to_string(targetUnicorn) + " ... ");
    }
    else  // case 2
    {
        std::uniform_int_distribution<int> playerDistribution(0, numOtherPlayers - 1);
        targetPlayer = playersWithUnicorns[playerDistribution(this->randomGenerator)];

        // then target the first unicorn
        targetUnicorn = 1;
        LOG("targetting random player " + std::to_string(targetPlayer->getPlayerNum()) + " and poisoning unicorn " + std::to_string(targetUnicorn) + " ... ");
        TEST("targetting random player " + std::to_string(targetPlayer->getPlayerNum()) + " and poisoning unicorn " + std::to_string(targetUnicorn) + " ... ");
    }
    
    #ifdef _DEBUGSTATEMENTS
        Hand targetHandAfter = Hand(*targetPlayer->getHand());
        targetHandAfter.removeUnicorn(targetUnicorn);
        int targetDistanceAfter = targetHandAfter.getDistance();
    #endif

    DEBUG("\nFinal decision:  target " + std::to_string(targetPlayer->getPlayerNum()) + " and poison unicorn " + std::to_string(targetUnicorn) + " \n");
    DEBUG("- Target Hand Before:     " + targetPlayer->getHand()->toString() + "\n");
    DEBUG("- Target Distance Before: " + std::to_string(targetPlayer->getHand()->getDistance()) + "\n");
    DEBUG("- Target Hand After:      " + targetHandAfter.toString() + "\n");
    DEBUG("- Target Distance After:  " + std::to_string(targetDistanceAfter));

    return std::tuple<Player *, int>(targetPlayer, targetUnicorn);
}

/*  Function: stealCard()
    Goal:     Go through the hands of every player and steal the first card you find
              that will improve your current hand; that is, for each player, go 
              through their cards one by one; once you find a card that will
              improve your hand, quit. 
              If no cards will improve your hand, take one that won't make it WORSE
              Otherwise, if no such cards exist, just steal the first card from the 
              first other player */
std::tuple<Player *, int> GreedyPlayer::action_stealCard()
{
    Player* targetPlayer = NULL;
    int targetCard = 0;
    int numOtherPlayers = this->otherPlayers.size();

    std::vector<Player *> playersWithCards;
    for (int i = 0; i < numOtherPlayers; i++)
    {
        if (this->otherPlayers[i]->getHandSize() != 0)
            playersWithCards.push_back(this->otherPlayers[i]);
    }
    numOtherPlayers = playersWithCards.size();

    if(numOtherPlayers == 0)
    {
        LOG("no cards to steal ... ");
        TEST("no cards to steal... ");
        DEBUG("No cards to steal... ");
        return std::tuple<Player *, int>(targetPlayer, targetCard);
    }


    bool stolen = false;
    std::function<bool(int, int)> cases[2] = {std::less<int>(), std::less_equal<int>()};
    DEBUG("Trying to steal...\n");
    for (int currentCase = 0; currentCase < 2 && !stolen; currentCase++)
    {
        // case 0: check if stealing the card will IMPROVE your hand
        // case 1: check if stealing tha card won't make a difference
        // case 2: stealing a card only makes you hand worse
        for (int i = 0; i < numOtherPlayers && !stolen; i++)
        {
            // for each player, go through their hand and try stealing...
            targetPlayer = playersWithCards[i];
            DEBUG("- targetting player " + std::to_string(targetPlayer->getPlayerNum()) + "... \n");
            targetCard = stealFirst(targetPlayer, cases[currentCase]);

            if (targetCard >= 0)
                stolen = true;
        }
    }

    if (stolen) // case 0 or 1
    {
        LOG("targetting player " + std::to_string(targetPlayer->getPlayerNum()) + " and stealing card " + std::to_string(targetCard) + " ... ");
        TEST("targetting player " + std::to_string(targetPlayer->getPlayerNum()) + " and stealing card " + std::to_string(targetCard) + " ... ");
    }
    else // case 2
    {
        // target a random player that has cards
        std::uniform_int_distribution<int> playerDistribution(0, numOtherPlayers - 1);
        targetPlayer = playersWithCards[playerDistribution(this->randomGenerator)];

        // then target the last card
        targetCard = targetPlayer->getHandSize() - 1;
        LOG("targetting random player " + std::to_string(targetPlayer->getPlayerNum()) + " and stealing card " + std::to_string(targetCard) + " ... ");
        TEST("targetting random player " + std::to_string(targetPlayer->getPlayerNum()) + " and stealing card " + std::to_string(targetCard) + " ... ");
    }

    #ifdef _DEBUGSTATEMENTS
        Hand targetHandAfter = Hand(*targetPlayer->getHand());
        char cardRemoved = targetHandAfter.removeCard(targetCard);
        int targetDistanceAfter = targetHandAfter.getDistance();

        Hand ownHandAfter = Hand(*this->hand);
        ownHandAfter.addToHand(cardRemoved);
        int ownDistanceAfter =  ownHandAfter.getDistance();
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

int GreedyPlayer::stealFirst(Player *player, std::function<bool(int, int)> func)
{
    Hand testHand = Hand(*this->hand); // use copy constructor to make a copy
    int testDistance = this->hand->getDistance();
    char testCard;
    int currentDistance = testDistance, cardToSteal = -1;
    bool removed = false;

    for (int i = 0; i < player->getHandSize() && !removed; i++)
    {
        DEBUG("-- stealing card " + std::to_string(i) + ", ");
        testCard = player->getCard(i);
        testHand.addToHand(testCard);
        testDistance = testHand.getDistance();

        DEBUG("own distance " + std::to_string(currentDistance) + " -> " + std::to_string(testDistance) + "... ");
        if (func(testDistance, currentDistance)) // either <, <=, >, >=, or ==
        {
            DEBUG("found potential target... ");
            removed = true;
            cardToSteal = i;
        }
        else
        {
            testHand = Hand(*this->hand);
        }
        DEBUG("\n");
    }

    return cardToSteal;
}

/*  Function: removeFirstUnicorn()
    Goal:     Remove the first unicorn such that func(testDistance, currentDistance)
              returns true; this function expects <, <=, ==, >=, > for func; 
              for example, if func = > = std::greater,
                    - find and remove the first unicorn such that, when it is 
                      removed, it makes the hand WORSE (i.e. farther from 
                      the goal string) */
int GreedyPlayer::removeFirstUnicorn(Player* player, std::function<bool(int, int)> func)
{
    bool removed = false;

    Hand* currentHand = player->getHand();
    int currentDistance = currentHand->getDistance();

    Hand testHand = Hand(*currentHand);
    int testDistance;
    int unicornToRemove = -1;

    for (int i = 1; i < currentHand->getNumUnicorns() + 1 && !removed; i++)
    {
        DEBUG("-- poison unicorn " + std::to_string(i) + ", ");

        testHand = Hand(*currentHand);
        testHand.removeUnicorn(i);
        testDistance = testHand.getDistance();

        DEBUG("target distance " + std::to_string(currentDistance) + " -> " + std::to_string(testDistance) + "... ");

        if (func(testDistance, currentDistance))
        {
            DEBUG("found potential target... ");
            removed = true;
            unicornToRemove = i;
        }
        DEBUG("\n");
        
    }
    return unicornToRemove;
}

/**************************************************/
/*                Public Functions                */
/**************************************************/

GreedyPlayer::GreedyPlayer(Deck *deck, std::string goalString, Cards *cardInfo, int playerNum, int seed)
: Player(deck, goalString, cardInfo, playerNum, seed)
{
    this->type = "greedy";
}

/*  Function: takeTurn()
    Goal:     If you have less than 2 cards, simply draw a card; otherwise,
              if swapping will improve your current hand, then do that;
              for swapping, simply choose the first possible swap that will
              improve your hand, even if a better swap down the line exists;
              if swapping does not help, draw a new card */
char GreedyPlayer::takeTurn()
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
        if(bestSwap.getDistance() < this->hand->getDistance())
        {
            // if swapping made the hand better, then make it the new hand
            LOG("Re-arrange cards ... ");
            delete this->hand;
            this->hand = new Hand(bestSwap);
        } else {
            LOG("draw ... ")
            card = drawCard();
        }
    }
    else
    {
        card = drawCard();
    }

    return card;
}

Hand GreedyPlayer::trySwapping()
{
    Hand bestHand = Hand(*this->hand);
    Hand testHand = Hand(*this->hand);
    int currentDistance = this->hand->getDistance(), testDistance;
    int cardToMove, whereToMove;

    int numCards = this->getHandSize();
    bool swapped = false;
    DEBUG("Trying to move...\n");
    
    #ifdef _DEBUGSTATEMENTS
        int actualCard, actualMove;
    #endif

    #ifdef _TESTSTATEMENTS
        bool foundCardToSwap;
    #endif

    for (cardToMove = 0; cardToMove < numCards && !swapped; cardToMove++)
    {
        // remove the card so we can insert it somewhere else
        for (whereToMove = 0; whereToMove < numCards && !swapped; whereToMove++)
        {
            // obviously, inserting a card in the exact position it already is
            // is pointless; so just skip the entire thing when this happens
            if (whereToMove != cardToMove)
            {
                testHand.moveCard(cardToMove, whereToMove);
                testDistance = testHand.getDistance();
                DEBUG("- move " + std::to_string(cardToMove) + " to " + std::to_string(whereToMove) + ", ");
                DEBUG("distance " + std::to_string(currentDistance) + " -> " + std::to_string(testDistance) + "... ");
                if (testDistance < currentDistance)
                {
                    swapped = true;
                    bestHand = Hand(testHand);
                    LOG("move " + std::to_string(cardToMove) + " to " + std::to_string(whereToMove) + " ... ");
                    TEST("move " + std::to_string(cardToMove) + " to " + std::to_string(whereToMove) + "... ");
                    DEBUG("found a possible move... ");
                    
                    #ifdef _TESTSTATEMENTS
                        foundCardToSwap = true;
                    #endif

                    #ifdef _DEBUGSTATEMENTS
                        actualCard = cardToMove;
                        actualMove = whereToMove;
                    #endif

                } else {
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

    DEBUG("\nFinal decision: move " + std::to_string(actualCard) + " to " + std::to_string(actualMove) + " \n");

    DEBUG("- Hand Before:     " + this->hand->toString() + "\n");
    DEBUG("- Distance Before: " + std::to_string(this->hand->getDistance()) + "\n");
    DEBUG("- Hand After:      " + bestHand.toString() + "\n");
    DEBUG("- Distance After:  " + std::to_string(bestHand.getDistance()));


    // return std::make_pair(bestDistance, bestHand);
    return bestHand;
}