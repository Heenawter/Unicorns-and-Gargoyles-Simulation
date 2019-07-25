#include "Deck.h"

/**************************************************/
/*                Private Functions               */
/**************************************************/

// none yet!

/**************************************************/
/*                Public Functions                */
/**************************************************/

Deck::Deck(std::map<char, int> cardCounts)
{
    int count = 0;
    for (auto const &x : cardCounts)
    {
        for(count = 0; count < x.second; count++)
        {
            cards.push_back(x.first);
        }
    }
}

Deck::~Deck()
{

}

/*  Function: shuffleDeck()
    Goal:     Shuffle the main deck */
void Deck::shuffleDeck()
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(cards.begin(), cards.end(), std::default_random_engine(seed));
}

char Deck::drawNextCard()
{

}

char Deck::drawNonActionCard()
{

}

void Deck::discardCard(char card)
{

}