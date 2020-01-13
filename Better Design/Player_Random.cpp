
#include "Player_Random.h"

TrollPlayer::TrollPlayer(Deck *deck, std::string goalString, Cards *cardInfo, int playerNum)
    : Player(deck, goalString, cardInfo, playerNum)
{
    int seed = std::chrono::system_clock::now().time_since_epoch().count();
    this->randomGenerator = std::mt19937(seed);
}

TrollPlayer::~TrollPlayer()
{

}

char TrollPlayer::takeTurn()
{
    std::cout << " Take turn ... ";

    char card = drawCard();
    return card;
}

/******************************* ACTION CARDS ******************************* /

/*  Function: discardCard()
    Goal:     Discard a random card from hand, without caring about
              the impact  or the best choice; if no cards, do nothing */
void TrollPlayer::action_discardCard()
{
    int numCards = getHandSize();
    if(numCards > 0)
    {
        std::uniform_int_distribution<int> distribution(0, numCards - 1);
        int cardToRemove = distribution(this->randomGenerator);
        
        this->discardCard(cardToRemove);

        std::cout << "Player " << this->playerNum << " removed card " << cardToRemove << " ... ";
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

    std::cout << "remove " << numCardsToRemove << " cards... ";
    for(int i = 0; i < numCardsToRemove; i++)
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
    std::vector<Player*> playersWithUnicorns;
    for(int i = 0; i < numOtherPlayers; i++) 
    {
        if (this->otherPlayers[i]->getUnicornCount() != 0)
            playersWithUnicorns.push_back(this->otherPlayers[i]);
    }

    int numPlayersWithUnicorns = playersWithUnicorns.size();
    Player* targetPlayer = NULL;
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

    if(targetPlayer != NULL)
        std::cout << "targetting player " << targetPlayer->getPlayerNum() << " and poisoning unicorn " << targetUnicorn << " ...";
    else
        std::cout << "no player targetted... ";
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
    for(int i = 0; i < numOtherPlayers; i++)
    {
        if (this->otherPlayers[i]->getHandSize() > 0)
            playersWithCards.push_back(this->otherPlayers[i]);
    }

    int numPlayersWithCards = playersWithCards.size();
    Player* targetPlayer = NULL;
    int targetCard = -1;
    if(numPlayersWithCards > 0) // if at least one player has cards...
    {
        // target a random player that has cards
        std::uniform_int_distribution<int> playerDistribution(0, numPlayersWithCards - 1);
        targetPlayer = playersWithCards[playerDistribution(this->randomGenerator)]; 

        // then find a random card to steal
        std::uniform_int_distribution<int> cardDistribution(0, targetPlayer->getHandSize() - 1);
        targetCard = cardDistribution(this->randomGenerator); // found the card to steal!
    }


    if (targetPlayer != NULL)
        std::cout << "targetting player " << targetPlayer->getPlayerNum() << " and stealing card " << targetCard << " ...";
    else
        std::cout << "no player targetted... ";
    return std::tuple<Player *, int>(targetPlayer, targetCard);
}