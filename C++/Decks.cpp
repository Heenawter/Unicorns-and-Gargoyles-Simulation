#include "Decks.h"

#include <chrono> // need std::chrono::system_clock for randomness

short Deck::getCardType(std::string card)
{
    return map[card];
}

std::string Deck::getCardName(short cardType)
{
    for (std::map<std::string, short>::iterator it = map.begin(); it != map.end(); it++)
        if (it->second == cardType)
            return it->first;
}

void Deck::shuffleDeck()
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(cards.begin(), cards.end(), std::default_random_engine(seed));
}

void Deck::addToDeck(std::string line)
{
    std::string cardName;
    int count;
    short cardType;

    cardName = line.substr(0, line.find_first_of(','));
    count = int(line[line.length() - 1]) - 48; // -48 to convert from ascii to int

    for (int j = 0; j < count; j++)
    {
        cardType = getCardType(cardName);
        cards.push_back(cardType);
    }
}

short Deck::drawCard()
{
    short newCard = cards.back();
    cards.pop_back();
    return newCard;
}