#include "Debugging.h"
#include "Game.h"
#include "GameStates.h"

/*
    - loop through each goal
        - loop through 2-10 players
            - find ideal number of players for current deck configuration
*/

int main()
{
    std::cout << "main" << std::endl;

    Game *game = new Game(5, "[ ][*][*][ ]");
    // Player *player1 = game->getPlayer(1);
    // Player *player2 = game->getPlayer(2);

    // player1->drawNonActionCard();
    // player1->drawNonActionCard();
    // player1->drawNonActionCard();

    // player1->discardCard();

    // for (int i = 0; i < 10; i++)
    // {
    //     game->gameRound();
    // }



    // Hand* hand1 = player1->getHand();
    // Hand* hand2 = player2->getHand();

    // bool test = (*hand1) < *hand2;
    // std::cout << test << std::endl;

    // try
    // {
    //     for (int i = 0; i < 10; i++)
    //     {
    //         game->gameRound();
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
    // catch (std::exception &e3)
    // {
    //     std::cout << e3.what() << std::endl;
    // }


    // delete game;

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