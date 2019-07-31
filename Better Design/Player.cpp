
#include "Player.h"

/**************************************************/
/*                Private Functions               */
/**************************************************/

/**************************************************/
/*                Public Functions                */
/**************************************************/

/*  Function: Player()
    Goal:     Constructor for the Player object;
              Takes a pointer to the Table instance and
              a pointer to the Cards instance;
              Generates the hand object for the player */
Player::Player(Deck *deck, std::string goalString, Cards *cardInfo)
{
    this->deck = deck;
    this->cardInfo = cardInfo;
    hand = new Hand(goalString, cardInfo);
}   

/*  Function: ~Player()
    Goal:     Deconstructor for the Player object; 
              For every new, there must be a delete. */
Player::~Player()
{
    delete hand;
}

char Player::drawCard()
{
    try
    {
        char newCard = deck->drawNextCard();
        if (cardInfo->isActionCard(newCard))
            // this is an action card, so don't add it to your hand;
            // instead, return it so Game.cpp can handle it
            return newCard;
        
        // a non-action card was drawn, so add it to your hand
        hand->addToHand(newCard);
    }
    catch (RanOutOfCardsException &e1)
    {
        throw e1;
    }
    return 'X';
}