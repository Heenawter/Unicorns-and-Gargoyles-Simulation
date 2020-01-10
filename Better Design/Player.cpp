
#include "Player.h"

#include "GameStates.h"

/**************************************************/
/*                Private Functions               */
/**************************************************/

/*  Function: drawCard()
    Goal:     Draws a new card from the deck (if possible).
              Returns the card the player drew in order to handle
              action cards; notice that action cards are NOT
              appended to the hand.
    Throws:   RanOutOfCardsException -- passed from drawNextCard */
char Player::drawCard()
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

void Player::initOtherPlayers(std::vector<Player *> otherPlayers)
{
    for(int i = 0; i < otherPlayers.size(); i++) {
        if(i != this->playerNum)
            this->otherPlayers.push_back(otherPlayers[i]);
    }
}

char Player::takeTurn()
{
    
    return drawCard();
}

/*  Function: drawNonActionCard()
    Goal:     Draw a non action card from the deck and add it to your hand
    Throws:   RanOutOfCardsException   -- if you run out of cards
              OnlyActionCardsException -- if there is no non-action card left 
              (Both exceptions are passed from deck->drawNonActionCard) */
void Player::drawNonActionCard()
{
    try
    {
        char nonActionCard = deck->drawNonActionCard();
        hand->addToHand(nonActionCard);
    }
    catch (RanOutOfCardsException &e1)
    {
        throw e1;
    }
    catch (OnlyActionCardsException &e2)
    {
        throw e2;
    }
}