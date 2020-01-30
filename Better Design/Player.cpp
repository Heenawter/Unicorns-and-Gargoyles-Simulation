
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
        LOG("Drew " + this->cardInfo->getCardName(newCard) + " ... ");
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

/*  Function: handleActionCard()
    Goal:     Handles calling the appropriate helper function depending on
              the action card drawn  */
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

/*  Function: action_discardCard_helper()
    Goal:     Helper for discardCard function, which is virtual function that depends on
              the player type; for each player, call their relevant discardCard function */
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

/*  Function: action_springCleaning_helper()
    Goal:     Helper for springCleaning function, which is virtual function that depends on
              the player type; for each player, call their relevant springCleaning function */
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

/*  Function: action_poisonUnicorn_helper()
    Goal:     Helper for poisonUnicorn function, which is virtual function that depends on
              the player type; call poisonUnicorn, which returns which player to target and 
              which unicorn to poison; then, remove the unicorn from the target player's
              hand and discard it */
void Player::action_poisonUnicorn_helper()
{
    Player *targetPlayer;
    int targetUnicorn;
    std::tie(targetPlayer, targetUnicorn) = this->action_poisonUnicorn();

    if(targetPlayer != NULL) 
    {
        std::cout << std::endl << targetPlayer->toString() << ", BEFORE distance: " << targetPlayer->getDistance() << std::endl;

        targetPlayer->hand->removeUnicorn(targetUnicorn);
        this->deck->discardCard(UNICORN);

        std::cout << targetPlayer->toString() << ", AFTER distance: " << targetPlayer->getDistance() << std::endl;
    }
}

/*  Function: action_stealCard_helper()
    Goal:     Helper for stealCard function, which is virtual function that depends on
              the player type; call stealCard, which returns which player to steal from 
              and which card to steal; then, remove that card from the target's hand
              and add it to your own */
void Player::action_stealCard_helper()
{
    Player *targetPlayer;
    int targetCard;
    std::tie(targetPlayer, targetCard) = this->action_stealCard();
    LOG("DONE");

    if (targetPlayer != NULL)
    {
        LOG("\nbefore: current=" + std::to_string(this->hand->getDistance()));
        LOG(", target=" + std::to_string(targetPlayer->getHand()->getDistance()) + "\n");
        this->hand->addToHand(targetPlayer->getHand()->removeCard(targetCard));
        LOG("after:  current=" + std::to_string(this->hand->getDistance()));
        LOG(", target=" + std::to_string(targetPlayer->getHand()->getDistance()) + "\n");
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
        LOG("Player " + std::to_string(this->playerNum) + " drew " + this->cardInfo->getCardName(nonActionCard) + " ... ");
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
    delete this->hand;
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

bool Player::matchesGoal()
{
    std::string current = this->hand->getCurrentString();
    std::string goal    = this->hand->getGoalString();
    return current == goal;
}

std::string Player::toString()
{
    std::string handString = this->hand->toString();
    std::string generatedString = this->hand->getCurrentString();

    return handString + " --> " + generatedString;
}

std::vector<Player *> Player::getPlayersWithUnicorns()
{
    std::vector<Player *> playersWithUnicorns;
    for (int i = 0; i < this->otherPlayers.size(); i++)
    {
        if (this->otherPlayers[i]->getUnicornCount() != 0)
            playersWithUnicorns.push_back(this->otherPlayers[i]);
    }
    return playersWithUnicorns;
}

/*  Function: discardCard()
    Goal:     Remove the Nth card from your hand and put it
              back in the deck, where N = card */
void Player::discardCard(int card)
{
    char cardType = this->hand->removeCard(card);
    this->deck->discardCard(cardType);
}

void Player::moveCard(int oldIndex, int newIndex) 
{
    this->hand->moveCard(oldIndex, newIndex);
}