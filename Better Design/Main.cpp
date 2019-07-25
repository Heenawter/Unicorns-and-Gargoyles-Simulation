#include "Main.h"
#include "Cards.h"
#include "Deck.h"
#include "GameStates.h"

int main()
{
    std::cout << "main" << std::endl;

    Cards cardRules;
    cardRules.readCards();
    std::cout << cardRules.getCardName(14) << std::endl;

    Deck deck(cardRules.getCardCounts());
    char newCard;

    try
    {
        for(int i = 0; i < 100; i++)
        {
            newCard = deck.drawNextCard();
            std::cout << cardRules.getCardName(newCard) << std::endl;
        }
    }
    catch (RanOutOfCardsException &r)
    {
        std::cout << r.what() << std::endl;
    }
}