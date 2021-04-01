#include "Deck.h"
#include "Cards.h"
#include "GameStates.h"

/**************************************************/
/*                Private Functions               */
/**************************************************/

/*  Function: hasNonActionCard(<given deck>)
    Goal:     Determine whether the given deck contains
              a non-action card. */
bool Deck::hasNonActionCard(std::vector<char> deck)
{
    std::vector<char>::iterator it;
    for (it = deck.begin(); it < deck.end(); it++)
    {
        if (*it < ACTION_CARD_DISCARD)
            return true; // found a non-action card
    }

    return false; // did NOT find a non-action card
}

/**************************************************/
/*                Public Functions                */
/**************************************************/

/*  Function: Deck(<map>)
    Goal:     Build a deck object with counts */
Deck::Deck(std::map<char, int> cardCounts, Cards *cardInfo, int seed)
{
    this->gameSeed = seed;
    this->cardInfo = cardInfo;
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
    std::shuffle(cards.begin(), cards.end(), std::mt19937(this->gameSeed));
}

/*  Function: drawNextCard()
    Goal:     Draw the next available card from the main deck.
              If there are no cards available, try making the discard deck the
              new main deck; if this also has no cards, then throw an exception
              since you ran out of cards completely.
    Throws:   RanOutOfCardsException */
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

/*  Function: drawNonActionCard()
    Goal:     Draw a non action card from the deck; if you draw an action
              card, discard it and try again until it is a non action.
    Throws:   RanOutOfCardsException   -- if you run out of cards
              OnlyActionCardsException -- if there is no non-action card left */
char Deck::drawNonActionCard()
{
    char nextCard;
    bool nonActionCardExists = hasNonActionCard();

    // std::cout << this->toString() << std::endl;

    // first, this can only be done IF A NON-ACTION CARD exists
    // in either the main deck or the discard deck; so, check this.
    if(!nonActionCardExists)
        throw(OnlyActionCardsException());

    // an action card exists, so now we just have to find it.
    try
    {
        nextCard = drawNextCard();
        while (nextCard >= ACTION_CARD_DISCARD)
        {
            // while the card drawn is an action card...
            discardCard(nextCard); // discard the action card
            nextCard = drawNextCard();
        }
    }
    catch (RanOutOfCardsException &e)
    {
        throw e;
    }

    return nextCard;
}

/*  Function: discardCard()
    Goal:     Put the given card into the discard deck */
void Deck::discardCard(char card)
{
    discard.push_back(card);
}

/*  Function: hasNonActionCard()
    Goal:     Determine whether EITHER deck (i.e. either the main deck
              or the discard deck) contains a non-action card. */
bool Deck::hasNonActionCard()
{
    bool mainDeckCheck = hasNonActionCard(cards);
    // if the main deck has a non action card, no point checking
    // the discard deck... so just return
    if(mainDeckCheck) 
        return true;
    
    // otherwise, the main deck does NOT have an action card,
    // so check the discard deck and return whether or not
    // it has a non-action card.
    bool discardDeckCheck = hasNonActionCard(discard);
    return discardDeckCheck;
}

std::string Deck::getCardName(char card)
{
    return cardInfo->getCardName(card);
}

std::string Deck::toString() 
{
    std::string str = "";

    str += "-- Deck --\n";
    for(int i = 0; i < cards.size(); i++)
    {
        str += cardInfo->getCardName(cards[i]);
    }

    str += "\n-- Discard --\n";
    for (int i = 0; i < discard.size(); i++)
    {
        str += cardInfo->getCardName(discard[i]);
    }
    return str;
}