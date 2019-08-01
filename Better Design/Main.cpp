#include "Debugging.h"
#include "Game.h"
#include "GameStates.h"


int main()
{
    std::cout << "main" << std::endl;

    Game* game = new Game(5, "[ ][*][*][ ]");
    Player* player1 = game->getPlayer(1);

    for (int i = 0; i < 100; i++)
    {
        try
        {
            player1->takeTurn();
        }
        catch (RanOutOfCardsException &e1)
        {
            std::cout << e1.what() << std::endl;
            i = 101;
        }
        catch (ActionCardException &e3)
        {
            try
            {
                game->handleActionCard(e3.triggeringPlayer, e3.type);
            }
            catch (RanOutOfCardsException &e1)
            {
                std::cout << e1.what() << std::endl;
                i = 101;
            }
            catch (OnlyActionCardsException &e4)
            {
                std::cout << e4.what() << std::endl;
                i = 101;
            }
        }
    }

    delete game;

        // Cards cardRules;
        // cardRules.readCards();
        // std::cout << cardRules.getCardName(14) << std::endl;

        // Deck deck;
        // char newCard;

        // try
        // {
        //     for(int i = 0; i < 100; i++)
        //     {
        //         newCard = deck.drawNonActionCard();
        //         std::cout << deck.getCardName(newCard) << std::endl;
        //     }
        // }
        // catch (RanOutOfCardsException &e1)
        // {
        //     std::cout << e1.what() << std::endl;
        // }
        // catch (OnlyActionCardsException &e2)
        // {
        //     std::cout << e2.what() << std::endl;
        // }
}