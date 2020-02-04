
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
    // std::cout << "before: " << this->hand->getDistance() << ", " << this->hand->toString() << std::endl;
    int numCards = getHandSize();
    if (numCards > 0)
    {
        int cardToRemove = 0;
        // first, try removing a card that makes the hand better
        bool removed = removeFirst(this, cardToRemove, std::less<int>());

        if (!removed)
        {
            // then, remove the first card that doesn't make the hand worse...
            removed = removeFirst(this, cardToRemove, std::less_equal<int>());

            if (!removed)
            {
                // then, just remove the first card regardless of the consequences
                cardToRemove = 0;
            }
        }

        this->discardCard(cardToRemove);
        LOG("Player " + std::to_string(this->playerNum) + " removed card " + std::to_string(cardToRemove) + " ... ");
        // std::cout << "after:  " << this->hand->getDistance() << ", " << this->hand->toString() << std::endl;
    }
}

/*  Function: springCleaning()
    Goal:     Remove a card; if the solution gets better, continue with 
              that hand and try removing another card.
              Basically, go through the hand ONCE and, every time you 
              encounter a card such that, when it is removed, the hand
              gets closer to the goal, remove it and continue */
void GreedyPlayer::action_springCleaning()
{
    Hand testHand = Hand(*this->hand); // use copy constructor to make a copy
    Hand currentHand = Hand(*this->hand);
    int testDistance;
    int currentDistance = currentHand.getDistance();

    for (int i = 0; i < testHand.getNumCards(); i++)
    {
        testHand.removeCard(i);
        testDistance = testHand.getDistance();

        // std::cout << i << ", distance: " << currentHand.getDistance();
        // std::cout << ", current hand: " << currentHand.toString() << std::endl;
        // std::cout << i << ", distance: " << testHand.getDistance();
        // std::cout << ", test hand:    " << testHand.toString() << std::endl;

        if (testDistance < currentDistance)
        {
            // std::cout << "... remove card " << i << " ... " << std::endl;
            // LOG("removed card " + std::to_string(i) + " ... ");
            currentHand.removeCard(i);
            currentDistance = currentHand.getDistance();
            i--;
        }
        testHand = Hand(currentHand);
        // std::cout << "Test hand: " << testHand.toString() << std::endl;
    }

    std::vector<int> setDifference = this->hand->setDifference(currentHand);
    for (int i = 0; i < setDifference.size(); i++)
    {
        LOG("removed card " + std::to_string(i) + " ... ");
        this->discardCard(setDifference[i]);

        // adjust next indeces, since hand is now 1 smaller
        for (int j = i + 1; j < setDifference.size(); j++)
        {
            setDifference[j]--;
        }
    }

    // delete this->hand;
    // this->hand = new Hand(currentHand);
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

    if(numOtherPlayers > 0)
    {
        std::function<bool(int, int)> cases[2] = {std::greater<int>(), std::greater_equal<int>()};

        for (int currentCase = 0; currentCase < 2 && !removed; currentCase++)
        {
            // case 0: check if removing a unicorn can make the hand WORSE
            // case 1: check if removing a unicorn won't make a difference
            // case 2: removing a unicorn ONLY makes every hand better
            // std::cout << "-------- case: " << currentCase << " --------" << std::endl;
            for (int i = 0; i < numOtherPlayers && !removed; i++)
            {
                // for each player, go through their hand and try removing...
                targetUnicorn = 0;
                targetPlayer = playersWithUnicorns[i];
                removed = removeFirstUnicorn(targetPlayer, targetUnicorn, cases[currentCase]);
            }
        }

        if (removed) // case 0 or 1
        {
            LOG("targetting player " + std::to_string(targetPlayer->getPlayerNum()) + " and poisoning unicorn " + std::to_string(targetUnicorn) + " ... ");
        }
        else  // case 2
        {
            targetPlayer = playersWithUnicorns[0];
            targetUnicorn = 1;
            LOG("targetting first player " + std::to_string(targetPlayer->getPlayerNum()) + " and poisoning unicorn " + std::to_string(targetUnicorn) + " ... ");
        }
    }
    else
    {
        LOG("no unicorns to remove ... ");
    }

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

    if(numOtherPlayers > 0)
    {
        bool stolen = false;
        std::function<bool(int, int)> cases[2] = {std::less<int>(), std::less_equal<int>()};
        for (int currentCase = 0; currentCase < 2 && !stolen; currentCase++)
        {
            // case 0: check if stealing the card will IMPROVE your hand
            // case 1: check if stealing tha card won't make a difference
            // case 2: stealing a card only makes you hand worse
            for (int i = 0; i < numOtherPlayers && !stolen; i++)
            {
                // for each player, go through their hand and try stealing...
                targetCard = 0;
                targetPlayer = playersWithCards[i];

                stolen = stealFirst(targetPlayer, targetCard, cases[currentCase]);
                // if(stolen)
                // {
                //     std::cout << "current distance: " << this->hand->getDistance() << std::endl;
                //     std::cout << "stole card " << targetCard << " from Player " << targetPlayer->getPlayerNum() << std::endl;
                //     this->hand->addToHand(targetPlayer->getCard(targetCard));
                //     std::cout << "after distance: " << this->hand->getDistance() << std::endl;
                // }
            }
        }

        if (stolen) // case 0 or 1
        {
            LOG("targetting player " + std::to_string(targetPlayer->getPlayerNum()) + " and stealing card " + std::to_string(targetCard) + " ... ");
        }
        else // case 2
        {
            targetPlayer = playersWithCards[0];
            targetCard = 0;
            LOG("targetting first player " + std::to_string(targetPlayer->getPlayerNum()) + " and stealing card " + std::to_string(targetCard) + " ... ");
        }
    }
    else
    {
        LOG("no cards to steal ... ");
    }

    return std::tuple<Player *, int>(targetPlayer, targetCard);
}

/*  Function: removeFirst()
    Goal:     Remove the first card such that func(testDistance, currentDistance)
              returns true; this function expects <, <=, ==, >=, > for func; 
              for example, if func = < = std::less,
                    - find and remove the first card such that, when it is 
                      removed, it makes the hand BETTER (i.e. closer to 
                      the goal string) */
bool GreedyPlayer::removeFirst(Player *player, int &cardToRemove, std::function<bool(int, int)> func)
{
    Hand testHand = Hand(*this->hand); // use copy constructor to make a copy
    int testDistance = this->hand->getDistance();
    int currentDistance = testDistance;
    bool removed = false;

    for (int i = 0; i < testHand.getNumCards() && !removed; i++)
    {
        testHand.removeCard(i);
        testDistance = testHand.getDistance();

        if (func(testDistance, currentDistance)) // either <, <=, >, >=, or ==
        {
            removed = true;
            // this->hand->removeCard(i);
            cardToRemove = i;
        }
        else
        {
            testHand = Hand(*this->hand);
        }
    }

    return removed;
}

bool GreedyPlayer::stealFirst(Player *player, int &cardToSteal, std::function<bool(int, int)> func)
{
    Hand testHand = Hand(*this->hand); // use copy constructor to make a copy
    int testDistance = this->hand->getDistance();
    char testCard;
    int currentDistance = testDistance;
    bool removed = false;

    // std::cout << "Player " << player->getPlayerNum() << " before distance: " << player->getHand()->getDistance() << std::endl;
    for (int i = 0; i < player->getHandSize() && !removed; i++)
    {
        // testHand.removeCard(i);
        testCard = player->getCard(i);
        testHand.addToHand(testCard);
        testDistance = testHand.getDistance();

        if (func(testDistance, currentDistance)) // either <, <=, >, >=, or ==
        {
            removed = true;
            // std::cout << "... removed!" << std::endl;
            cardToSteal = i;
        }
        else
        {
            testHand = Hand(*this->hand);
        }
    }
    // std::cout << "Player " << player->getPlayerNum() << " after distance: " << testDistance << std::endl;

    return removed;
}

/*  Function: removeFirstUnicorn()
    Goal:     Remove the first unicorn such that func(testDistance, currentDistance)
              returns true; this function expects <, <=, ==, >=, > for func; 
              for example, if func = > = std::greater,
                    - find and remove the first unicorn such that, when it is 
                      removed, it makes the hand WORSE (i.e. farther from 
                      the goal string) */
bool GreedyPlayer::removeFirstUnicorn(Player* player, int& unicornNumber, std::function<bool(int, int)> func)
{
    bool removed = false;

    Hand* currentHand = player->getHand();
    char currentCard;
    int currentDistance;

    Hand testHand = Hand(*currentHand);
    int testDistance;

    for (int i = 0; i < currentHand->getNumCards() & !removed; i++)
    {
        currentCard = currentHand->getCard(i);
        currentDistance = currentHand->getDistance();
        if (currentCard == UNICORN)
        {
            unicornNumber++;
            testHand = Hand(*currentHand);
            testHand.removeCard(i);
            testDistance = testHand.getDistance();

            if (func(testDistance, currentDistance))
            {
                removed = true;
            }
        }
    }
    return removed;
}

/**************************************************/
/*                Public Functions                */
/**************************************************/

GreedyPlayer::GreedyPlayer(Deck *deck, std::string goalString, Cards *cardInfo, int playerNum)
: Player(deck, goalString, cardInfo, playerNum)
{
}

/*  Function: takeTurn()
    Goal:     If you have less than 2 cards, simply draw a card; otherwise,
              if swapping will improve your current hand, then do that;
              for swapping, simply choose the first possible swap that will
              improve your hand, even if a better swap down the line exists;
              if swapping does not help, draw a new card */
char GreedyPlayer::takeTurn()
{
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
                if (testDistance < currentDistance)
                {
                    swapped = true;
                    bestHand = Hand(testHand);
                    LOG("move " + std::to_string(cardToMove) + " to " + std::to_string(whereToMove) + " ... ");
                } else {
                    testHand = Hand(*this->hand);
                }
            }
        }
    }

    // return std::make_pair(bestDistance, bestHand);
    return bestHand;
}