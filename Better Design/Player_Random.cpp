
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

/******************************* ACTION CARDS ******************************* /

/*  Function: discardCard()
    Goal:     Discard a random card from hand, without caring about
              the impact  or the best choice; if no cards, do nothing */
void TrollPlayer::discardCard()
{
    int numCards = this->hand->getNumCards();
    if(numCards > 0)
    {
        std::uniform_int_distribution<int> distribution(0, numCards - 1);
        int cardToRemove = distribution(this->randomGenerator);
        this->hand->removeCard(cardToRemove);
    }
}

/*  Function: springCleaning()
    Goal:     Randomly discard a random number of cards (i.e. call
              discardCard x times, where x is random) */
void TrollPlayer::springCleaning()
{
    int numCards = this->hand->getNumCards();
    std::uniform_int_distribution<int> distribution(0, numCards);
    int numCardsToRemove = distribution(this->randomGenerator);

    for(int i = 0; i < numCardsToRemove; i++)
    {
        discardCard();
    }
}

/*  Function: poisonUnicorn()
    Goal:     Target a random player with a unicorn and poison one;
              decide which unicorn to poison randomly */
void TrollPlayer::poisonUnicorn()
{
    // first, narrow down to only players who HAVE unicorns
    int numOtherPlayers = this->otherPlayers.size();
    std::vector<Player*> playersWithUnicorns;
    for(int i = 0; i < numOtherPlayers; i++) 
    {
        if(this->otherPlayers[i]->getHand()->getNumUnicorns() != 0) 
        {
            playersWithUnicorns.push_back(this->otherPlayers[i]);
        }
    }

    int numPlayersWithUnicorns = playersWithUnicorns.size();
    if (numPlayersWithUnicorns > 0) // if at least one person has a unicorn....
    {
        // remove a random unicorn
        std::uniform_int_distribution<int> playerDistribution(0, numPlayersWithUnicorns - 1);
        int targetPlayer = playerDistribution(this->randomGenerator);

        Hand* targetHand = playersWithUnicorns[targetPlayer]->getHand();
        std::uniform_int_distribution<int> unicornDistribution(1, targetHand->getNumUnicorns());
        int targetUnicorn = unicornDistribution(this->randomGenerator);

        targetHand->removeUnicorn(targetUnicorn);
    }
}

/*  Function: stealCard()
    Goal:     Steal a random card from a random player that has cards; 
              that is, if player has no cards, try another random player */
void TrollPlayer::stealCard()
{
    
}