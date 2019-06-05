
#include "Decks.h"
#include "Player.h"

#include <vector>
#include <iostream>

void Player::drawCard(StringDeck &deck)
{
    short newCard = deck.drawCard();
    std::vector<short> currentHand;
    int numHands = allHands.size();
    int numCards;

    int i, j;
    if(numHands == 0) {
        std::vector<short> newHand;
        newHand.push_back(newCard);
        allHands.push_back(newHand);
    } else {
        for (i = 0; i < numHands; i++)
        {
            currentHand = allHands.front();
            allHands.pop_front();
            numCards = currentHand.size();

            for (j = 0; j < numCards + 1; j++)
            {
                std::vector<short> newHand(currentHand);
                newHand.insert(newHand.begin() + j, newCard);
                allHands.push_back(newHand);
                newHand = currentHand;
            }
        }
    }

    // allHands.push_back(new hand)

    // hand.push_back(newCard);
}

void Player::generateString(StringDeck &deck)
{

}

void Player::takeTurn(StringDeck &deck)
{
    drawCard(deck);
}

void Player::printAll() {
    // std::cout << "print..." << std::endl;
    // std::list<std::vector<short>>::iterator it;
    // std::vector<short> hand;
    // for (it = allHands.begin(); it != allHands.end(); it++)
    // {
    //     hand = *it;
    //     std::vector<short>::iterator it2;
    //     for (it2 = hand.begin(); it2 != hand.end(); it2++)
    //     {
    //         std::cout << *it2 << "-";
    //     }
    //     std::cout << std::endl;
    // }

    std::cout <<  allHands.size() << std:: endl;
}