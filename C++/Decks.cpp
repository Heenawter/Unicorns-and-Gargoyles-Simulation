
#include "Decks.h"

short Deck::getCardType(std::string card) {
    return map[card];
}

std::string Deck::getCardName(short cardType) {
    for (std::map<std::string, short>::iterator it = map.begin(); it != map.end(); it++)
        if (it->second == cardType)
            return it->first;
}

void Deck::shuffleDeck() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(cards.begin(), cards.end(), std::default_random_engine(seed));
}

short Deck::drawCard() {
    short newCard = cards.back();
    cards.pop_back();
    return newCard;
}