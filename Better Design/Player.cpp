
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
        std::cout << "Drew " << this->cardInfo->getCardName(newCard) << " .... ";
    }
    catch (RanOutOfCardsException &e)
    {
        throw e;
    }

    if (!(cardInfo->isActionCard(newCard)))
        // a non-action card was drawn, so add it to your hand
        hand->addToHand(newCard);
    else
    {
        handleActionCard(newCard);
    }
    

    return newCard;
}

void Player::handleActionCard(char type)
{
    switch (type)
    {
    case ACTION_CARD_DRAW:
        action_drawNonActionCard_helper();
        break;
    case ACTION_CARD_DISCARD:
        action_discardCard_helper();
        break;
    case ACTION_CARD_SPRING_CLEANING:
        action_springCleaning_helper();
        break;
    case ACTION_CARD_POISON:
        action_poisonUnicorn_helper();
        break;
    case ACTION_CARD_STEAL:
        action_stealCard_helper();
        break;
    default:
        break;
    }

    this->deck->discardCard(type); // discard the action card after use
}

/*  Function: drawNonActionCard_helper()
    Goal:     Draw a non action card from the deck and add it to your hand
    Throws:   RanOutOfCardsException   -- if you run out of cards
              OnlyActionCardsException -- if there is no non-action card left 
              (Both exceptions are passed from deck->drawNonActionCard) */
void Player::action_drawNonActionCard_helper()
{
    try
    {
        this->action_drawNonActionCard(); // do it for yourself

        // then, do it for everyone else...
        int numPlayers = this->otherPlayers.size();
        for (int i = 0; i < numPlayers; i++)
        {
            this->otherPlayers[i]->action_drawNonActionCard();
        }
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

void Player::action_discardCard_helper()
{
    this->action_discardCard(); // do it for yourself

    // then, do it for everyone else...
    int numPlayers = this->otherPlayers.size();
    for (int i = 0; i < numPlayers; i++)
    {
        this->otherPlayers[i]->action_discardCard();
    }
}

void Player::action_springCleaning_helper()
{
    this->action_springCleaning(); // do it for yourself

    // then, do it for everyone else...
    int numPlayers = this->otherPlayers.size();
    for (int i = 0; i < numPlayers; i++)
    {
        this->otherPlayers[i]->action_springCleaning();
    }
}

void Player::action_poisonUnicorn_helper()
{
    Player *targetPlayer;
    int targetUnicorn;
    std::tie(targetPlayer, targetUnicorn) = this->action_poisonUnicorn();

    if(targetPlayer != NULL) 
    {
        targetPlayer->hand->removeUnicorn(targetUnicorn);
        this->deck->discardCard(UNICORN);
    }
}

void Player::action_stealCard_helper()
{
    Player *targetPlayer;
    int targetCard;
    std::tie(targetPlayer, targetCard) = this->action_stealCard();

    if (targetPlayer != NULL)
    {
        this->hand->addToHand(targetPlayer->hand->removeCard(targetCard));
    }
}

/*  Function: stealCard_helper()
    Goal:     Steal the Nth card from targetPlayer and add it
              to your own hand, where N = cardToSteal */
void Player::stealCard_helper(int cardToSteal, Player *targetPlayer)
{
    char cardType = targetPlayer->hand->removeCard(cardToSteal);
    this->hand->addToHand(cardType);
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

/*  Function: initOtherPlayers()
    Goal:     Initializes pointers to the other players; this is needed
              for action cards, which may require interaction
              between players, such as stealing cards */
void Player::initOtherPlayers(std::vector<Player *> otherPlayers)
{
    for(int i = 0; i < otherPlayers.size(); i++) {
        if(i != this->playerNum)
            this->otherPlayers.push_back(otherPlayers[i]);
    }
}

/*  Function: action_drawNonActionCard()
    Goal:     Do the "draw non action card" for yourself and everyone else
    Throws:   RanOutOfCardsException   -- if you run out of cards
              OnlyActionCardsException -- if there is no non-action card left 
              (Both exceptions are passed from deck->drawNonActionCard) */
void Player::action_drawNonActionCard()
{
    try
    {
        char nonActionCard = deck->drawNonActionCard();
        hand->addToHand(nonActionCard);
        std::cout << "Player " << this->playerNum << " drew " << this->cardInfo->getCardName(nonActionCard) << " ...";
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


/*  Function: discardCard()
    Goal:     Remove the Nth card from your hand and put it
              back in the deck, where N = card */
void Player::discardCard(int card)
{
    char cardType = this->hand->removeCard(card);
    this->deck->discardCard(cardType);
}