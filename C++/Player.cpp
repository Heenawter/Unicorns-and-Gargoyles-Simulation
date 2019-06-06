
#include "Decks.h"
#include "Player.h"

#include <vector>
#include <iostream>

void Player::drawCard(StringDeck &deck)
{
    char newCard = deck.drawCard();
    std::vector<char> currentHand;
    std::pair<std::set<std::string>::iterator, bool> result;

    int numHands = allHands.size();
    int numCards;

    int i, j;
    if(numHands == 0) {
        std::vector<char> newHand;
        newHand.push_back(newCard);
        allHands.push_back(newHand);
    } else {
        for (i = 0; i < numHands; i++)
        {
            currentHand = allHands.front();
            allHands.pop_front();
            numCards = currentHand.size();

            for (j = 0; j <= numCards; j++)
            {
                std::vector<char> newHand(currentHand);
                newHand.insert(newHand.begin() + j, newCard);
                allHands.push_back(newHand);
            }
        }
    }
}

std::string Player::generateString(StringDeck &deck, std::vector<char> &hand, std::string current)
{
    std::vector<char>::iterator it;
    for (it = hand.begin(); it < hand.end(); it++) {
        deck.playCard(*it, current);
    }

    return current;
}

void Player::printHand(StringDeck &deck, std::vector<char> &hand)
{
    std::vector<char>::iterator it;
    for (it = hand.begin(); it < hand.end(); it++)
    {
        std::cout << "[" << deck.getCardName(*it) << "]";
    }
    std::cout << std::endl;
}

void Player::takeTurn(StringDeck &deck)
{
    drawCard(deck);
}

void Player::printAll(StringDeck &deck)
{
    std::cout << "print..." << std::endl;
    std::list<std::vector<char>>::iterator it;
    std::vector<char> hand;
    for (it = allHands.begin(); it != allHands.end(); it++)
    {
        printHand(deck, *it);
    }
}

void Player::printSize(int numCards) {
    int numPerms = allHands.size();
    int elementSize = sizeof(char) * numCards;
    int handSize = elementSize + sizeof(std::vector<char>);
    int permSize = handSize * numPerms;

    std::cout << "Number of permutations:   " << numPerms << std::endl;
    std::cout << "Size of each hand:        " << elementSize << " bytes" << std::endl; 
    std::cout << "Size of each vector:      " << handSize << " bytes" << std::endl;
    std::cout << "Size of all permutations: " << permSize << " bytes" << std::endl;
}