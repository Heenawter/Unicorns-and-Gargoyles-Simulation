#include "Main.h"
#include "Cards.h"
#include "Deck.h"
#include "GameStates.h"

int main()
{
    std::cout << "main" << std::endl;

    // Cards cardRules;
    // cardRules.readCards();
    // std::cout << cardRules.getCardName(14) << std::endl;

    Deck deck;
    char newCard;

    try
    {
        for(int i = 0; i < 100; i++)
        {
            newCard = deck.drawNonActionCard();
            std::cout << deck.getCardName(newCard) << std::endl;
        }
    }
    catch (RanOutOfCardsException &e1)
    {
        std::cout << e1.what() << std::endl;
    }
    catch (OnlyActionCardsException &e2)
    {
        std::cout << e2.what() << std::endl;
    }
}