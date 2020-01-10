
#include "Player_Aggressive.h"

AggressivePlayer::AggressivePlayer(Deck *deck, std::string goalString, Cards *cardInfo, int playerNum)
    : Player(deck, goalString, cardInfo, playerNum)
{

}

AggressivePlayer::~AggressivePlayer()
{

}

/**** ACTION CARDS ****/
void AggressivePlayer::discardCard()
{
    Hand testHand = Hand(*this->hand); // use copy constructor to make a copy
    Hand bestHand = Hand(*this->hand);
    bool removed = false;

    // first, see if any card will IMPROVE the current hand....
    for (int i = 0; i < this->hand->getNumCards(); i++)
    {
        // try removing every single card once
        testHand.removeCard(i);
        if (testHand < bestHand)
        {                              // if testHand is better than current best hand....
            bestHand = Hand(testHand); // set as new best
            removed = true;
        }

        testHand = Hand(*this->hand); // start over
    }

    if (removed) // removed a card that improved current string...
        this->hand = &bestHand;
    else
    {
        // since there was no removal that IMPROVED our hand, remove
        // some low-impact card

        this->hand->removeCard(0);
        std::cout << "no cards removed..." << std::endl;
    }

    std::cout << "hand: " << this->hand->getNumCards() << std::endl;
    std::cout << "hand copy: " << testHand.getNumCards() << std::endl;
    std::cout << "testHand: " << &testHand << std::endl;
    std::cout << "my hand: " << this->hand << std::endl;
    std::cout << "best hand: " << &bestHand << std::endl;
}

void AggressivePlayer::springCleaning()
{

}

void AggressivePlayer::poisonUnicorn()
{

}

void AggressivePlayer::stealCard()
{

}