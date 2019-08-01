
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
              Returns the card the player drew in order to handle
              action cards; notice that action cards are NOT
              appended to the hand.
    Throws:   RanOutOfCardsException 
              ActionCardException */
char Player::takeTurn()
{
    char newCard = ' ';
    try
    {
        newCard = deck->drawNextCard();
    }
    catch (RanOutOfCardsException &e)
    {
        throw e;
    }

    if (!(cardInfo->isActionCard(newCard)))
        // a non-action card was drawn, so add it to your hand
        hand->addToHand(newCard);

    return newCard;
}

/*  Function: drawNonActionCard()
    Goal:     Draw a non action card from the deck and add it to your hand
    Throws:   RanOutOfCardsException   -- if you run out of cards
              OnlyActionCardsException -- if there is no non-action card left */
void Player::drawNonActionCard()
{
    try
    {
        char nonActionCard = deck->drawNonActionCard();
        hand->addToHand(nonActionCard);
    }
    catch(std::exception& e)
    {
        // catches either RanOutOfCardsException or OnlyActionCardsException
        throw e;
    }
}