
#include "Player.h"

#include "GameStates.h"

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
Player::Player(Deck *deck, std::string goalString, Cards *cardInfo, int playerNum)
{
    this->deck = deck;
    this->cardInfo = cardInfo;
    this->playerNum = playerNum;
    hand = new Hand(goalString, cardInfo);
}   

/*  Function: ~Player()
    Goal:     Deconstructor for the Player object; 
              For every new, there must be a delete. */
Player::~Player()
{
    delete hand;
}

/*  Function: takeTurn()
    Goal:     Draws a new card from the deck (if possible).
              If it is an action card, call the action card handler;
              otherwise, append it to your current hand.  
    Throws:   RanOutOfCardsException */
void Player::takeTurn()
{
    try
    {
        char newCard = deck->drawNextCard();
        if (cardInfo->isActionCard(newCard))
            // this is an action card, so don't add it to your hand;
            // instead, return it so Game.cpp can handle it
            handleActionCard(newCard);
        else
            // a non-action card was drawn, so add it to your hand
            hand->addToHand(newCard);
    }
    catch (RanOutOfCardsException &e)
    {
        throw e;
    }
}

void Player::handleActionCard(char card)
{
    if(card == ACTION_CARD_DISCARD)
        throw ActionCardException_Draw(this);
}

void Player::drawNonActionCard()
{
    try
    {
        char nonActionCard = deck->drawNonActionCard();
        hand->addToHand(nonActionCard);
    }
    catch(std::exception& e)
    {
        throw e;
    }
}