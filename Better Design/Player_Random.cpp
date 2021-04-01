
#include "Player_Random.h"

/**************************************************/
/*               Private Functions                */
/**************************************************/

/******************************* ACTION CARDS *******************************/

int TrollPlayer::findRandomCard()
{
    int numCards = getHandSize();
    std::uniform_int_distribution<int> distribution(0, numCards - 1);
    int cardToRemove = distribution(this->randomGenerator);
    return cardToRemove;
}

/*  Function: discardCard()
    Goal:     Discard a random card from hand, without caring about
              the impact  or the best choice; if no cards, do nothing */
void TrollPlayer::action_discardCard()
{
    int numCards = getHandSize();
    if (numCards > 0)
    {
        int cardToRemove = findRandomCard();

        DEBUG("Final decision: remove card " + std::to_string(cardToRemove) + "\n");
        
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
    Goal:     Randomly discard a random number of cards (i.e. call
              discardCard x times, where x is random) */
void TrollPlayer::action_springCleaning()
{
    int numCards = getHandSize();
    std::uniform_int_distribution<int> distribution(0, numCards);
    int numCardsToRemove = distribution(this->randomGenerator);
    int cardToRemove;

    #ifdef _DEBUGSTATEMENTS
        std::string handBefore = this->hand->toString();
        std::string distanceBefore = std::to_string(this->hand->getDistance());
    #endif

    LOG("remove " + std::to_string(numCardsToRemove) + " cards... ");
    DEBUG("Removing " + std::to_string(numCardsToRemove) + " cards... \n");
    for (int i = 0; i < numCardsToRemove; i++)
    {
        cardToRemove = findRandomCard();
        DEBUG("- remove card " + std::to_string(cardToRemove) + ", ");
        DEBUG("distance " + std::to_string(this->hand->getDistance()) + " -> ");
        this->discardCard(cardToRemove);
        DEBUG(std::to_string(this->hand->getDistance()) + "... \n");
        TEST("remove " + std::to_string(cardToRemove) + "... ");
    }

    DEBUG("\nFinal decision: Removed " + std::to_string(numCardsToRemove) + " card(s)\n");
    DEBUG("- Hand Before:     " + handBefore + "\n");
    DEBUG("- Distance Before: " + distanceBefore + "\n");
    DEBUG("- Hand After:      " + this->hand->toString() + "\n");
    DEBUG("- Distance After:  " + std::to_string(this->hand->getDistance()));

    LOG("Player " + std::to_string(this->playerNum) + " removed card " + std::to_string(cardToRemove) + " ... ");
    #ifdef _TESTSTATEMENTS
        if(numCardsToRemove <= 0)
            std::cout << "don't remove any cards";
    #endif
}

/*  Function: poisonUnicorn()
    Goal:     Target a random player with a unicorn and poison one;
              decide which unicorn to poison randomly */
std::tuple<Player *, int> TrollPlayer::action_poisonUnicorn()
{
    // first, narrow down to only players who HAVE unicorns
    int numOtherPlayers = this->otherPlayers.size();
    std::vector<Player *> playersWithUnicorns = this->getPlayersWithUnicorns();
    int numPlayersWithUnicorns = playersWithUnicorns.size();
    Player *targetPlayer = NULL;
    int targetUnicorn = -1;
    if (numPlayersWithUnicorns > 0) // if at least one person has a unicorn....
    {
        // target a random player with a unicorn
        std::uniform_int_distribution<int> playerDistribution(0, numPlayersWithUnicorns - 1);
        targetPlayer = playersWithUnicorns[playerDistribution(this->randomGenerator)]; // found a target!!

        // then remove a random unicorn from that player
        std::uniform_int_distribution<int> unicornDistribution(1, targetPlayer->getUnicornCount());
        targetUnicorn = unicornDistribution(this->randomGenerator);
    }

    if (targetPlayer != NULL)
    {
        LOG("targetting player " + std::to_string(targetPlayer->getPlayerNum()) + " and poisoning unicorn " + std::to_string(targetUnicorn) + " ... ");
        TEST("targetting player " + std::to_string(targetPlayer->getPlayerNum()) + " and poisoning unicorn " + std::to_string(targetUnicorn) + " ... ");

        #ifdef _DEBUGSTATEMENTS
            Hand targetHandAfter = Hand(*targetPlayer->getHand());
            targetHandAfter.removeUnicorn(targetUnicorn);
            int targetDistanceAfter = targetHandAfter.getDistance();
        #endif

        DEBUG("Final decision:  target " + std::to_string(targetPlayer->getPlayerNum()) + " and poison unicorn " + std::to_string(targetUnicorn) + " \n");
        DEBUG("- Target Hand Before:     " + targetPlayer->getHand()->toString() + "\n");
        DEBUG("- Target Distance Before: " + std::to_string(targetPlayer->getHand()->getDistance()) + "\n");
        DEBUG("- Target Hand After:      " + targetHandAfter.toString() + "\n");
        DEBUG("- Target Distance After:  " + std::to_string(targetDistanceAfter));
    }
    else
    {
        LOG("no unicorns to remove ... ");
        TEST("no unicorns to remove... ");
        DEBUG("No unicorns to remove... ");
    }
    return std::tuple<Player *, int>(targetPlayer, targetUnicorn);
}

/*  Function: stealCard()
    Goal:     Steal a random card from a random player that has cards; 
              that is, if player has no cards, try another random player */
std::tuple<Player *, int> TrollPlayer::action_stealCard()
{
    // first, narrow down only to players with cards...
    int numOtherPlayers = this->otherPlayers.size();
    std::vector<Player *> playersWithCards;
    for (int i = 0; i < numOtherPlayers; i++)
    {
        if (this->otherPlayers[i]->getHandSize() > 0)
            playersWithCards.push_back(this->otherPlayers[i]);
    }

    int numPlayersWithCards = playersWithCards.size();

    Player *targetPlayer = NULL;
    int targetCard = -1;
    if (numPlayersWithCards > 0) // if at least one player has cards...
    {
        // target a random player that has cards
        std::uniform_int_distribution<int> playerDistribution(0, numPlayersWithCards - 1);
        targetPlayer = playersWithCards[playerDistribution(this->randomGenerator)];

        // then find a random card to steal
        std::uniform_int_distribution<int> cardDistribution(0, targetPlayer->getHandSize() - 1);
        targetCard = cardDistribution(this->randomGenerator); // found the card to steal!

        if (targetPlayer != NULL)
        {
            LOG("targetting player " + std::to_string(targetPlayer->getPlayerNum()) + " and stealing card " + std::to_string(targetCard) + " ... ");
            TEST("targetting player " + std::to_string(targetPlayer->getPlayerNum()) + " and stealing card " + std::to_string(targetCard) + " ... ");
            DEBUG("targetting player " + std::to_string(targetPlayer->getPlayerNum()) + " and stealing card " + std::to_string(targetCard) + "... ");
        }
        
    } else {
        TEST("no cards to steal... ");
        DEBUG("no cards to steal... ");
    }

    #ifdef _DEBUGSTATEMENTS
        Hand targetHandAfter = Hand(*targetPlayer->getHand());
        char cardRemoved = targetHandAfter.removeCard(targetCard);
        int targetDistanceAfter = targetHandAfter.getDistance();

        Hand ownHandAfter = Hand(*this->hand);
        ownHandAfter.addToHand(cardRemoved);
        int ownDistanceAfter =  ownHandAfter.getDistance();
    #endif

    DEBUG("\n\nFinal decision:  target " + std::to_string(targetPlayer->getPlayerNum()) + " and steal card " + std::to_string(targetCard) + " \n");
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

/**************************************************/
/*                Public Functions                */
/**************************************************/

TrollPlayer::TrollPlayer(Deck *deck, std::string goalString, Cards *cardInfo, int playerNum, int seed)
    : Player(deck, goalString, cardInfo, playerNum, seed)
{
    this->type = "troll";
}

TrollPlayer::~TrollPlayer()
{
    
}

    /*  Function: takeTurn()
    Goal:     Randomly decide to either draw a card or randomly rearrange your cards...
              obviously, if there is not more than 1 card in your hand, just draw;
              otherwise, randomly decide between drawing and rearranging */
char TrollPlayer::takeTurn()
{
    if(this->getHandSize() >= 20)
    {
        throw(TooManyCardsException());
    }

    LOG(" Take turn ... ");
    char card = ' ';

    if(this->getHandSize() > 1)
    {
        // we have more than 1 card, so randomly decide between drawing and rearranging
        std::uniform_int_distribution<int> choice(0, 100);
        int randomChoice = choice(this->randomGenerator);
        if(randomChoice < 50) // draw a card!
        {
            TEST("draw card... ");
            DEBUG("draw card... ")
            card = drawCard();
        }
        else // rearrange cards...
        {
            trySwapping();
        }
    }
    else
    {
        card = drawCard();
    }

    return card;
}

Hand TrollPlayer::trySwapping()
{
    LOG("Re-arrange cards ... ");
    std::uniform_int_distribution<int> cardDistribution(0, this->getHandSize() - 1);

    int targetCard = cardDistribution(this->randomGenerator);
    int newLocation = cardDistribution(this->randomGenerator);
    while(newLocation == targetCard)
    {
        DEBUG("move " + std::to_string(targetCard) + " to " + std::to_string(newLocation) + "... \n");
        newLocation = cardDistribution(this->randomGenerator);
    }
    LOG("move " + std::to_string(targetCard) + " to " + std::to_string(newLocation) + " ... ");
    TEST("move " + std::to_string(targetCard) + " to " + std::to_string(newLocation) + "... ");
    DEBUG("Final decision: move " + std::to_string(targetCard) + " to " + std::to_string(newLocation) + "... \n");

    DEBUG("- Hand Before:     " + this->hand->toString() + "\n");
    DEBUG("- Distance Before: " + std::to_string(this->hand->getDistance()) + "\n");
    this->moveCard(targetCard, newLocation);
    DEBUG("- Hand After:      " + this->hand->toString() + "\n");
    DEBUG("- Distance After:  " + std::to_string(this->hand->getDistance()));

    return *this->hand;
}