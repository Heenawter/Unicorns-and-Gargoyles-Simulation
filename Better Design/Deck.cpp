#include "Deck.h"
#include "Cards.h"
#include "GameStates.h"

/**************************************************/
/*                Private Functions               */
/**************************************************/

// none yet!

/**************************************************/
/*                Public Functions                */
/**************************************************/

/*  Function: Deck()
    Goal:     Build a deck based on the counts */
Deck::Deck(std::map<char, int> cardCounts)
{
    for (auto const &x : cardCounts)
    {
        for(int count = 0; count < x.second; count++)
        {
            cards.push_back(x.first);
        }
    }

    shuffleDeck();
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
    if(cards.size() == 0)
    {
        // ran out of cards, so make the DISCARD deck the new MAIN deck (if possible)
        if(discard.size() > 0)
        {
            // discard deck still has cards, so we can go ahead and swap decks
            cards = discard; // assign the new deck to be the discard (hard copy)
            discard.clear(); // empty the discard deck once more
        } else {
            // discard deck ALSO ran out of cards... oh no!
            throw(RanOutOfCardsException());
        }
    }

    // now that we know there is at least one card, go ahead and draw one
    char nextCard = cards.back(); // get the last element
    cards.pop_back();             // then remove the last element
    return nextCard;
}

char Deck::drawNonActionCard()
{

}

void Deck::discardCard(char card)
{

}