
#include "Player_Random.h"

/**************************************************/
/*               Private Functions                */
/**************************************************/

/******************************* ACTION CARDS ******************************* /

/*  Function: discardCard()
    Goal:     Discard a random card from hand, without caring about
              the impact  or the best choice; if no cards, do nothing */
void TrollPlayer::action_discardCard()
{
    int numCards = getHandSize();
    if (numCards > 0)
    {
        std::uniform_int_distribution<int> distribution(0, numCards - 1);
        int cardToRemove = distribution(this->randomGenerator);

        this->discardCard(cardToRemove);

        LOG("Player " + std::to_string(this->playerNum) + " removed card " + std::to_string(cardToRemove) + " ... ");
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

    LOG("remove " + std::to_string(numCardsToRemove) + " cards ... ");
    for (int i = 0; i < numCardsToRemove; i++)
    {
        this->action_discardCard();
    }
}

/*  Function: poisonUnicorn()
    Goal:     Target a random player with a unicorn and poison one;
              decide which unicorn to poison randomly */
std::tuple<Player *, int> TrollPlayer::action_poisonUnicorn()
{
    // first, narrow down to only players who HAVE unicorns
    int numOtherPlayers = this->otherPlayers.size();
    std::vector<Player *> playersWithUnicorns;
    for (int i = 0; i < numOtherPlayers; i++)
    {
        if (this->otherPlayers[i]->getUnicornCount() != 0)
            playersWithUnicorns.push_back(this->otherPlayers[i]);
    }

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
    }
    else
    {
        LOG("no player targetted ... ");
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
    }

    if (targetPlayer != NULL)
    {
        LOG("targetting player " + std::to_string(targetPlayer->getPlayerNum()) + " and stealing card " + std::to_string(targetCard) + " ... ");
    }
    else
    {
        LOG("no player targetted ... ");
    }
    return std::tuple<Player *, int>(targetPlayer, targetCard);
}

/**************************************************/
/*                Public Functions                */
/**************************************************/

TrollPlayer::TrollPlayer(Deck *deck, std::string goalString, Cards *cardInfo, int playerNum)
    : Player(deck, goalString, cardInfo, playerNum)
{
    int seed = std::chrono::system_clock::now().time_since_epoch().count();
    this->randomGenerator = std::mt19937(seed);
}

char TrollPlayer::takeTurn()
{
    LOG(" Take turn ... ");
    char card = ' ';

    // randomly decide to either draw a card or randomly rearrange your cards...
    // obviously, if there is not more than 1 card in your hand, just draw;
    // otherwise, randomly decide between drawing and rearranging

    if(this->getHandSize() > 1)
    {
        // we have more than 1 card, so randomly decide between drawing and rearranging
        std::uniform_int_distribution<int> choice(0, 100);
        int randomChoice = choice(this->randomGenerator);
        if(randomChoice < 50) // draw a card!
        {
            card = drawCard();
        }
        else // rearrange cards...
        {
            LOG("Re-arrange cards ... ");
            std::uniform_int_distribution<int> cardDistribution(0, this->getHandSize() - 1);
            int targetCard = cardDistribution(this->randomGenerator);
            int newLocation = cardDistribution(this->randomGenerator);
            while(newLocation == targetCard)
            {
                newLocation = cardDistribution(this->randomGenerator);
            }
            LOG("move " + std::to_string(targetCard) + " to " + std::to_string(newLocation) + " ... ");

            this->moveCard(targetCard, newLocation);
        }
    }
    else
    {
        card = drawCard();
    }
    
    return card;
}